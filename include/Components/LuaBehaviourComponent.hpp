#pragma once

#include "Component.hpp"

#include "../lib/sol.hpp"
#include <utility>

class LuaBehaviourComponent : public Component {
private:
	sol::function _onAwake;
	sol::function _onUpdate;
	sol::function _onDestroy;

public:
	LuaBehaviourComponent(sol::function onAwake, sol::function onUpdate,
												sol::function onDestroy) {
		_onAwake   = std::move(onAwake);
		_onUpdate  = std::move(onUpdate);
		_onDestroy = std::move(onDestroy);
	}

	void OnAwake() override {
		if (_onAwake) {
			_onAwake();
		}
	}

	void Update(uint32_t deltaTime) override {
		if (_onUpdate) {
			_onUpdate(deltaTime);
		}
	}

	void OnDestroy() override {
		if (_onDestroy) {
			_onDestroy();
		}
	}
};
