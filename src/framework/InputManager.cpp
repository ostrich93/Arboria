#include "InputManager.h"
#include <SDL.h>
#include "../Heap.h"

namespace Arboria {
	InputManager::InputManager() {
		head = 0;
		tail = 0;
		mouseEventFactory = new MouseEventFactory();
		keyboardEventFactory = new KeyboardEventFactory();
		controllerEventFactory = new ControllerEventFactory();
		inputConfiguration = new InputConfiguration();
		inputConfiguration->initialize();
		//read in configuration file
	}

	void InputManager::translateSdlEvents() {
		SDL_Event _se;
		Event* e;

		while (SDL_PollEvent(&_se)) {
			switch (_se.type) {
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					if (mouseEventFactory == NULL) {
						mouseEventFactory = new MouseEventFactory();
					}
					e = mouseEventFactory->generateEvent(_se);
					this->submitEvent(e);
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if (keyboardEventFactory == NULL) {
						keyboardEventFactory = new KeyboardEventFactory();
					}
					e = keyboardEventFactory->generateEvent(_se);
					this->submitEvent(e);
					break;
				case SDL_JOYAXISMOTION:
				case SDL_JOYHATMOTION:
				case SDL_JOYBALLMOTION:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					if (controllerEventFactory == NULL) {
						controllerEventFactory = new ControllerEventFactory();
					}
					e = controllerEventFactory->generateEvent(_se);
					this->submitEvent(e);
					break;
				case SDL_WINDOWEVENT:
					if (_se.window.event == SDL_WINDOWEVENT_CLOSE) {
						_se.type = SDL_QUIT;
						SDL_PushEvent(&_se);
					}
					break;
				default:
					break;
			}
		}
	}

	void InputManager::submitEvent(const Event* ev) {
		//if head - tail >= MAX_EVENT_COUNT, yield
		queue[tail] = *ev;
		tail = (tail + 1) & (MAX_EVENT_COUNT - 1); //basically a modulo
	}

	const Event& InputManager::getFront() const {
		return queue[head];
	}

	Event& InputManager::getFront() {
		return queue[head];
	}

	void InputManager::popFront() {
		if (head == tail) return;
		head = (head + 1) & (MAX_EVENT_COUNT - 1);
	}
}