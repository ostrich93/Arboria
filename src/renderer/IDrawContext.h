#ifndef IDRAWCONTEXT_H
#define IDRAWCONTEXT_H

#include "Drawing.h"
#include "../utils/Color.h"

namespace Arboria {
	struct IDrawEngine;

	struct IDrawContext {
		virtual ~IDrawContext() = default;

		virtual void drawLine(RenderTarget& target, Color color) = 0;
		virtual void drawRectangle(RenderTarget& target, Color color, int32_t left, int32_t right, int32_t top, int32_t bottom) = 0;
		virtual void drawText() = 0;
	};
}

#endif
