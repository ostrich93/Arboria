#include "InputBindings.h"
#include "Engine.h"

namespace Arboria {
	InputConfiguration::~InputConfiguration() {}

	void InputConfiguration::initialize()
	{
		//For the time being, we'll be loading default values here, but that will be separated into its own function once file parsing is implemented
		inputBindings.append({ BIND_KEYBOARD, InputActionType::CANCEL, SDL_SCANCODE_Z });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::CONFIRM, SDL_SCANCODE_X });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::UNIT_MENU, SDL_SCANCODE_S });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::HELP, SDL_SCANCODE_E });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::START, SDL_SCANCODE_RETURN });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::UP, SDL_SCANCODE_UP });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::DOWN, SDL_SCANCODE_DOWN });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::LEFT, SDL_SCANCODE_LEFT });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::RIGHT, SDL_SCANCODE_RIGHT });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::LSHIFT, SDL_SCANCODE_D });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::RSHIFT, SDL_SCANCODE_F });
		inputBindings.append({ BIND_KEYBOARD, InputActionType::QUIT, SDL_SCANCODE_ESCAPE });
	}

	InputActionType InputConfiguration::getActionFromInputValue(InputBindType _bindType, int inputCode) {
		InputActionType retVal = InputActionType::INVALID;
		for (int i = 0; i < inputBindings.getLength(); i++) {
			if (inputBindings[i].getBindType() == _bindType && inputBindings[i].getValue() == inputCode) {
				retVal = inputBindings[i].getActionType();
				break;
			}
		}
		return retVal;
	}

	int InputConfiguration::getBoundValue(InputActionType _actionType, InputBindType _inputBindType) {
		int _bv = -1;
		for (int i = 0; i < inputBindings.getLength(); i++) {
			if (inputBindings[i].getActionType() == _actionType && inputBindings[i].getBindType() == _inputBindType) {
				_bv = inputBindings[i].getValue();
				break;
			}
		}
		return _bv;
	}
}