#ifndef SESSION_H
#define SESSION_H

#include "Engine.h"

namespace Arboria {
	class Context {
		private:
			uint8_t keysPressed[256];
			uint32_t lastKeyPressed;
			const uint8_t* keyStates;

		public:
			~Context();
			explicit Context();
			void update();
	};
}

#endif