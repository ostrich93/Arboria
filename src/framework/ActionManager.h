#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "InputManager.h"
#include "String.h"

namespace Arboria {
	typedef void (*actionCallback)(void* data);

	/*struct ActionBinding {
		InputDeviceType deviceType;
		InputActionType actionValue;
		AEvent::JoystickEvent::JoystickAxis axis;
		int value;
	};*/

	class InputAction {
		private:
			InputDeviceType deviceType;
			uint16_t mods;
			int32_t key;

		public:
			InputAction() = default;
			InputAction(InputDeviceType _deviceType, uint16_t _mods, int32_t _key) : deviceType(_deviceType), mods(_mods), key(_key) {}
			bool matchesEvent(const AEvent& e) const;
	};

	class ActionBinding {
		friend class ActionManager;
		private:
			int actionId;
			String actionName;
			List<InputAction> defaultBindings;
			List<InputAction> currentBindings;
		public:
			ActionBinding() : actionId(0), actionName("") {}
			ActionBinding(int id, String name) : actionId(id), actionName(name) {}
			int getId() { return actionId; }
			String getName() { return actionName; }
			List<InputAction> getDefaultBindings() { return defaultBindings; }
			List<InputAction> getCurrentBindings() { return currentBindings; }

	};

	class ActionManager {
		private:
			List<ActionBinding> actions;
		public:
			ActionManager();
			ActionManager(const ActionManager&) = delete;
			void loadUserBindings();
			void saveUserBindings();

			ActionBinding* getActionBinding(int id);
			ActionBinding* getActionBinding(String name);
			ActionBinding* getActionBinding(const AEvent& e);
			int getAction(const AEvent& e);
	};

	inline bool InputAction::matchesEvent(const AEvent& e) const {
		if (e.inputDeviceType != deviceType)
			return false;

		if (e.inputDeviceType == InputDeviceType::INPUT_DEVICE_KEYBOARD) {
			return (e.keyboardEvent.scancode == key && e.keyboardEvent.mods == mods);
		}
		else if (e.inputDeviceType == InputDeviceType::INPUT_DEVICE_CONTROLLER_BUTTON) {
			return (e.controllerEvent.button == key);
		}

		return false;
	}
}

#endif