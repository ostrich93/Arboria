#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Engine.h"
#include "../events/Event.h"

const int MAX_EVENT_COUNT = 64;

namespace Arboria {

	struct KeyState {
		bool down;
		bool up;
		int repeats;
		unsigned short int scancode;
		KeyState(bool _down = false, int _repeats = 0, unsigned short int _scancode = 0)
		: down(_down), up(false), repeats(_repeats), scancode(_scancode){}
	};

	class InputManager {
		private:
			//InputConfiguration* inputConfiguration;
			AEvent queue[64];
			unsigned int head;
			unsigned int tail;
		protected:
			//InputActionType keyBindings[128]; //the index of each array represents a scancode, with unbound codes having a value of -1.
			uint8_t keysPressed[256];
			uint32_t lastKeyPressed;
			const uint8_t* kStates;
			KeyState keyStates[128];
			KeyState* lastKeyState;
		public:
			explicit InputManager();
			~InputManager();
			void translateSdlEvents();
			void submitEvent(const AEvent* ev);
			const AEvent& getFront() const;
			AEvent& getFront();
			void popFront();
			bool isQueueEmpty() const { return head == tail; }
			void clearQueue();
			//void updateKeyInputState(AEvent* _keyEv);
	};
}

#endif