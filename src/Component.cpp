#include "Component.hpp"

bool Component::IsEnabled() const { return _isEnabled; }

void Component::SetEnable(bool enabled, bool triggerCallbacks) {
	if (_isEnabled == enabled) {
		return;
	}

	_isEnabled = enabled;
	if (!triggerCallbacks) {
		return;
	}

	if (enabled) {
		OnEnable();
	} else {
		OnDisable();
	}
}
