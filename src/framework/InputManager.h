#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "../events/Event.h"
#include "InputActionType.h"
#include "InputBindings.h"

const int MAX_EVENT_COUNT = 256;

namespace Arboria {

	class InputManager {
		private:
			MouseEventFactory* mouseEventFactory;
			KeyboardEventFactory* keyboardEventFactory;
			ControllerEventFactory* controllerEventFactory;
			InputConfiguration* inputConfiguration;
			Event queue[256];
			unsigned int head;
			unsigned int tail;
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
			InputConfiguration& getInputConfiguration() { return *inputConfiguration; }
			const InputConfiguration& getInputConfiguration() const { return *inputConfiguration; }
			int lastAction; //this, along with the two below arrays, are used in the scene objects
			bool isPressed[InputActionType::ACTION_COUNT];
			bool isLocked[InputActionType::ACTION_COUNT];
	};
}

#endif