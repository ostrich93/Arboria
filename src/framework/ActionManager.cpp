#include "ActionManager.h"
#include "Lexer.h"
#include "../FileSystem.h"

namespace Arboria {
	bool ActionManager::parseActonBindings(Lexer& lex)
	{
		Token tok;
		InputAction act;
		ActionBinding* bnd = nullptr;

		if (!lex.expectTokenString("=")) return false;
		if (!lex.expectTokenString("(")) return false;

		while (lex.readToken(&tok)) {
			if (tok == ")") return true;
			lex.unreadToken(&tok);
			if (tok == "name") {
				lex.expectTokenString("=");
				lex.expectTokenType(TOKENTYPE_STRING, 0, &tok);
				bnd = getActionBinding(tok);
				if (bnd == nullptr) {
					Engine::printError("Action binding %s was not found.", tok);
					return false;
				}
			}
			if (tok == "inputDeviceType") {
				lex.expectTokenString("=");
				lex.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				act.setInputDeviceType(static_cast<InputDeviceType>(tok.getIntegerValue()));
			}
			if (tok == "mods") {
				lex.expectTokenString("=");
				lex.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				act.setMods(tok.getUnsignedIntegerValue());
			}
			if (tok == "key") {
				lex.expectTokenString("=");
				lex.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				act.setKey(tok.getUnsignedIntegerValue());
			}
		}

		return true;
	}
	String& ActionManager::convertToString(const ActionBinding& b, const InputAction& act)
	{
		String bindString = "Action =(name=\"\"" + b.actionName + "\" inputDeviceType=" + static_cast<uint8_t>(act.getDeviceType()) + " mods="
			+ act.getMods() + " key=" + act.getKey() + ")\n";
		return bindString;
	}
	ActionManager::ActionManager() {

		ActionBinding confirmBinding{ 1, "Confirm" };
		InputAction defaultConfirm{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_X };
		confirmBinding.defaultBindings.append(defaultConfirm);
		actions.append(confirmBinding);

		ActionBinding cancelBinding{ 2, "Cancel" };
		InputAction defaultCancel{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_Z };
		cancelBinding.defaultBindings.append(defaultCancel);
		actions.append(cancelBinding);

		ActionBinding unitMenuBinding{ 3, "Unit Menu" };
		InputAction defaultUMBinding{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_A };
		unitMenuBinding.defaultBindings.append(defaultUMBinding);
		actions.append(unitMenuBinding);

		ActionBinding helpBinding{ 4, "Help" };
		InputAction defaultHelp{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_S };
		helpBinding.defaultBindings.append(defaultHelp);
		actions.append(helpBinding);

		ActionBinding startBinding{ 5, "Start" };
		InputAction defaultStart{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_RETURN };
		startBinding.defaultBindings.append(defaultStart);
		actions.append(startBinding);

		ActionBinding upBinding{ 6, "Up" };
		InputAction defaultUp{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_UP };
		upBinding.defaultBindings.append(defaultUp);
		actions.append(upBinding);

		ActionBinding downBinding{ 7, "Down" };
		InputAction defaultDown{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_DOWN };
		downBinding.defaultBindings.append(defaultDown);
		actions.append(downBinding);

		ActionBinding leftBinding{ 8, "Left" };
		InputAction defaultLeft{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_LEFT };
		leftBinding.defaultBindings.append(defaultLeft);
		actions.append(leftBinding);

		ActionBinding rightBinding{ 9, "Right" };
		InputAction defaultRight{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_RIGHT };
		rightBinding.defaultBindings.append(defaultRight);
		actions.append(rightBinding);

		ActionBinding lShiftBinding{ 10, "LShift" };
		InputAction defaultLShift{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_D };
		lShiftBinding.defaultBindings.append(defaultLShift);
		actions.append(lShiftBinding);

		ActionBinding rShiftBinding{ 11, "RShift" };
		InputAction defaultRShift{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_C };
		rShiftBinding.defaultBindings.append(defaultRShift);
		actions.append(rShiftBinding);

		ActionBinding quitBinding{ 12, "Quit" };
		InputAction defaultQuit{ InputDeviceType::INPUT_DEVICE_KEYBOARD, 0, SDL_SCANCODE_ESCAPE };
		quitBinding.defaultBindings.append(defaultQuit);
		quitBinding.currentBindings.append(defaultQuit);
		actions.append(quitBinding);

		if (!loadUserBindings()) {
			confirmBinding.currentBindings.append(defaultConfirm);
			cancelBinding.currentBindings.append(defaultCancel);
			unitMenuBinding.currentBindings.append(defaultUMBinding);
			helpBinding.currentBindings.append(defaultHelp);
			startBinding.currentBindings.append(defaultStart);
			upBinding.currentBindings.append(defaultUp);
			downBinding.currentBindings.append(defaultDown);
			leftBinding.currentBindings.append(defaultLeft);
			rightBinding.currentBindings.append(defaultRight);
			lShiftBinding.currentBindings.append(defaultLShift);
			rShiftBinding.currentBindings.append(defaultRShift);
		}
	}

	bool ActionManager::loadUserBindings()
	{
		void* fileBuffer;
		int fLength = readFileFromPhysFS("inputs.ini", &fileBuffer);
		if (!fLength) return false;

		Lexer lex;
		Token t;
		lex.loadMemory((char*)fileBuffer, fLength, 0);
		do {
			if (!lex.readToken(&t))
				return false;
			if (t == "Action") {
				if (!parseActonBindings(lex))
					return false;
			}
		} while (1);

		return true;
	}

	void ActionManager::saveUserBindings()
	{
		PHYSFS_File* outFile = PHYSFS_openWrite("inputs.ini");
		for (ActionBinding b : actions) {
			for (InputAction act : b.currentBindings) {
				String bindString = convertToString(b, act);
				if (&b == actions.end()) { //last element, end the string
					bindString[bindString.length() - 1] = '\0';
				}
				//write to PHYSFS
				PHYSFS_writeBytes(outFile, (void*)bindString.c_str(), bindString.length());
			}
		}
		PHYSFS_close(outFile);
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

	void ActionManager::updateAction(const AEvent& e, String name, InputDeviceType deviceMode, int32_t _key)
	{
		ActionBinding* bindingToUpdate = getActionBinding(name);
		InputAction* actionToUpdate = nullptr;

		for (int i = 0; i < bindingToUpdate->currentBindings.getLength(); i++) {
			if (bindingToUpdate->currentBindings[i].getDeviceType() == deviceMode) {
				bool matchesInput = false;
				switch (e.inputDeviceType) {
					case InputDeviceType::INPUT_DEVICE_KEYBOARD:
						matchesInput = bindingToUpdate->currentBindings[i].getKey() == e.keyboardEvent.keyCode &&
							bindingToUpdate->currentBindings[i].getMods() == e.keyboardEvent.mods;
						break;
					default:
						break;
				}
				if (matchesInput) {
					actionToUpdate = &bindingToUpdate->currentBindings[i];
					break;
				}
			}
		}

		if (actionToUpdate == nullptr) {
			return;
		}

		InputAction* matchingAction = nullptr;

		for (int j = 0; j < actions.getLength(); j++) {
			for (int k = 0; k < actions[j].getCurrentBindings().getLength(); k++) {
				InputAction currBinding = actions[j].getCurrentBindings()[k];
				if (currBinding.getDeviceType() == actionToUpdate->getDeviceType() &&
					currBinding.getKey() == _key) {
					matchingAction = &currBinding;
					break;
				}
			}
			if (matchingAction)
				break;
		}

		if (matchingAction) {
			matchingAction->setKey(actionToUpdate->getKey());
		}
		
		actionToUpdate->setKey(_key);
	}

	void ActionManager::restoreDefaults()
	{
		for (int i = 0; i < actions.getLength(); i++) {
			actions[i].currentBindings = actions[i].defaultBindings;
		}
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