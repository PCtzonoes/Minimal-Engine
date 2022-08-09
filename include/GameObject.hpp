#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "GameObjectManager.hpp"
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

class GameObjectManager;

class Component;

class TransformComponent;

class SpriteComponent;

class GameObject {
private:
	GameObjectManager                            &_manager;
	bool                                          _isActive;
	std::vector<Component *>                      _components;
	std::map<const std::type_info *, Component *> _componentTypeMap;
	TransformComponent                           *_transform;
	bool                                          _canDestroy = false;

public:
	GameObjectManager *GetManager() const;

	std::string Name;
	LayerType   Layer;

	explicit GameObject(GameObjectManager &manager);

	GameObject(GameObjectManager &manager, std::string name, LayerType layer);

	GameObject(GameObjectManager &manager, std::string name, int posX, int posY,
						 int scaleX, int scaleY, LayerType layer);

	void Update(uint32_t deltaTime);

	void Render();

	void Destroy();

	void ClearData();

	void SetActive(bool active);

	[[nodiscard]] bool IsActive() const;

	[[nodiscard]] TransformComponent *GetTransform() const;

	template <typename T, typename... TArgs> T &AddComponent(TArgs &&...args) {
		T *newComponent     = new T(std::forward<TArgs>(args)...);
		newComponent->Owner = this;
		_components.emplace_back(newComponent);
		_componentTypeMap[&typeid(*newComponent)] = newComponent;
		newComponent->OnAwake();
		newComponent->OnEnable();
		return *newComponent;
	}

	template <typename T> T *GetComponent() {
		return static_cast<T *>(_componentTypeMap[&typeid(T)]);
	}
};