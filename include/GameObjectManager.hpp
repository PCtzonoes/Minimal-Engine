#pragma once

#include "Component.hpp"
#include "GameObject.hpp"
#include <queue>
#include <vector>
#include <string>
#include <map>

class GameObject;

class Component;

class GameObjectManager {
private:
	std::map<LayerType, std::vector<GameObject *>> _gameObjects;
	std::queue<GameObject *>                       _toDestroy;

public:
	void Update(uint32_t deltaTime);

	void Render() const;

	void ClearData();

	void TryDestroyObjects();

	void AddObjectToDestroyQ(GameObject *obj);

	[[nodiscard]] bool IsEmpty() const;

	[[nodiscard]] uint64_t GetGameObjectCount() const;

	[[nodiscard]] std::vector<GameObject *> GetGameObjects() const;

	[[nodiscard]] std::vector<GameObject *> GetGameObjects(LayerType layer) const;

	GameObject *FindGameObject(const std::string &gameObjectName,
														 LayerType          layer);

	GameObject &AddGameObject(std::string gameObjectName, LayerType layer);

	GameObject &AddGameObject(std::string gameObjectName, int posX, int posY,
														int scaleX, int scaleY, LayerType layer);

	CollisionTagType CheckCollisions(GameObject &go) const;

	static void CheckAllCollisions();
};