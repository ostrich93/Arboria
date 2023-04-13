#include "Event.h"

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

}