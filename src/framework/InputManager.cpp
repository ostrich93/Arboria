#include "InputManager.h"
#include "../Heap.h"

namespace Arboria {
	InputManager::InputManager() {
		head = 0;
		tail = 0;
		lastKeyState = NULL;
		//inputConfiguration = new InputConfiguration();
		//inputConfiguration->initialize();
		//read in configuration file
	}

	InputManager::~InputManager() {
		clearQueue();
		memset(keyStates, 0, sizeof(keyStates));
		Mem_Free(lastKeyState);
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
				case SDL_KEYUP:
					if (_se.key.keysym.scancode < 128) {
						e.inputDeviceType = InputDeviceType::INPUT_DEVICE_KEYBOARD;
						e.keyboardEvent.scancode = _se.key.keysym.scancode;
						e.keyboardEvent.keyCode = _se.key.keysym.sym;
						e.keyboardEvent.mods = SDL_GetModState();
						e.keyboardEvent.keyState = _se.key.state == SDL_PRESSED;
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
}