#ifndef COLOR_H
#define COLOR_H

#include "SDL2/SDL.h"

namespace Arboria {

	class Color {
		public:
			uint8_t r, g, b, a;
			Color();
			Color(uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a = 255);
			operator SDL_Color() const;
			bool operator ==(const Color& other);
			bool operator !=(const Color& other);
			uint32_t encodeRGBA();
			void decodeRGBA(const uint32_t encodedColor);
	};
}

#endif