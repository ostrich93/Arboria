#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "InputManager.h"
#include "String.h"

namespace Arboria {
	typedef void (*actionCallback)(void* data);
	class Lexer;

	class InputAction {
		private:
			InputDeviceType deviceType;
			uint16_t mods;
			int32_t key;

		public:
			InputAction() = default;
			InputAction(InputDeviceType _deviceType, uint16_t _mods, int32_t _key) : deviceType(_deviceType), mods(_mods), key(_key) {}
			bool matchesEvent(const AEvent& e) const;
			InputDeviceType getDeviceType() const { return deviceType; }
			void setInputDeviceType(InputDeviceType _type) { deviceType = _type; }
			uint16_t getMods() const { return mods; }
			void setMods(uint16_t _mods) { mods = _mods; }
			int32_t getKey() const { return key; }
			void setKey(uint32_t _key) { key = _key; }
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
			bool parseActonBindings(Lexer& lex);
			String& convertToString(const ActionBinding& b, const InputAction& act);
		public:
			ActionManager();
			ActionManager(const ActionManager&) = delete;
			bool loadUserBindings();
			void saveUserBindings();

			ActionBinding* getActionBinding(int id);
			ActionBinding* getActionBinding(String name);
			ActionBinding* getActionBinding(const AEvent& e);
			int getAction(const AEvent& e);
			void updateAction(const AEvent& e, String name, InputDeviceType deviceMode, int32_t _key);
			void restoreDefaults();
	};

	inline bool InputAction::matchesEvent(const AEvent& e) const {
		if (e.inputDeviceType != deviceType)
			return false;

		if (e.eventType == EventType::EVENT_UI_INTERACTION) {
			if (e.inputDeviceType == InputDeviceType::INPUT_DEVICE_KEYBOARD) {
				return (e.guiEvent.keyInfo.scancode == key && (e.guiEvent.keyInfo.mods == (mods | KMOD_NUM)));
			}
		}

		if (e.inputDeviceType == InputDeviceType::INPUT_DEVICE_KEYBOARD) {
			return (e.keyboardEvent.scancode == key && e.keyboardEvent.mods == (mods | KMOD_NUM));
		}
		else if (e.inputDeviceType == InputDeviceType::INPUT_DEVICE_CONTROLLER_BUTTON) {
			return (e.controllerEvent.button == key);
		}

		return false;
	}
}

#endif