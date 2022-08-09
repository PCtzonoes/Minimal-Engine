#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "Component.hpp"
#include "Components/TransformComponent.hpp"

#include <iostream>
#include <utility>

GameObject::GameObject(GameObjectManager &manager) : _manager(manager) {
	_isActive = true;
	_transform = &AddComponent<TransformComponent>();
}

GameObject::GameObject(GameObjectManager &manager, std::string name, LayerType layer) : _manager(
		manager), Name(std::move(name)), Layer(layer) {
	_isActive = true;
	_transform = &AddComponent<TransformComponent>();
}

GameObject::GameObject(GameObjectManager &manager, std::string name, int posX, int posY, int scaleX,
                       int scaleY, LayerType layer) : _manager(manager), Name(std::move(name)), Layer(layer) {
	_isActive = true;
	_transform = &AddComponent<TransformComponent>(posX, posY, scaleX, scaleY);
}

void GameObject::Update(uint32_t deltaTime) {

	for (auto &comp: _components) {
		if (!_isActive) {
			return;
		}
		comp->Update(deltaTime);
	}
}

void GameObject::Render() {
	for (auto &comp: _components) {
		comp->Render();
	}
}

void GameObject::Destroy() {
	_manager.AddObjectToDestroyQ(this);
}

void GameObject::ClearData() {
	_isActive = false;
	for (auto &comp: _components) {
		comp->OnDisable();
	}
	for (auto &comp: _components) {
		comp->OnDestroy();
	}
	for (uint64_t i = 0, to = _components.size(); i < to; ++i) {
		delete _components[i];
	}
	_components.clear();
	delete this;
}

bool GameObject::IsActive() const {
	return _isActive;
}

void GameObject::SetActive(bool active) {
	if (_isActive == active) {
		return;
	}

	_isActive = active;
	for (auto &comp: _components) {
		if (comp->IsEnabled()) {
			if (active) {
				comp->OnEnable();
			} else {
				comp->OnDisable();
			}
		}
	}
}

TransformComponent *GameObject::GetTransform() const {
	return _transform;
}

GameObjectManager *GameObject::GetManager() const {
	return &_manager;
}


