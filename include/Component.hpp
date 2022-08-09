#pragma once

#include "GameObject.hpp"

class GameObject; // Forward Declaration

class Component // this is meant to be inherited from
{
private:
	bool _isEnabled;

public:
	GameObject *Owner;

	virtual ~Component() {}

	virtual void OnAwake() {}

	virtual void OnEnable() {}

	virtual void Update(uint32_t deltaTime) {}

	virtual void Render() {}

	virtual void OnDisable() {}

	virtual void OnDestroy() {}

	[[nodiscard]] bool IsEnabled() const;

	void SetEnable(bool enabled, bool triggerCallbacks = true);
};