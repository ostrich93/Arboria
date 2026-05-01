#ifndef EVENT_H
#define EVENT_H

#include "../framework/Engine.h"

namespace Arboria {
	enum WidgetEventType;

	class Widget;
	enum class EventType : uint32_t {
		EVENT_UNDEFINED,
		EVENT_KEY, //evValue = scancode, evValue2 = down flag, include gamepad BUTTONS?
		EVENT_CONTROLLER_BUTTON, //evValue = scancode, evValue2 = down flag, include gamepad BUTTONS?
		EVENT_CONTROLLER_AXIS, //evValue = axis number, evValue2 = current status (-127 to 127), control stick/R2/L2 buttons fsr
		EVENT_MOUSE_DOWN, //evValue and evValue2 = relative signed x/y coords
		EVENT_MOUSE_UP,
		EVENT_MOUSE_MOVE, //evValue/2 = relative signed x/y coords
		EVENT_MOUSE_MOVE_ABS, //evValue/2 = absolute x/y coords
		EVENT_WINDOW_ACTIVATE,
		EVENT_WINDOW_DEACTIVATE,
		EVENT_WINDOW_RESIZE,
		EVENT_WINDOW_CLOSE,
		EVENT_UI_INTERACTION, //evPtr = guiEventInfo
		EVENT_TEXT_INPUT,
		EVENT_TIMER,
		EVENT_MAX_VALUE = 0x7fffffff
	};

	class AEvent {
		public:

			struct DisplayEvent {
				int width = 0;
				int height = 0;
				bool active = false;
			};

			typedef enum JoystickAxis {
				AXIS_LEFT_X = 0,
				AXIS_LEFT_Y,
				AXIS_RIGHT_X,
				AXIS_RIGHT_Y,
				AXIS_LEFT_TRIG,
				AXIS_RIGHT_TRIG,
				AXIS_COUNT
			};

			EventType eventType;
			int eventValue; //key inputs
			int eventValue2;
			void* eventPtr;
			bool isHandled;

			~AEvent();
	};

	typedef void (*eventCallback)(AEvent*, void*);
}

#endif