#include "InputBindings.h"
#include "Engine.h"

namespace Arboria {
	InputConfiguration::~InputConfiguration() {}

	void InputConfiguration::initialize()
	{
		//For the time being, we'll be loading default values here, but that will be separated into its own function once file parsing is implemented
		InputBinding cancelBinding = inputBindings.allocateNewElement();
		cancelBinding.setActionType(InputActionType::CANCEL);
		cancelBinding.setValue(SDL_SCANCODE_Z);
		InputBinding confirmBinding = inputBindings.allocateNewElement();
		confirmBinding.setActionType(InputActionType::CONFIRM);
		confirmBinding.setValue(SDL_SCANCODE_X);
		InputBinding unitMenuBinding = inputBindings.allocateNewElement();
		unitMenuBinding.setActionType(InputActionType::UNIT_MENU);
		unitMenuBinding.setValue(SDL_SCANCODE_S);
		InputBinding helpBinding = inputBindings.allocateNewElement();
		helpBinding.setActionType(InputActionType::HELP);
		helpBinding.setValue(SDL_SCANCODE_E);
		InputBinding startBinding = inputBindings.allocateNewElement();
		startBinding.setActionType(InputActionType::START);
		startBinding.setValue(SDL_SCANCODE_RETURN);
		InputBinding upBinding = inputBindings.allocateNewElement();
		upBinding.setActionType(InputActionType::UP);
		upBinding.setValue(SDL_SCANCODE_UP);
		InputBinding downBinding = inputBindings.allocateNewElement();
		downBinding.setActionType(InputActionType::DOWN);
		downBinding.setValue(SDL_SCANCODE_DOWN);
		InputBinding leftBinding = inputBindings.allocateNewElement();
		leftBinding.setActionType(InputActionType::LEFT);
		leftBinding.setValue(SDL_SCANCODE_LEFT);
		InputBinding rightBinding = inputBindings.allocateNewElement();
		rightBinding.setActionType(InputActionType::RIGHT);
		rightBinding.setValue(SDL_SCANCODE_RIGHT);
		InputBinding lShiftBinding = inputBindings.allocateNewElement();
		lShiftBinding.setActionType(InputActionType::LSHIFT);
		lShiftBinding.setValue(SDL_SCANCODE_D);
		InputBinding rShiftBinding = inputBindings.allocateNewElement();
		rShiftBinding.setActionType(InputActionType::RSHIFT);
		rShiftBinding.setValue(SDL_SCANCODE_F);
		InputBinding quitBinding = inputBindings.allocateNewElement();
		quitBinding.setActionType(InputActionType::QUIT);
		quitBinding.setValue(SDL_SCANCODE_ESCAPE);
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