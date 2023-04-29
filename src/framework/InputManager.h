#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Engine.h"
#include "InputBindings.h"
#include "../events/Event.h"

const int MAX_EVENT_COUNT = 256;

namespace Arboria {

	struct KeyState {
		bool down;
		bool up;
		int repeats;
		unsigned short int scancode;
		InputActionType bindValue;
		KeyState(bool _down = false, int _repeats = 0, unsigned short int _scancode = 0, InputActionType _bValue = InputActionType::INVALID)
		: down(_down), up(false), repeats(_repeats), scancode(_scancode), bindValue(_bValue){}
	};

	class InputManager {
		private:
			KeyboardEventFactory* keyboardEventFactory;
			InputConfiguration* inputConfiguration;
			Event queue[256];
			unsigned int head;
			unsigned int tail;
		protected:
			InputActionType keyBindings[128]; //the index of each array represents a scancode, with unbound codes having a value of -1.
			KeyState keyStates[InputActionType::ACTION_COUNT];
			KeyState* lastKeyState;
		public:
			InputManager();
			~InputManager();
			void translateSdlEvents();
			void submitEvent(const Event* ev);
			const Event& getFront() const;
			Event& getFront();
			void popFront();
			bool isQueueEmpty() const { return head == tail; }
			void clearQueue();
			void updateKeyInputState(KeyboardEvent* _keyEv);
			InputActionType getActionTranslation(KeyboardEventData* keyboardEventData);
			InputActionType getActionTranslation(KeyboardEvent* _keyEv);
			InputConfiguration& getInputConfiguration() { return *inputConfiguration; }
			const InputConfiguration& getInputConfiguration() const { return *inputConfiguration; }
			int lastAction; //this, along with the two below arrays, are used in the scene objects
			bool isPressed[InputActionType::ACTION_COUNT];
			bool isLocked[InputActionType::ACTION_COUNT];
	};
}

#endif