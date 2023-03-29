#include "InputManager.h"
#include <SDL.h>
#include "../Heap.h"

namespace Arboria {
	InputManager::InputManager() {
		head = 0;
		tail = 0;
		lastKeyState = NULL;
		keyboardEventFactory = new KeyboardEventFactory();
		inputConfiguration = new InputConfiguration();
		inputConfiguration->initialize();
		//read in configuration file
	}

	InputManager::~InputManager() {
		clearQueue();
		memset(keyBindings, 0, sizeof(keyBindings));
		memset(keyStates, 0, sizeof(keyStates));
		Mem_Free(lastKeyState);
		Mem_Free(keyboardEventFactory);
	}

	void InputManager::translateSdlEvents() {
		SDL_Event _se;
		Event* e;

		while (SDL_PollEvent(&_se)) {
			switch (_se.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if (keyboardEventFactory == NULL) {
						keyboardEventFactory = new KeyboardEventFactory();
					}

					if (_se.key.keysym.scancode < 128 && keyBindings[_se.key.keysym.scancode] != -1) {
						e = keyboardEventFactory->generateEvent(_se);
						this->submitEvent(e);
						this->updateKeyInputState(dynamic_cast<KeyboardEvent*>(e));
					}
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

	void InputManager::clearQueue()
	{
		memset(queue, 0, sizeof(queue));
		head = 0;
		tail = 0;
	}

	void InputManager::updateKeyInputState(KeyboardEvent* _keyEv)
	{
		InputActionType submittedAction = keyBindings[_keyEv->getData().scanCode];
		if (_keyEv->getEventType() == EVENT_KEY_DOWN) {
			keyStates[submittedAction].down = true;
			keyStates[submittedAction].up = false;
			keyStates[submittedAction].scancode = _keyEv->getData().scanCode;

			if (lastKeyState != NULL) {
				if (lastKeyState->bindValue == submittedAction && lastKeyState->down && !lastKeyState->up) {
					keyStates[submittedAction].repeats++;
					lastKeyState->repeats = keyStates[submittedAction].repeats;
				}
			}
		}
		else if (_keyEv->getEventType() == EVENT_KEY_UP) {
			keyStates[submittedAction].down = true;
			keyStates[submittedAction].up = true;
			keyStates[submittedAction].scancode = _keyEv->getData().scanCode;
			keyStates[submittedAction].repeats = 0;
			if (lastKeyState == NULL) {
				lastKeyState = new KeyState();
			}
			lastKeyState->down = keyStates[submittedAction].down;
			lastKeyState->up = keyStates[submittedAction].up;
			lastKeyState->scancode = keyStates[submittedAction].scancode;
			lastKeyState->repeats = 0;
		}
	}

	InputActionType InputManager::getActionTranslation(KeyboardEventData* keyboardEventData)
	{
		if (keyboardEventData == NULL) return InputActionType::INVALID;

		if (keyboardEventData->scanCode > 127 && keyboardEventData->scanCode < 0) {
			return InputActionType::INVALID;
		}
		return keyBindings[keyboardEventData->scanCode];
	}

	InputActionType InputManager::getActionTranslation(KeyboardEvent* _keyEv)
	{
		if (_keyEv == NULL) return InputActionType::INVALID;

		if (_keyEv->getData().scanCode > 127 && _keyEv->getData().scanCode < 0)
			return InputActionType::INVALID;
		return keyBindings[_keyEv->getData().scanCode];
	}
}