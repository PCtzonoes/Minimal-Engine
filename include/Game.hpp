#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include "sol.hpp"

class AssetManager;

class GameObject;

class Game {
private:
	static bool _isRunning;
	bool _isVSyncOn;
	uint32_t _ticksLastFrame;
	uint32_t _deltaTime;
	SDL_Window *_window;
	sol::state _luaState;

	~Game();

	void VSync() const;

	void UpdateDeltaTime();

	static void ProcessInputs();

	static void Render();

	void HandleCameraMovement();

	static void CheckCollisions();

	static bool HasAsset(sol::table &data, int index);

	static bool HasAsset(sol::table &data, std::string assetName);

	static void LoadAssetsFromLua(sol::table &sceneData);

	static void LoadSceneFromLua(sol::table &sceneData);

	void LoadGameObjectsFromLua(sol::table &sceneData);

	void AddComponentsFromLua(GameObject *go, sol::table &components);

	void RegisterFunctionsToLua();

	static void AddSprite(GameObject *go, sol::table &data);

	static void AddKeyboardInput(GameObject *go, sol::table &data);

	static void AddRigidBody(GameObject *go, sol::table &data);

	static void AddCollider(GameObject *go, sol::table &data);

	static void AddProjectileEmitter(GameObject *go, sol::table &data);

	static void AddLuaBehaviour(GameObject *go, sol::table &data);

	static void AddTextLabel(GameObject *go, sol::table &data);

public:
	Game();

	static SDL_Renderer *Renderer;
	static AssetManager *GlobalAssetManager;
	static glm::vec2 Gravity;
	static SDL_Event Event;
	static SDL_Rect Camera;
	static bool DebugMode;

	static bool IsRunning();

	static void GameOver(const std::string& msg = "Game Over!!!");

	static void DestroyObject(std::string objName, int layerNum);

	static void UpdateLabelText(std::string objName, std::string newText);

	static void
	UpdateLabelColor(std::string objName, unsigned char r, unsigned char g, unsigned char b,
	                 unsigned char a);

	[[nodiscard]] float DeltaTime() const;

	void Initialize(int width, int height, bool useVSync = true);

	void Update();

	void LoadScene(int sceneIndex);

	void Destroy();
};
