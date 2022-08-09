#include <algorithm>
#include "GameObjectManager.hpp"
#include "Collision.hpp"
#include "Components/ColliderComponent.hpp"

void GameObjectManager::ClearData() {
	for (auto &layerVector: _gameObjects) {
		for (auto &go: layerVector.second) {
			go->Destroy();
		}
	}
}

void GameObjectManager::Update(uint32_t deltaTime) {
	for (auto &layerVector: _gameObjects) {
		for (auto &go: layerVector.second) {
			go->Update(deltaTime);
		}
	}
}

void GameObjectManager::Render() const {
	for (int layerNum = 0; layerNum < LAYERS_COUNT; layerNum++) {
		for (auto &go: GetGameObjects(static_cast<LayerType>(layerNum))) {
			go->Render();
		}
	}
}

void GameObjectManager::TryDestroyObjects() {
	while (!_toDestroy.empty()) {
		auto obj = _toDestroy.front();
		_toDestroy.pop();

		auto &vec = _gameObjects[obj->Layer];
		auto itr = std::find(vec.begin(), vec.end(), obj);
		if (itr != vec.end()) {
			obj->ClearData();

			auto whichToRemove = std::remove(vec.begin(), vec.end(), obj);
			vec.erase(whichToRemove, vec.end());
		}
	}
}

void GameObjectManager::AddObjectToDestroyQ(GameObject *obj) {
	_toDestroy.emplace(obj);
}

GameObject &GameObjectManager::AddGameObject(std::string gameObjectName, LayerType layer) {
	GameObject *go = new GameObject(*this, gameObjectName, layer);
	_gameObjects[layer].emplace_back(go);
	return *go;
}

GameObject &
GameObjectManager::AddGameObject(std::string gameObjectName, int posX, int posY, int scaleX,
                                 int scaleY, LayerType layer) {
	GameObject *go = new GameObject(*this, gameObjectName, posX, posY, scaleX, scaleY, layer);
	_gameObjects[layer].emplace_back(go);
	return *go;
}

CollisionTagType GameObjectManager::CheckCollisions(GameObject &gameObject) const {
	auto col = gameObject.GetComponent<ColliderComponent>();
	if (col == nullptr) {
		return CollisionTagType::None;
	}

	for (auto &otherCol: Collision::Colliders) {
		if (col == otherCol) {
			continue;
		}
		if (Collision::CheckRectangleCollision(col->Collider, otherCol->Collider)) {
			return otherCol->ColliderTag;
		}
	}

	return CollisionTagType::None;
}

void GameObjectManager::CheckAllCollisions() {
	uint64_t amountOfColliders = Collision::Colliders.size();
	for (uint64_t i = 0; i < amountOfColliders - 1; ++i) {
		auto thisCol = Collision::Colliders[i];
		for (uint64_t j = i + 1; j < amountOfColliders; ++j) {
			auto thatCol = Collision::Colliders[j];
			if (Collision::CheckRectangleCollision(thisCol->Collider, thatCol->Collider)) {
				thisCol->OnCollisionWith(thatCol);
				thatCol->OnCollisionWith(thisCol);
			}
		}
	}
}

bool GameObjectManager::IsEmpty() const {
	return _gameObjects.empty();
}

[[maybe_unused]] uint64_t GameObjectManager::GetGameObjectCount() const {
	uint64_t count = 0;
	for (auto &layerVector: _gameObjects) {
		count += layerVector.second.size();
	}
	return count;
}

std::vector<GameObject *> GameObjectManager::GetGameObjects() const {
	std::vector<GameObject *> selectedGameObjects;
	for (auto &layerVector: _gameObjects) {
		selectedGameObjects.insert(selectedGameObjects.begin(), layerVector.second.begin(),
		                           layerVector.second.end());
	}
	return selectedGameObjects;
}

std::vector<GameObject *> GameObjectManager::GetGameObjects(LayerType layer) const {
	if (_gameObjects.find(layer) == _gameObjects.end()) {
		return std::vector<GameObject *>();
	}
	return _gameObjects.at(layer);
}

GameObject *GameObjectManager::FindGameObject(const std::string& gameObjectName, LayerType layer) {
	for (auto obj: _gameObjects[layer]) {
		if (obj->Name == gameObjectName) {
			return obj;
		}
	}
	return nullptr;
}
