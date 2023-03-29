#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "Renderer.h"
#include "TextRenderer.h"

namespace Arboria {
	class RenderSystem {
		private:
			TextRenderer* textRenderer;
			SpriteRenderer* spriteRenderer;
	};
}

#endif