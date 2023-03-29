#ifndef __EVENT_H__
#define __EVENT_H__

#include <SDL.h>
#include "../gui/WidgetEnums.h"

namespace Arboria {
	class Widget;
	enum EventType {
		EVENT_UNDEFINED = -1,
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
		EVENT_TIMER
	};

	typedef struct KeyboardEventData {
		int keyCode;
		int scanCode;
		unsigned int mods;
	} KeyboardEventData;

	/*typedef struct MouseEventData {
		int x;
		int y;
		int deltaX;
		int deltaY;
		int button;
	} MouseEventData;

	typedef struct ControllerEventData {
		int id;
		int axis;
		int stick;
		int hat;
		int position;
		int button;
	} ControllerEventData;*/

	typedef struct GuiEventData {
		Widget* raisedBy;
		EventType parentEventType;
		WidgetEventType guiEventType;
		KeyboardEventData keyboardData;
		//MouseEventData mouseData;
		//ControllerEventData controllerData;
	} GuiEventData;

	class Event {
		protected:
			Event(EventType _eventType) : eventType(_eventType), isHandled(false) {}
			EventType eventType;
		public:
			Event() : eventType(EventType::EVENT_UNDEFINED), isHandled(false) {}
			EventType getEventType() { return eventType; }
			const EventType getEventType() const { return eventType; }
			bool isHandled;
			virtual ~Event() = default;
	};

	class KeyboardEvent : public Event {
		private:
			KeyboardEventData data;
			friend class Event;
		public:
			KeyboardEvent(EventType _eventType, KeyboardEventData& _data) : Event(_eventType), data(_data) {}
			KeyboardEventData getData() { return data; }
			~KeyboardEvent() override = default;
	};

	class WidgetEvent : public Event {
		private:
			GuiEventData data;
			friend class Event;
		public:
			WidgetEvent() : Event(EVENT_UI_INTERACTION) {}
			void setGuiEventType(WidgetEventType _type) { data.guiEventType = _type; }
			void setRaisedBy(Widget& widget) { data.raisedBy = &widget; }
			void setParentEventType(EventType eType) { data.parentEventType = eType; }
			GuiEventData getData() { return data; }
			~WidgetEvent() override = default;
	};

	class EventFactory {
		public:
			virtual Event* generateEvent(SDL_Event sdl_event) = 0;
	};

	class KeyboardEventFactory : public EventFactory {
		private:
			KeyboardEventData generateData(SDL_Event _sdlEvent);
		public:
			Event* generateEvent(SDL_Event sdl_event) override;
	};
}

#endif