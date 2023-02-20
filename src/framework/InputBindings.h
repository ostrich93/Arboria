#ifndef __INPUT_BINDINGS_H__
#define __INPUT_BINDINGS_H__

#include "InputActionType.h"
#include "../containers/List.h"

namespace Arboria {

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
			bool repeatLock;
		public:
			InputBinding(InputBindType _bindType, InputActionType _actionType, int _value = -1) : bindType(_bindType), actionType(_actionType), value(_value), repeatLock(false) {}
			int getValue() { return value; }
			InputBindType getBindType() { return bindType; }
			InputActionType getActionType() { return actionType; }
			bool hasValue(const int v) const { return value == v; }
			void setBindValue(int _value) { value = _value; }
	};

	//This will be read in through a file called "inputbindings.ini" but for now, we'll have a default set.
	class InputConfiguration {
		protected:
			List<InputBinding> inputBindings;
		public:
			InputConfiguration() {}
			~InputConfiguration();
			void initialize(); //does the reading from the configuration file(s).
			InputActionType getActionFromInputValue(InputBindType _bindType, int inputCode);
			int getBoundValue(InputActionType _actionType, InputBindType _inputBindType);
	};
}

#endif