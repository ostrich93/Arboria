#include "InputBindings.h"

namespace Arboria {
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