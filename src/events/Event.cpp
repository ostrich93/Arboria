#include "Event.h"
#include <SDL_mouse.h>

namespace Arboria {

	KeyboardEventData KeyboardEventFactory::generateData(SDL_Event _sdlEvent) {
		KeyboardEventData keyData;
		keyData.keyCode = _sdlEvent.key.keysym.sym;
		keyData.scanCode = _sdlEvent.key.keysym.scancode;
		keyData.mods = _sdlEvent.key.keysym.mod;

		return keyData;
	}

	Event* KeyboardEventFactory::generateEvent(SDL_Event _sdl_event) {
		KeyboardEventData _data = generateData(_sdl_event);
		EventType _etype = _sdl_event.key.state == SDL_PRESSED ? EVENT_KEY_DOWN : EVENT_KEY_UP;
		return new KeyboardEvent(_etype, _data);
	}

	MouseEventData MouseEventFactory::generateData(SDL_Event _sdlEvent) {
		MouseEventData mouseData;
		switch (_sdlEvent.type) {
			case SDL_MOUSEMOTION:
				//mouseData.x = coordWindowToDisplayX(_sdlEvent.motion.x);
				mouseData.x = _sdlEvent.motion.x;
				//mouseData.y = coordWindowToDisplayY(_sdlEvent.motion.y);
				mouseData.y = _sdlEvent.motion.y;
				mouseData.deltaX = _sdlEvent.motion.xrel;
				mouseData.deltaY = _sdlEvent.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				//mouseData.x = coordWindowToDisplayX(_sdlEvent.button.x);
				mouseData.x = _sdlEvent.button.x;
				//mouseData.y = coordWindowToDisplayY(_sdlEvent.button.y);
				mouseData.y = _sdlEvent.button.y;
				mouseData.deltaX = 0;
				mouseData.deltaY = 0;
				mouseData.button = SDL_BUTTON(_sdlEvent.button.button);
				break;
			default:
				break;
		}
		return mouseData;
	}

	Event* MouseEventFactory::generateEvent(SDL_Event _sdl_event) {
		EventType _eType = EventType::EVENT_UNDEFINED;
		MouseEventData _data = generateData(_sdl_event);
		switch (_sdl_event.type) {
			case SDL_MOUSEMOTION:
				_eType = EVENT_MOUSE_MOVE;
				break;
			case SDL_MOUSEBUTTONDOWN:
				_eType = EVENT_MOUSE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				_eType = EVENT_MOUSE_UP;
				break;
			default:
				break;
		}

		return new MouseEvent(_eType, _data);
	}

}