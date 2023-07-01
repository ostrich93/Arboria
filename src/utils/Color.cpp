#include "Color.h"

namespace Arboria {
	Color::Color(): r(0), g(0), b(0), a(255){}
	Color::Color(uint8_t _r, uint8_t _b, uint8_t _g, uint8_t _a) : r(_r), b(_b), g(_g), a(_a) {}
	Color::operator SDL_Color() const {
		SDL_Color color;
		color.r = r;
		color.b = b;
		color.g = g;
		color.a = a;
		return color;
	}
	bool Color::operator==(const Color& other) {
		return r == other.r && b == other.b && g == other.g && a == other.a;
	}
	bool Color::operator!=(const Color& other) {
		return !((*this) == other);
	}

	uint32_t Color::encodeRGBA() {
		uint32_t result = static_cast<uint32_t>(a);
		result |= static_cast<uint32_t>(r) << 24;
		result |= static_cast<uint32_t>(g) << 16;
		result |= static_cast<uint32_t>(b) << 8;

		return result;
	}

	void Color::decodeRGBA(const uint32_t encodedColor) {
		a = static_cast<uint8_t>(encodedColor);
		r = static_cast<uint8_t>(encodedColor >> 24);
		g = static_cast<uint8_t>(encodedColor >> 16);
		b = static_cast<uint8_t>(encodedColor >> 8);
	}
}