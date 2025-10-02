#ifndef DRAWING_H
#define DRAWING_H

#include "../utils/Vector.h"
#include <cstdint>

namespace Arboria {
	struct RenderTarget {
		uint8_t* bits;
		int32_t x;
		int32_t y;
		int32_t pitch;
		int32_t cullingX;
		int32_t cullingY;
		int32_t cullingWidth;
		int32_t cullingHeight;

		Vector2<int32_t> lastTextPosition;
	};
}
#endif