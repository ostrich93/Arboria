#include "InputManager.h"
#include "../Heap.h"

namespace Arboria {
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
					e.inputDeviceType = InputDeviceType::INPUT_DEVICE_KEYBOARD;
					e.keyboardEvent.keyCode = translateKeyCode(_se.key.keysym.scancode);
					e.keyboardEvent.mods = SDL_GetModState();
					e.eventType = EventType::EVENT_KEY_DOWN;
					submitEvent(&e);
					break;
				case SDL_KEYUP:
					if (_se.key.keysym.scancode < 128) {
						e.inputDeviceType = InputDeviceType::INPUT_DEVICE_KEYBOARD;
						e.keyboardEvent.scancode = _se.key.keysym.scancode;
						e.keyboardEvent.keyCode = _se.key.keysym.sym;
						e.keyboardEvent.mods = SDL_GetModState();
						e.keyboardEvent.keyState = _se.key.state == SDL_PRESSED;
						e.eventType = EventType::EVENT_KEY_PRESS;
						this->submitEvent(&e);
					}
					break;
				case SDL_JOYAXISMOTION:
					e.inputDeviceType = InputDeviceType::INPUT_DEVICE_CONTROLLER_AXIS;
					e.controllerEvent.joystickId = _se.jaxis.which;
					e.controllerEvent.axisValues[_se.jaxis.axis] = _se.jaxis.value;
					e.controllerEvent.button = _se.jaxis.value;
					e.controllerEvent.buttonState = _se.jaxis.value != 0 ? 1 : 0;
					this->submitEvent(&e);
					break;
				case SDL_JOYBUTTONUP:
					e.inputDeviceType = InputDeviceType::INPUT_DEVICE_CONTROLLER_BUTTON;
					e.controllerEvent.joystickId = _se.jbutton.which;
					e.controllerEvent.button = _se.jbutton.button;
					e.controllerEvent.buttonState = _se.jbutton.state == SDL_PRESSED;
					this->submitEvent(&e);
					break;
				case SDL_CONTROLLERBUTTONUP:
					e.inputDeviceType = InputDeviceType::INPUT_DEVICE_CONTROLLER_BUTTON;
					e.controllerEvent.joystickId = _se.cbutton.which;
					e.controllerEvent.button = _se.cbutton.button;
					e.controllerEvent.buttonState = _se.cbutton.state == SDL_PRESSED;
					e.eventType = EventType::EVENT_BUTTON_UP;
					this->submitEvent(&e);
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
}