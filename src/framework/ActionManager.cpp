#include "ActionManager.h"

namespace Arboria {

	ActionManager::ActionManager() {
		ActionBinding confirmBinding{ 1, "Confirm" };
		InputAction defaultConfirm{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_X };
		confirmBinding.defaultBindings.append(defaultConfirm);
		confirmBinding.currentBindings.append(defaultConfirm);
		actions.append(confirmBinding);

		ActionBinding cancelBinding{ 2, "Cancel" };
		InputAction defaultCancel{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_Z };
		cancelBinding.defaultBindings.append(defaultCancel);
		cancelBinding.currentBindings.append(defaultCancel);
		actions.append(cancelBinding);

		ActionBinding unitMenuBinding{ 3, "UnitMenu" };
		InputAction defaultUMBinding{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_A };
		unitMenuBinding.defaultBindings.append(defaultUMBinding);
		unitMenuBinding.currentBindings.append(defaultUMBinding);
		actions.append(unitMenuBinding);

		ActionBinding helpBinding{ 4, "Help" };
		InputAction defaultHelp{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_S };
		helpBinding.defaultBindings.append(defaultHelp);
		helpBinding.currentBindings.append(defaultHelp);
		actions.append(helpBinding);

		ActionBinding startBinding{ 5, "Start" };
		InputAction defaultStart{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_RETURN };
		startBinding.defaultBindings.append(defaultStart);
		startBinding.currentBindings.append(defaultStart);
		actions.append(startBinding);

		ActionBinding upBinding{ 6, "Up" };
		InputAction defaultUp{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_UP };
		upBinding.defaultBindings.append(defaultUp);
		upBinding.currentBindings.append(defaultUp);
		actions.append(upBinding);

		ActionBinding downBinding{ 7, "Down" };
		InputAction defaultDown{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_DOWN };
		downBinding.defaultBindings.append(defaultDown);
		downBinding.currentBindings.append(defaultDown);
		actions.append(downBinding);

		ActionBinding leftBinding{ 8, "Left" };
		InputAction defaultLeft{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_LEFT };
		leftBinding.defaultBindings.append(defaultLeft);
		leftBinding.currentBindings.append(defaultLeft);
		actions.append(leftBinding);

		ActionBinding rightBinding{ 9, "Right" };
		InputAction defaultRight{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_RIGHT };
		rightBinding.defaultBindings.append(defaultRight);
		rightBinding.currentBindings.append(defaultRight);
		actions.append(rightBinding);

		ActionBinding lShiftBinding{ 10, "LShift" };
		InputAction defaultLShift{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_D };
		lShiftBinding.defaultBindings.append(defaultLShift);
		lShiftBinding.currentBindings.append(defaultLShift);
		actions.append(lShiftBinding);

		ActionBinding rShiftBinding{ 11, "RShift" };
		InputAction defaultRShift{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_C };
		rShiftBinding.defaultBindings.append(defaultRShift);
		rShiftBinding.currentBindings.append(defaultRShift);
		actions.append(rShiftBinding);

		ActionBinding quitBinding{ 12, "Quit" };
		InputAction defaultQuit{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_ESCAPE };
		quitBinding.defaultBindings.append(defaultQuit);
		quitBinding.currentBindings.append(defaultQuit);
		actions.append(quitBinding);
	}

	ActionBinding* ActionManager::getActionBinding(const AEvent& e) {
		int i, j;
		ActionBinding* retBinding = NULL;
		for (i = 0; i < actions.getLength(); i++) {
			ActionBinding binding = actions[i];
			for (j = 0; j < binding.getCurrentBindings().getLength(); j++) {
				if (binding.getCurrentBindings()[j].matchesEvent(e)) {
					retBinding = &binding;
					break;
				}
			}
			if (retBinding) {
				break;
			}
		}

		return retBinding;
	}

	int ActionManager::getAction(const AEvent& e)
	{
		int i, j;
		List<InputAction> currBindings;
		for (i = 0; i < actions.getLength(); i++) {
			currBindings = actions[i].getCurrentBindings();
			for (j = 0; j < currBindings.getLength(); j++) {
				if (currBindings[j].matchesEvent(e)) {
					return actions[i].getId();
				}
			}
		}
		return 0;
	}

	ActionBinding* ActionManager::getActionBinding(int id) {
		for (int i = 0; i < actions.getLength(); i++) {
			if (actions[i].getId() == id) {
				return &actions[i];
			}
		}
		return NULL;
	}

	ActionBinding* ActionManager::getActionBinding(String name) {
		for (int i = 0; i < actions.getLength(); i++) {
			if (actions[i].getName().compare(name) == 0) {
				return &actions[i];
			}
		}
		return NULL;
	}
}