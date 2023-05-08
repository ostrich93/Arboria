#ifndef INPUT_BINDINGS_H
#define INPUT_BINDINGS_H

#include "InputActionType.h"
#include "../containers/List.h"

namespace Arboria {

	class InputManager;

	enum InputBindType {
		BIND_KEYBOARD,
		BIND_MOUSE,
		BIND_CONTROLLER_BUTTON,
		BIND_CONTROLLER_AXIS
	};

	class InputBinding {
		private:
			InputBindType bindType;
			InputActionType actionType;
			int value;
		public:
			InputBinding() : bindType(InputBindType::BIND_KEYBOARD), actionType(InputActionType::INVALID), value(-1) {}
			InputBinding(InputBindType _bindType, InputActionType _actionType, int _value = -1) : bindType(_bindType), actionType(_actionType), value(_value), repeatLock(false) {}
			int getValue() { return value; }
			InputBindType getBindType() { return bindType; }
			void setBindType(InputBindType bType) { bindType = bType; }
			InputActionType getActionType() { return actionType; }
			void setActionType(InputActionType aType) { actionType = aType; }
			bool hasValue(const int v) const { return value == v; }
			void setValue(int _value) { value = _value; }
			bool repeatLock;
	};

	//This will be read in through a file called "inputbindings.ini" but for now, we'll have a default set.
	class InputConfiguration {
		protected:
			List<InputBinding> inputBindings;
			friend class InputManager;
		public:
			InputConfiguration() {}
			~InputConfiguration();
			void initialize(); //does the reading from the configuration file(s).
			InputActionType getActionFromInputValue(InputBindType _bindType, int inputCode);
			int getBoundValue(InputActionType _actionType, InputBindType _inputBindType);
	};
}

#endif