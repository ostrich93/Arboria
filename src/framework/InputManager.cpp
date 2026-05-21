#include "InputManager.h"
#include "../Heap.h"
#include "../FileSystem.h"
#include "Lexer.h"

namespace Arboria {

	static const ActBinding defaultBindings[] =
	{
		{ArboriaKey_S, ArboriaKey_GamepadFaceDown},
		{ArboriaKey_X, ArboriaKey_GamepadFaceRight},
		{ArboriaKey_A, ArboriaKey_GamepadFaceLeft},
		{ArboriaKey_D, ArboriaKey_GamepadFaceUp},
		{ArboriaKey_LeftArrow, ArboriaKey_GamepadDpadLeft},
		{ArboriaKey_RightArrow, ArboriaKey_GamepadDpadRight},
		{ArboriaKey_UpArrow, ArboriaKey_GamepadDpadUp},
		{ArboriaKey_DownArrow, ArboriaKey_GamepadDpadDown},
		{ArboriaKey_W, ArboriaKey_GamepadLeftTrigger_1},
		{ArboriaKey_E, ArboriaKey_GamepadRightTrigger_1},
		{ArboriaKey_Q, ArboriaKey_GamepadLeftTrigger_2},
		{ArboriaKey_C, ArboriaKey_GamepadRightTrigger_2},
		{ArboriaKey_R, ArboriaKey_GamepadLeftTrigger_3},
		{ArboriaKey_J, ArboriaKey_GamepadRightTrigger_3},
		{ArboriaKey_Return, ArboriaKey_GamepadStart},
		{ArboriaKey_Space, ArboriaKey_GamepadBack}
	};

	CVariable joyAxis_deadzone = CVariable("joyAxisDeadzone", "0.25", CVAR_FLOAT | CVAR_ENGINE, "");

	int InputManager::translateKeyCode(SDL_Scancode keycode)
	{
		switch (keycode) {
		case SDL_SCANCODE_A: return ArboriaKey_A;
		case SDL_SCANCODE_B: return ArboriaKey_B;
		case SDL_SCANCODE_C: return ArboriaKey_C;
		case SDL_SCANCODE_D: return ArboriaKey_D;
		case SDL_SCANCODE_E: return ArboriaKey_E;
		case SDL_SCANCODE_F: return ArboriaKey_F;
		case SDL_SCANCODE_G: return ArboriaKey_G;
		case SDL_SCANCODE_H: return ArboriaKey_H;
		case SDL_SCANCODE_I: return ArboriaKey_I;
		case SDL_SCANCODE_J: return ArboriaKey_J;
		case SDL_SCANCODE_K: return ArboriaKey_K;
		case SDL_SCANCODE_L: return ArboriaKey_L;
		case SDL_SCANCODE_M: return ArboriaKey_M;
		case SDL_SCANCODE_N: return ArboriaKey_N;
		case SDL_SCANCODE_O: return ArboriaKey_O;
		case SDL_SCANCODE_P: return ArboriaKey_P;
		case SDL_SCANCODE_Q: return ArboriaKey_Q;
		case SDL_SCANCODE_R: return ArboriaKey_R;
		case SDL_SCANCODE_S: return ArboriaKey_S;
		case SDL_SCANCODE_T: return ArboriaKey_T;
		case SDL_SCANCODE_U: return ArboriaKey_U;
		case SDL_SCANCODE_V: return ArboriaKey_V;
		case SDL_SCANCODE_W: return ArboriaKey_W;
		case SDL_SCANCODE_X: return ArboriaKey_X;
		case SDL_SCANCODE_Y: return ArboriaKey_Y;
		case SDL_SCANCODE_Z: return ArboriaKey_Z;
		case SDL_SCANCODE_0: return ArboriaKey_0;
		case SDL_SCANCODE_1: return ArboriaKey_1;
		case SDL_SCANCODE_2: return ArboriaKey_2;
		case SDL_SCANCODE_3: return ArboriaKey_3;
		case SDL_SCANCODE_4: return ArboriaKey_4;
		case SDL_SCANCODE_5: return ArboriaKey_5;
		case SDL_SCANCODE_6: return ArboriaKey_6;
		case SDL_SCANCODE_7: return ArboriaKey_7;
		case SDL_SCANCODE_8: return ArboriaKey_8;
		case SDL_SCANCODE_9: return ArboriaKey_9;
		case SDL_SCANCODE_RETURN: return ArboriaKey_Return;
		case SDL_SCANCODE_ESCAPE: return ArboriaKey_Escape;
		case SDL_SCANCODE_BACKSPACE: return ArboriaKey_Backspace;
		case SDL_SCANCODE_TAB: return ArboriaKey_Tab;
		case SDL_SCANCODE_MINUS: return ArboriaKey_Minus;
		case SDL_SCANCODE_EQUALS: return ArboriaKey_Equals;
		case SDL_SCANCODE_LEFTBRACKET: return ArboriaKey_LeftBracket;
		case SDL_SCANCODE_RIGHTBRACKET: return ArboriaKey_RightBracket;
		case SDL_SCANCODE_BACKSLASH: return ArboriaKey_Backslash;
		case SDL_SCANCODE_SEMICOLON: return ArboriaKey_Semicolon;
		case SDL_SCANCODE_APOSTROPHE: return ArboriaKey_Apostrophe;
		case SDL_SCANCODE_GRAVE: return ArboriaKey_Grave;
		case SDL_SCANCODE_COMMA: return ArboriaKey_Comma;
		case SDL_SCANCODE_PERIOD: return ArboriaKey_Period;
		case SDL_SCANCODE_SLASH: return ArboriaKey_Slash;
		case SDL_SCANCODE_CAPSLOCK: return ArboriaKey_CapsLock;
		case SDL_SCANCODE_F1: return ArboriaKey_F1;
		case SDL_SCANCODE_F2: return ArboriaKey_F2;
		case SDL_SCANCODE_F3: return ArboriaKey_F3;
		case SDL_SCANCODE_F4: return ArboriaKey_F4;
		case SDL_SCANCODE_F5: return ArboriaKey_F5;
		case SDL_SCANCODE_F6: return ArboriaKey_F6;
		case SDL_SCANCODE_F7: return ArboriaKey_F7;
		case SDL_SCANCODE_F8: return ArboriaKey_F8;
		case SDL_SCANCODE_F9: return ArboriaKey_F9;
		case SDL_SCANCODE_F10: return ArboriaKey_F10;
		case SDL_SCANCODE_F11: return ArboriaKey_F11;
		case SDL_SCANCODE_F12: return ArboriaKey_F12;
		case SDL_SCANCODE_F13: return ArboriaKey_F13;
		case SDL_SCANCODE_F14: return ArboriaKey_F14;
		case SDL_SCANCODE_F15: return ArboriaKey_F15;
		case SDL_SCANCODE_F16: return ArboriaKey_F16;
		case SDL_SCANCODE_F17: return ArboriaKey_F17;
		case SDL_SCANCODE_F18: return ArboriaKey_F18;
		case SDL_SCANCODE_F19: return ArboriaKey_F19;
		case SDL_SCANCODE_F20: return ArboriaKey_F20;
		case SDL_SCANCODE_F21: return ArboriaKey_F21;
		case SDL_SCANCODE_F22: return ArboriaKey_F22;
		case SDL_SCANCODE_F23: return ArboriaKey_F23;
		case SDL_SCANCODE_F24: return ArboriaKey_F24;
		case SDL_SCANCODE_PRINTSCREEN: return ArboriaKey_PrintScreen;
		case SDL_SCANCODE_SCROLLLOCK: return ArboriaKey_ScrollLock;
		case SDL_SCANCODE_PAUSE: return ArboriaKey_Pause;
		case SDL_SCANCODE_INSERT: return ArboriaKey_Insert;
		case SDL_SCANCODE_HOME: return ArboriaKey_Home;
		case SDL_SCANCODE_PAGEUP: return ArboriaKey_PageUp;
		case SDL_SCANCODE_PAGEDOWN: return ArboriaKey_PageDown;
		case SDL_SCANCODE_DELETE: return ArboriaKey_Delete;
		case SDL_SCANCODE_END: return ArboriaKey_End;
		case SDL_SCANCODE_LEFT: return ArboriaKey_LeftArrow;
		case SDL_SCANCODE_RIGHT: return ArboriaKey_RightArrow;
		case SDL_SCANCODE_UP: return ArboriaKey_UpArrow;
		case SDL_SCANCODE_DOWN: return ArboriaKey_DownArrow;
		case SDL_SCANCODE_NUMLOCKCLEAR: return ArboriaKey_NumLock;
		case SDL_SCANCODE_KP_DIVIDE: return ArboriaKey_KP_Divide;
		case SDL_SCANCODE_KP_MULTIPLY: return ArboriaKey_KP_Multiply;
		case SDL_SCANCODE_KP_MINUS: return ArboriaKey_KP_Minus;
		case SDL_SCANCODE_KP_PLUS: return ArboriaKey_KP_Plus;
		case SDL_SCANCODE_KP_ENTER: return ArboriaKey_KP_Enter;
		case SDL_SCANCODE_KP_0: return ArboriaKey_KP_0;
		case SDL_SCANCODE_KP_1: return ArboriaKey_KP_1;
		case SDL_SCANCODE_KP_2: return ArboriaKey_KP_2;
		case SDL_SCANCODE_KP_3: return ArboriaKey_KP_3;
		case SDL_SCANCODE_KP_4: return ArboriaKey_KP_4;
		case SDL_SCANCODE_KP_5: return ArboriaKey_KP_5;
		case SDL_SCANCODE_KP_6: return ArboriaKey_KP_6;
		case SDL_SCANCODE_KP_7: return ArboriaKey_KP_7;
		case SDL_SCANCODE_KP_8: return ArboriaKey_KP_8;
		case SDL_SCANCODE_KP_9: return ArboriaKey_KP_9;
		case SDL_SCANCODE_KP_PERIOD: return ArboriaKey_KP_Period;
		case SDL_SCANCODE_KP_EQUALS: return ArboriaKey_KP_Equal;
		case SDL_SCANCODE_APPLICATION: return ArboriaKey_Menu;
		case SDL_SCANCODE_LCTRL: return ArboriaKey_LCtrl;
		case SDL_SCANCODE_LSHIFT: return ArboriaKey_LShift;
		case SDL_SCANCODE_LALT: return ArboriaKey_LAlt;
		case SDL_SCANCODE_LGUI: return ArboriaKey_LGui;
		case SDL_SCANCODE_RCTRL: return ArboriaKey_RCtrl;
		case SDL_SCANCODE_RSHIFT: return ArboriaKey_RShift;
		case SDL_SCANCODE_RALT: return ArboriaKey_RAlt;
		case SDL_SCANCODE_RGUI: return ArboriaKey_RGui;
		default: return ArboriaKey_Unknown;
		}
		return 0;
	}

	int InputManager::translateGamepadCode(SDL_GameControllerButton button)
	{
		switch (button) {
		case SDL_CONTROLLER_BUTTON_A: return ArboriaKey_GamepadFaceDown;
		case SDL_CONTROLLER_BUTTON_B: return ArboriaKey_GamepadFaceRight;
		case SDL_CONTROLLER_BUTTON_X: return ArboriaKey_GamepadFaceLeft;
		case SDL_CONTROLLER_BUTTON_Y: return ArboriaKey_GamepadFaceUp;
		case SDL_CONTROLLER_BUTTON_BACK: return ArboriaKey_GamepadBack;
		case SDL_CONTROLLER_BUTTON_START: return ArboriaKey_GamepadStart;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK: return ArboriaKey_GamepadLeftTrigger_3;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return ArboriaKey_GamepadRightTrigger_3;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return ArboriaKey_GamepadLeftTrigger_1;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return ArboriaKey_GamepadRightTrigger_1;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: return ArboriaKey_GamepadDpadUp;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return ArboriaKey_GamepadDpadDown;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return ArboriaKey_GamepadDpadLeft;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return ArboriaKey_GamepadDpadRight;
		default: return ArboriaKey_Unknown;
		}
		return 0;
	}

	int InputManager::translateGamepadAxis(SDL_GameControllerAxis axis)
	{
		switch (axis) {
		case SDL_CONTROLLER_AXIS_LEFTX: return AXIS_LEFT_X;
		case SDL_CONTROLLER_AXIS_LEFTY: return AXIS_LEFT_Y;
		case SDL_CONTROLLER_AXIS_RIGHTX: return AXIS_RIGHT_X;
		case SDL_CONTROLLER_AXIS_RIGHTY: return AXIS_RIGHT_Y;
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return AXIS_LEFT_TRIGGER;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return AXIS_RIGHT_TRIGGER;
		}
		return 0;
	}

	InputManager::InputManager() {
		head = 0;
		tail = 0;
		configFlags = 7;
		deltaTime = 1.0f / 60;
		//inputConfiguration = new InputConfiguration();
		//inputConfiguration->initialize();
		//read in configuration file
	}

	InputManager::~InputManager() {
		clearQueue();
		memset(keyStates, 0, sizeof(keyStates));
	}

	void InputManager::translateSdlEvents() {
		SDL_Event _se;
		AEvent e = {};

		while (SDL_PollEvent(&_se)) {
			switch (_se.type) {
				case SDL_QUIT:
					_isQuit = true;
					break;
				case SDL_WINDOWEVENT:
					if (_se.window.event == SDL_WINDOWEVENT_CLOSE) {
						e.eventType = EventType::EVENT_WINDOW_CLOSE;
						this->submitEvent(&e);
					}
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					e.eventType = EventType::EVENT_KEY;
					e.eventValue = translateKeyCode(_se.key.keysym.scancode);
					e.eventValue2 = _se.type == SDL_KEYDOWN;
					submitEvent(&e);
					break;
				case SDL_CONTROLLERAXISMOTION:
				{
					const int range = 16384;

					int inputAxis = translateGamepadAxis((SDL_GameControllerAxis)_se.caxis.axis);
					e.eventType = EventType::EVENT_CONTROLLER_AXIS;
					e.eventValue = inputAxis;
					e.eventValue2 = _se.caxis.value;
					if (inputAxis == AXIS_LEFT_X) {
						pushAxisButton(ArboriaKey_GamepadLStickLeft, (_se.caxis.value < -range));
						pushAxisButton(ArboriaKey_GamepadLStickRight, (_se.caxis.value > range));
					}
					else if (inputAxis == AXIS_LEFT_Y) {
						pushAxisButton(ArboriaKey_GamepadLStickUp, (_se.caxis.value < -range));
						pushAxisButton(ArboriaKey_GamepadLStickDown, (_se.caxis.value > range));
					}
					else if (inputAxis == AXIS_RIGHT_X) {
						pushAxisButton(ArboriaKey_GamepadRStickLeft, (_se.caxis.value < -range));
						pushAxisButton(ArboriaKey_GamepadRStickRight, (_se.caxis.value > range));
					}
					else if (inputAxis == AXIS_RIGHT_Y) {
						pushAxisButton(ArboriaKey_GamepadRStickUp, (_se.caxis.value < -range));
						pushAxisButton(ArboriaKey_GamepadRStickDown, (_se.caxis.value > range));
					}
					else if (inputAxis == AXIS_LEFT_TRIGGER) {
						pushAxisButton(ArboriaKey_GamepadLeftTrigger_2, (_se.caxis.value > range));
					}
					else if (inputAxis == AXIS_RIGHT_TRIGGER) {
						pushAxisButton(ArboriaKey_GamepadRightTrigger_2, (_se.caxis.value > range));
					}

					//handle deadzone
					if (inputAxis >= AXIS_LEFT_X && inputAxis < MAX_TRIGGER_AXIS) {
						//get cvar deadzone's float

						float dz = joyAxis_deadzone.getFloat();
						float val = fabsf(_se.caxis.value * (1.0f / 32767.0f));

						if (val < dz)
							val = 0.0f;
						else {
							val -= dz;

							val *= val * (1.0f / (1.0f - dz));

							if (_se.caxis.value < 0)
								val = -val;
						}

						e.eventValue2 = val;
					}
					this->submitEvent(&e);
					continue;
				}
				break;
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					e.eventType = EventType::EVENT_CONTROLLER_BUTTON;
					e.eventValue = translateGamepadCode((SDL_GameControllerButton)_se.cbutton.button);
					e.eventValue2 = _se.type == SDL_CONTROLLERBUTTONDOWN;
					this->submitEvent(&e);
					break;
				case SDL_MOUSEMOTION:
					e.eventType = EventType::EVENT_MOUSE_MOVE;
					e.eventValue = _se.motion.x;
					e.eventValue2 = _se.motion.y;
					this->submitEvent(&e);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					e.eventType = EventType::EVENT_KEY;
					switch (_se.button.button) {
					case SDL_BUTTON_LEFT:
						e.eventValue = ArboriaKey_MouseLeft;
						break;
					case SDL_BUTTON_RIGHT:
						e.eventValue = ArboriaKey_MouseRight;
						break;
					case SDL_BUTTON_MIDDLE:
						e.eventValue = ArboriaKey_MouseMiddle;
						break;
					default:
						break;
					}
					e.eventValue2 = _se.type == SDL_MOUSEBUTTONDOWN;
					break;
				default:
					break;
			}
		}
	}

	void InputManager::submitEvent(const AEvent* ev) {
		//if head - tail >= MAX_EVENT_COUNT, yield
		queue[tail] = *ev;
		tail = (tail + 1) & (MAX_EVENT_COUNT - 1); //basically a modulo
	}

	const AEvent& InputManager::getFront() const {
		return queue[head];
	}

	AEvent& InputManager::getFront() {
		return queue[head];
	}

	void InputManager::popFront() {
		if (head == tail) return;
		head = (head + 1) & (MAX_EVENT_COUNT - 1);
	}

	void InputManager::clearQueue()
	{
		memset(queue, 0, sizeof(queue));
		head = 0;
		tail = 0;
	}
	bool InputManager::isGamepadKey(int scancode)
	{
		return false;
	}
	int InputManager::getRepeatCount(int key, float repeat_delay, float repeat_rate)
	{
		KeyState keyState = keyStates[key];
		float prevTime = keyState.downDuration - (deltaTime/1e6);
		float nowTime = keyState.downDuration;

		if (prevTime == 0.0f) return 1;
		if (prevTime >= nowTime) return 0;
		if (repeat_rate <= 0) return (nowTime < repeat_delay) && (prevTime >= repeat_delay);

		const int count_prev = (prevTime < repeat_delay) ? -1 : (int)((prevTime - repeat_delay) / repeat_rate);
		const int count_now = (nowTime < repeat_delay) ? -1 : (int)((nowTime - repeat_delay) / repeat_rate);
		return count_prev - count_now;
	}
	void InputManager::updateKeyState(int keyCode, bool isDown)
	{
		if (keyCode <= ArboriaKey_Unknown) {
			//print warning
			return;
		}

		KeyState* keyData = getKeyState(keyCode);
		if (isDown && keyData->down) { //if key is already down
			//we need to take into account the repeat delay/rate, and if it's surpassed it, we simply update the durations
			const float t = keyData->downDuration;
			bool isPressed = isDown;
			if (t == 0.0f) {
				gLastKeyboardPressTime = gCurrentTime;
				int repeatCount = getRepeatCount(keyCode, 0.725f, 0.05f);
				if (t > 0.725f && repeatCount > 0) {
					double pressed_time = gCurrentTime / 1e6 - t + 0.00001f;
					if (gLastKeyboardPressTime <= pressed_time) {
						isPressed = false;
					}
				}
			}
			if (isPressed) {
				keyData->lastDownDuration = keyData->downDuration;
				keyData->downDuration += deltaTime;
				keyData->analog = 1.0f;
			}

		}
		else if (isDown && !keyData->down) {
			keyData->down = true;
			keyData->lastDownDuration = keyData->downDuration;
			keyData->downDuration = 0.0f;
			keyData->analog = 1.0f;
		}
		else if (!isDown && keyData->down) {
			keyData->down = false;
			keyData->lastDownDuration = keyData->downDuration;
			keyData->downDuration = -1.0f;
			keyData->analog = 0.0f;
		}
	}

	void InputManager::unbindKeyAction(int keyNum, int action)
	{
		if (keyNum <= ArboriaKey_Unknown || keyNum >= ArboriaKey_GamepadStart) {
			Engine::printError("InputManager::unbindKeyAction failed. keyNum input was not a key");
			return;
		}
		if (action <= ACTION_NONE || action > ACTION_SELECT) {
			Engine::printError("InputManager::unbindKeyAction failed. action input was invalid");
			return;
		}
		keyStates[keyNum].binding = ArboriaKey_Unknown;
	}

	void InputManager::unbindKeyAction(int action)
	{
		for (int i = 0; i < ArboriaKey_GamepadStart; i++) {
			if (keyStates[i].binding == action) {
				keyStates[i].binding = ACTION_NONE;
				return;
			}
		}
	}

	void InputManager::unbindGamepadAction(int buttonNum, int action)
	{
		if (buttonNum < ArboriaKey_GamepadStart || buttonNum > ArboriaKey_GamepadRStickDown) {
			Engine::printError("InputManager::unbindGamepadAction: requested button number was not a proper gamepad button");
			return;
		}
		if (action <= ACTION_NONE || action > ACTION_SELECT) {
			Engine::printError("InputManager::unbindGamepadAction failed. action input was invalid");
			return;
		}
		keyStates[buttonNum] = ArboriaKey_Unknown;
	}

	void InputManager::unbindGamepadAction(int action)
	{
		for (int i = ArboriaKey_GamepadStart; i < ArboriaKey_MouseLeft; i++) {
			if (keyStates[i].binding == action) {
				keyStates[i].binding = ACTION_NONE;
				return;
			}
		}
	}

	void InputManager::bindAction(int keynum, int action)
	{
		keyStates[keynum].binding = action;
	}

	int InputManager::getKeyFromAction(int action)
	{
		for (int i = ArboriaKey_A; i < ArboriaKey_GamepadStart; i++) {
			if (keyStates[i].binding == action)
				return i;
		}
		return -1;
	}

	int InputManager::getGamepadButtonFromAction(int action) {
		for (int i = ArboriaKey_GamepadStart; i < ArboriaKey_MouseLeft; i++) {
			if (keyStates[i].binding == action)
				return i;
		}
		return -1;
	}

	KeyState* InputManager::getKeyInputFromAction(int action)
	{
		for (int i = ArboriaKey_A; i < ArboriaKey_GamepadStart; i++) {
			if (keyStates[i].binding == action)
				return &keyStates[i];
		}
		return NULL;
	}

	KeyState* InputManager::getButtonInputFromAction(int action) {
		for (int i = ArboriaKey_GamepadStart; i < ArboriaKey_MouseLeft; i++) {
			if (keyStates[i].binding == action)
				return &keyStates[i];
		}
		return NULL;
	}

	void InputManager::pushAxisButton(int key, bool value)
	{
		if (keyStates[key].down != value) {
			AEvent res = { EventType::EVENT_CONTROLLER_BUTTON, key, value ? 1 : 0, NULL };
			submitEvent(&res);
		}
	}

	void InputManager::restoreDefaults() {
		for (int act = ACTION_CONFIRM; act < ACTION_SELECT + 1; act++) {
			int key = getKeyFromAction(act);
			if (key > -1)
				unbindKeyAction(key, act);
			bindAction(defaultBindings[act].key, act);
			int button = getGamepadButtonFromAction(act);
			if (button > -1)
				unbindGamepadAction(button, act);
			bindAction(defaultBindings[act].button, act);
		}
	}

	void InputManager::saveUserBindings() {
		PHYSFS_File* outFile = PHYSFS_openWrite("inputs.ini");
		for (int act = ACTION_CONFIRM; act < ACTION_SELECT + 1; act++) {
			int key = getKeyFromAction(act);
			int button = getGamepadButtonFromAction(act);
			String bindString = convertToString(act, key, button);
			if (act == ACTION_SELECT) {
				bindString[bindString.length() - 1] = '\0';
			}
			PHYSFS_writeBytes(outFile, (void*)bindString.c_str(), bindString.length());
		}
		PHYSFS_close(outFile);
	}

	bool InputManager::loadUserBindings() {
		void* fileBuffer;
		int fLength = readFileFromPhysFS("inputs.ini", &fileBuffer);

		int action = ACTION_NONE;
		int key = ArboriaKey_Unknown;
		int button = ArboriaKey_Unknown;

		int boundKey = ArboriaKey_Unknown;
		int boundButton = ArboriaKey_Unknown;

		Lexer src;
		Token t;
		src.loadMemory((char*)fileBuffer, fLength, 0);
		do {
			if (!src.readToken(&t))
				return false;
			if (t == "Action") {
				if (!parseActionBindings(src, action, key, button))
					return false;
				else {
					boundKey = getKeyFromAction(action);
					if (boundKey != key) {
						if (boundKey == -1) {
							bindAction(key, action);
						}
						else {
							unbindKeyAction(boundKey, action);
							bindAction(key, action);
						}
					}
					boundButton = getGamepadButtonFromAction(action);
					if (boundButton != button) {
						if (boundButton == -1) {
							bindAction(button, action);
						}
						else {
							unbindGamepadAction(boundButton, action);
							bindAction(button, action);
						}
					}
				}
			}
		} while (1);

		return true;
	}

	bool InputManager::parseActionBindings(Lexer& src, int& act, int& key, int& button) {
		Token tok;

		if (!src.expectTokenString("=")) return false;
		if (!src.expectTokenString("(")) return false;

		while (src.readToken(&tok)) {
			if (tok == ")") return true;
			//src.unreadToken(&tok);
			if (tok == "action") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				act = tok.getIntegerValue();
				if (act <= ACTION_NONE || act > ACTION_SELECT) {
					Engine::printError("Error parsing an action binding: action value is not a valid action");
					return false;
				}
			}
			if (tok == "key") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				key = tok.getIntegerValue();
			}
			if (tok == "button") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &tok);
				button = tok.getIntegerValue();
			}
		}
		return true;
	}

	String& InputManager::convertToString(const int& act, const int& key, const int& button)
	{
		String bindString = "Action=(action=";
		bindString = bindString + act + " key=" + key + " button=" + button + ")\n";
		return bindString;
	}
}