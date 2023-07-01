#ifndef EVENT_H
#define EVENT_H

#include "../framework/Engine.h"

namespace Arboria {
	enum WidgetEventType;

	enum class InputDeviceType : uint8_t {
		INPUT_DEVICE_NONE = 0,
		INPUT_DEVICE_KEYBOARD,
		INPUT_DEVICE_CONTROLLER_BUTTON,
		INPUT_DEVICE_CONTROLLER_AXIS,
		INPUT_DEVICE_MOUSE
	};

	class Widget;
	enum class EventType : uint32_t {
		EVENT_UNDEFINED,
		EVENT_WINDOW_ACTIVATE,
		EVENT_WINDOW_DEACTIVATE,
		EVENT_WINDOW_RESIZE,
		EVENT_WINDOW_CLOSE,
		EVENT_KEY_DOWN,
		EVENT_KEY_PRESS,
		EVENT_KEY_UP,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_UP,
		EVENT_CONTROLLER_AXIS,
		EVENT_CONTROLLER_HAT,
		EVENT_BUTTON_DOWN,
		EVENT_BUTTON_UP,
		EVENT_BUTTON_PRESS,
		EVENT_UI_INTERACTION,
		EVENT_TEXT_INPUT,
		EVENT_TIMER,
		EVENT_MAX_VALUE = 0x7fffffff
	};

	enum class GUIEventType : uint32_t {
		GUI_FOCUS_LEFT = 0,
		GUI_FOCUS_OBTAINED,
		GUI_MENU_SELECT_CHANGE,
		GUI_MENU_CONFIRM,
		GUI_MENU_CANCEL
	};

	class AEvent {
		public:
			struct GUIEvent {
				Widget* raisedBy;
				GUIEventType eventType;
			};

			struct KeyInputEvent {
				unsigned short scancode;
				int32_t keyCode;
				uint16_t mods;
				bool keyState : 1;
			};

			struct JoystickEvent {
				enum JoystickAxis {
					AXIS_X = 0,
					AXIS_Y,
					AXIS_Z,
					AXIS_R,
					AXIS_U,
					AXIS_V,
					AXIS_COUNT
				};

				signed short axisValues[JoystickAxis::AXIS_COUNT];
				uint8_t joystickId;
				uint8_t button;
				bool buttonState : 1;
			};

			struct UserEvent {
				void* userData1;
				void* userData2;
			};

			InputDeviceType inputDeviceType;
			EventType eventType;
			bool isHandled;

			union {
				struct GUIEvent guiEvent;
				struct KeyInputEvent keyboardEvent;
				struct JoystickEvent controllerEvent;
				struct UserEvent userEvent;
			};
	};
}

#endif