#include "Game.hpp"
#include "AssetManager.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/KeyboardInputComponent.hpp"
#include "Components/LifeSpanComponent.hpp"
#include "Components/LuaBehaviourComponent.hpp"
#include "Components/ProjectileEmitterComponent.hpp"
#include "Components/RigidbodyComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TextLabelComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Constants.hpp"
#include "GameObjectManager.hpp"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <utility>

GameObjectManager Manager;
AssetManager     *Game::GlobalAssetManager = new AssetManager(&Manager);
SDL_Renderer     *Game::Renderer;
glm::vec2         Game::Gravity = glm::vec2(0.0f, 9.81f);
SDL_Event         Game::Event;
SDL_Rect          Game::Camera     = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
bool              Game::DebugMode  = false;
bool              Game::_isRunning = false;

Game::Game() {
	_isVSyncOn       = true;
	Game::_isRunning = false;
	_ticksLastFrame  = 0;
	_deltaTime       = 0.0f;
}

Game::~Game() {}

bool Game::IsRunning() { return Game::_isRunning; }

void Game::GameOver(const std::string &msg) {
	std::cout << msg << std::endl;
	Game::_isRunning = false;
}

void Game::DestroyObject(std::string objName, int layerNum) {
	auto layer = static_cast<LayerType>(layerNum);
	auto go    = Manager.FindGameObject(std::move(objName), layer);
	go->Destroy();
}

void Game::UpdateLabelText(std::string objName, std::string newText) {
	auto go      = Manager.FindGameObject(std::move(objName), LayerType::UI);
	auto txtComp = go->GetComponent<TextLabelComponent>();
	if (txtComp != nullptr) {
		txtComp->SetLabelText(std::move(newText));
	}
}

void Game::UpdateLabelColor(std::string objName, u_char r, u_char g, u_char b,
														u_char a) {
	auto go      = Manager.FindGameObject(std::move(objName), LayerType::UI);
	auto txtComp = go->GetComponent<TextLabelComponent>();
	if (txtComp != nullptr) {
		SDL_Color color = {r, g, b, a};
		txtComp->SetLabelColor(color);
	}
}

float Game::DeltaTime() const { return _deltaTime; }

void Game::Initialize(int width, int height, bool useVSync) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error Initializing SDL2." << std::endl;
		return;
	}

	if (TTF_Init() != 0) {
		std::cerr << "Error Initializing SDL_TTF" << std::endl;
		return;
	}

	_window =
			SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											 width, height, SDL_WINDOW_BORDERLESS);
	if (!_window) {
		std::cerr << "Error Creating SDL2 Window." << std::endl;
		return;
	}

	Renderer = SDL_CreateRenderer(_window, -1, 0);
	if (!Renderer) {
		std::cerr << "Error Creating SDL2 Renderer." << std::endl;
		return;
	}

	LoadScene(0);

	_isVSyncOn       = useVSync;
	Game::_isRunning = true;
}

void Game::ProcessInputs() {
	SDL_PollEvent(&Event);
	switch (Event.type) {
	case SDL_QUIT:
		Game::GameOver("Quit...");
		break;

	case SDL_KEYDOWN:
		if (Event.key.keysym.sym == SDLK_ESCAPE) {
			Game::GameOver("Quit...");
		}
		if (Event.key.keysym.sym == SDLK_F1) {
			DebugMode = !DebugMode;
		}
		break;

	default:
		break;
	}
}

void Game::VSync() const {
	if (!_isVSyncOn) {
		return;
	}

	// Good VSync
	auto timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - _ticksLastFrame);
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
		SDL_Delay(timeToWait);
	}
}

void Game::UpdateDeltaTime() {
	_deltaTime      = (SDL_GetTicks() - _ticksLastFrame) / 1000u;
	_deltaTime      = _deltaTime > MAX_FRAME_TIME ? MAX_FRAME_TIME : _deltaTime;
	_ticksLastFrame = SDL_GetTicks();
}

void Game::Update() {
	VSync();
	UpdateDeltaTime();
	ProcessInputs();
	Manager.Update(_deltaTime);
	HandleCameraMovement();
	CheckCollisions();
	Render();

	// reset the input event keysym
	Game::Event.key.keysym.sym = (SDL_Keycode)0;

	Manager.TryDestroyObjects();
}

void Game::Render() {
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer); // Clears the BackBuffer
	Manager.Render();
	SDL_RenderPresent(Renderer); // Swaps & presents the back buffer

	// if (Manager.IsEmpty())
	// {
	//     return;
	// }
}

void Game::Destroy() {
	Manager.ClearData();
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	delete this; // this calls the Dtor and Frees the space in heap (ram)
}

// FIXME: This is temp code
Scene      *scene;
GameObject &playerObject =
		Manager.AddGameObject("Player", 240, 495, 1, 1, LayerType::Player);

void Game::HandleCameraMovement() {
	auto playerTr = playerObject.GetTransform();
	Camera.x      = playerTr->Position.x - (WINDOW_WIDTH / 2);
	Camera.y      = playerTr->Position.y - (WINDOW_HEIGHT / 2);

	Camera.x = Camera.x < 0 ? 0 : Camera.x;
	Camera.y = Camera.y < 0 ? 0 : Camera.y;

	int limitX = scene->GetMapPixelSizeX() - Camera.w;
	int limitY = scene->GetMapPixelSizeY() - Camera.h;
	Camera.x   = Camera.x > limitX ? limitX : Camera.x;
	Camera.y   = Camera.y > limitY ? limitY : Camera.y;
}

void Game::CheckCollisions() { GameObjectManager::CheckAllCollisions(); }

bool Game::HasAsset(sol::table &data, int index) {
	sol::optional<sol::table> hasAsset = data[index];
	return hasAsset != sol::nullopt;
}

bool Game::HasAsset(sol::table &data, std::string assetName) {
	sol::optional<sol::table> hasAsset = data[assetName];
	return hasAsset != sol::nullopt;
}

void Game::LoadAssetsFromLua(sol::table &sceneData) {
	std::cout << "\nLoading Assets: " << std::endl;
	sol::table sceneAssets = sceneData["assets"];
	uint64_t   index       = 0;
	while (true) {
		if (!HasAsset(sceneAssets, index)) {
			break;
		} else {
			sol::table  asset         = sceneAssets[index];
			std::string assetType     = asset["type"];
			std::string assetId       = asset["id"];
			std::string assetFilePath = asset["file"];

			if (assetType == "texture") {
				std::cout << "    - texture: " << assetId << ", " << assetFilePath
									<< std::endl;
				GlobalAssetManager->AddTexture(assetId, assetFilePath.c_str());
			}
			if (assetType == "font") {
				int fontSize = static_cast<int>(asset["fontSize"]);
				std::cout << "    - font: " << assetId << ", " << assetFilePath << ", "
									<< fontSize << std::endl;
				GlobalAssetManager->AddFont(assetId, assetFilePath.c_str(), fontSize);
			}
		}
		index++;
	}
}

void Game::LoadSceneFromLua(sol::table &sceneData) {
	sol::table sceneMap = sceneData["map"];

	std::string textureId = sceneMap["textureId"];
	std::string filePath  = sceneMap["file"];
	int         scale     = static_cast<int>(sceneMap["scale"]);
	int         tileSize  = static_cast<int>(sceneMap["tileSize"]);
	int         mapSizeX  = static_cast<int>(sceneMap["mapSizeX"]);
	int         mapSizeY  = static_cast<int>(sceneMap["mapSizeY"]);

	scene = new Scene(textureId, scale, tileSize);
	scene->LoadFromFile(filePath, mapSizeX, mapSizeY);
	std::cout << "\nLoading Scene: " << filePath << " with textures from "
						<< textureId << std::endl;
}

void Game::LoadGameObjectsFromLua(sol::table &sceneData) {
	sol::table   gameObjects = sceneData["gameObjects"];
	unsigned int index       = 0;
	while (true) {
		if (!HasAsset(gameObjects, index)) {
			break;
		} else {
			sol::table goData = gameObjects[index];

			std::string goName = goData["name"];
			int         posX   = static_cast<int>(goData["position"]["x"]);
			int         posY   = static_cast<int>(goData["position"]["y"]);
			int         scaleX = static_cast<int>(goData["scale"]["x"]);
			int         scaleY = static_cast<int>(goData["scale"]["y"]);
			LayerType   layer =
					static_cast<LayerType>(static_cast<int>(goData["layer"]));

			GameObject *go =
					&Manager.AddGameObject(goName, posX, posY, scaleX, scaleY, layer);
			std::cout << "\nCreated GO: " << go->Name << std::endl;

			if (HasAsset(goData, "components")) {
				sol::table components = goData["components"];
				AddComponentsFromLua(go, components);
			}
		}
		index++;
	}
}

void Game::AddComponentsFromLua(GameObject *go, sol::table &components) {
	if (HasAsset(components, "sprite")) {
		sol::table spriteData = components["sprite"];
		AddSprite(go, spriteData);
	}
	if (HasAsset(components, "rigidBody")) {
		sol::table rigidBodyData = components["rigidBody"];
		AddRigidBody(go, rigidBodyData);
	}
	if (HasAsset(components, "keyboardInput")) {
		sol::table keyboardInputData = components["keyboardInput"];
		AddKeyboardInput(go, keyboardInputData);
	}
	if (HasAsset(components, "collider")) {
		sol::table colliderData = components["collider"];
		AddCollider(go, colliderData);
	}
	if (HasAsset(components, "projectileEmitter")) {
		sol::table projectileEmitterData = components["projectileEmitter"];
		AddProjectileEmitter(go, projectileEmitterData);
	}
	if (HasAsset(components, "luaBehaviour")) {
		sol::table luaBehaviourData = components["luaBehaviour"];
		AddLuaBehaviour(go, luaBehaviourData);
	}
	if (HasAsset(components, "textLabel")) {
		sol::table textLabelData = components["textLabel"];
		AddTextLabel(go, textLabelData);
	}
}

void Game::RegisterFunctionsToLua() {
	_luaState.set_function("GameOver", &Game::GameOver);
	_luaState.set_function("DestroyObject", &Game::DestroyObject);
	_luaState.set_function("UpdateLabelText", &Game::UpdateLabelText);
	_luaState.set_function("UpdateLabelColor", &Game::UpdateLabelColor);
}

void Game::LoadScene(int sceneIndex) {
	_luaState.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

	std::string sceneName = "Scene_" + std::to_string(sceneIndex);
	_luaState.script_file("./Assets/Scripts/" + sceneName + ".lua");

	sol::table sceneData = _luaState[sceneName];

	RegisterFunctionsToLua();

	LoadAssetsFromLua(sceneData);
	LoadSceneFromLua(sceneData);
	LoadGameObjectsFromLua(sceneData);
	std::cout << "--------------------------\n" << std::endl;
}

void Game::AddSprite(GameObject *go, sol::table &data) {
	std::cout << "    <Sprite>" << std::endl;
	bool isAnimated = static_cast<bool>(data["isAnimated"]);
	if (isAnimated) {
		go->AddComponent<SpriteComponent>(
				data["textureId"], static_cast<int>(data["width"]),
				static_cast<int>(data["height"]), static_cast<int>(data["frameCount"]),
				static_cast<int>(data["animationSpeed"]),
				static_cast<bool>(data["hasDirections"]),
				static_cast<bool>(data["isFixed"]));
	} else {
		go->AddComponent<SpriteComponent>(
				data["textureId"], static_cast<int>(data["sourceX"]),
				static_cast<int>(data["sourceY"]), static_cast<int>(data["width"]),
				static_cast<int>(data["height"]), static_cast<bool>(data["isFixed"]));
	}
}

void Game::AddKeyboardInput(GameObject *go, sol::table &data) {
	std::cout << "    <KeybooardInput>" << std::endl;
	std::string up    = data["up"];
	std::string down  = data["down"];
	std::string left  = data["left"];
	std::string right = data["right"];
	std::string shoot = data["shoot"];
	go->AddComponent<KeyboardInputComponent>(up, down, left, right, shoot);
}

void Game::AddRigidBody(GameObject *go, sol::table &data) {
	std::cout << "    <RigidBody>" << std::endl;
	int   velX         = static_cast<int>(data["velX"]);
	int   velY         = static_cast<int>(data["velY"]);
	float gravityScale = static_cast<float>(data["gravityScale"]);
	go->AddComponent<RigidbodyComponent>(velX, velY, gravityScale);
}

void Game::AddCollider(GameObject *go, sol::table &data) {
	std::cout << "    <Collider>" << std::endl;
	CollisionTagType tag =
			static_cast<CollisionTagType>(static_cast<int>(data["tag"]));
	int           width    = static_cast<int>(data["width"]);
	int           height   = static_cast<int>(data["height"]);
	int           offsetX  = static_cast<int>(data["offsetX"]);
	int           offsetY  = static_cast<int>(data["offsetY"]);
	sol::function callback = data["onCollision"];

	if (callback) {
		go->AddComponent<ColliderComponent>(
				tag, width, height, offsetX, offsetY,
				[callback](ColliderComponent *other) {
					callback.call(static_cast<int>(other->ColliderTag));
				});
	} else {
		go->AddComponent<ColliderComponent>(tag, width, height, offsetX, offsetY,
																				nullptr);
	}
}

void Game::AddProjectileEmitter(GameObject *go, sol::table &data) {
	std::cout << "    <ProjectileEmitter>" << std::endl;

	LayerType objLayer =
			static_cast<LayerType>(static_cast<int>(data["refObjectLayer"]));
	std::string objName   = data["refObjectName"];
	int         offsetX   = static_cast<int>(data["offsetX"]);
	int         offsetY   = static_cast<int>(data["offsetY"]);
	int         speed     = static_cast<int>(data["speed"]);
	int         angleDeg  = static_cast<int>(data["angleDeg"]);
	int         range     = static_cast<int>(data["range"]);
	bool        isLooping = static_cast<bool>(data["isLooping"]);

	auto                obj = Manager.FindGameObject(objName, objLayer);
	TransformComponent *transformToFollow = obj->GetTransform();

	go->AddComponent<ProjectileEmitterComponent>(
			transformToFollow, offsetX, offsetY, speed, angleDeg, range, isLooping);
}

void Game::AddLuaBehaviour(GameObject *go, sol::table &data) {
	std::cout << "    <LuaBehaviour>" << std::endl;

	sol::function onAwake   = data["onAwake"];
	sol::function onUpdate  = data["onUpdate"];
	sol::function onDestroy = data["onDestroy"];
	go->AddComponent<LuaBehaviourComponent>(onAwake, onUpdate, onDestroy);
}

void Game::AddTextLabel(GameObject *go, sol::table &data) {
	go->Layer = LayerType::UI;
	std::cout << "    <TextLabel>" << std::endl;

	std::string text    = data["text"];
	std::string assetId = data["fontAssetId"];
	SDL_Color   color   = COLOR_WHITE;
	if (HasAsset(data, "fontColor")) {
		sol::table    col = data["fontColor"];
		unsigned char r   = static_cast<unsigned char>(col["r"]);
		unsigned char g   = static_cast<unsigned char>(col["g"]);
		unsigned char b   = static_cast<unsigned char>(col["b"]);
		unsigned char a   = static_cast<unsigned char>(col["a"]);
		color             = {r, g, b, a};
	}
	go->AddComponent<TextLabelComponent>(text, assetId, color);
}

// void Game::LoadScene(int sceneIndex)
// {
//     // Load Assets into AssetDatabase
//     GlobalAssetManager->AddTexture("tank-image",
//     "./Assets/Images/Tank-Big-Right.png");
//     GlobalAssetManager->AddTexture("chopper-image",
//     "./Assets/Images/Chopper-Spritesheet.png");
//     GlobalAssetManager->AddTexture("jungle-tilemap-texture",
//     "./Assets/Tilemaps/Jungle.png");
//     GlobalAssetManager->AddTexture("player-bullet-image",
//     "./Assets/Images/Bullet-Friendly.png");
//     GlobalAssetManager->AddTexture("enemy-bullet-image",
//     "./Assets/Images/Bullet-Enemy.png");
//     GlobalAssetManager->AddTexture("radar-image",
//     "./Assets/Images/Radar.png");
//     GlobalAssetManager->AddFont("charriot-20-font",
//     "./Assets/Fonts/Charriot.ttf", 20);
//
//     // Load Scene from Files
//     _scene = new Scene("jungle-tilemap-texture", 2, 32);
//     _scene->LoadFromFile("./Assets/Tilemaps/Jungle.scene", 25, 20);
//
//     // Create GameObjects & Components
//
//     GameObject &labelLevelName = Manager.AddGameObject("Label_LevelName", 32,
//     16, 100, 50, LayerType::UI);
//     labelLevelName.AddComponent<TextLabelComponent>("Level 01",
//     "charriot-20-font", COLOR_WHITE);
//
//     GameObject &radar = Manager.AddGameObject("Radar", WINDOW_WIDTH - 16 - 64
//     * 2, 16, 2, 2, LayerType::UI);
//     radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 8, 8, false,
//     true);
//
//     _playerObject.AddComponent<RigidbodyComponent>(0, 0, 0.0f);
//     _playerObject.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2,
//     16, true, false);
//     _playerObject.AddComponent<KeyboardInputComponent>(SDLK_w, SDLK_s,
//     SDLK_a, SDLK_d, SDLK_SPACE);
//     _playerObject.AddComponent<ColliderComponent>(CollisionTagType::Player,
//     32, 32, 0, 0, [this](ColliderComponent *other)
//                                                   { _isRunning = false; });
//
//     GameObject &tankObject = Manager.AddGameObject("Tank", 150, 495, 1, 1,
//     LayerType::Enemy);
//     // tankObject.AddComponent<RigidbodyComponent>(12, 0, 0.0f);
//     tankObject.AddComponent<SpriteComponent>("tank-image", 0, 0, 32, 32);
//     tankObject.AddComponent<ColliderComponent>(CollisionTagType::Enemy, 32,
//     32);
//
//     GameObject &tankBullet = Manager.AddGameObject("TankBullet", 0, 0, 1, 1,
//     LayerType::Projectile);
//     tankBullet.AddComponent<SpriteComponent>("enemy-bullet-image", 0, 0, 4,
//     4); tankBullet.AddComponent<ColliderComponent>(CollisionTagType::Enemy,
//     4, 4);
//     tankBullet.AddComponent<ProjectileEmitterComponent>(tankObject.GetTransform(),
//     16, 16, 100, 270, 200, true);
// }