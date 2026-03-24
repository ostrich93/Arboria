#ifndef DRAW_SPRITE_COMMAND_H
#define DRAW_SPRITE_COMMAND_H

#include <GL/glew.h>
#include "../utils/Color.h"

namespace Arboria {
	struct DrawSpriteCommand {
		GLfloat texPosition[2];
		GLfloat spriteSize[2];
		GLfloat screenPosition[2];
		GLfloat displaySize[2]; //size of the object being rendered
		int32_t page;
		int32_t uses_palette;
		Color tint;
	};
}

#endif