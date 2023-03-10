#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "../utils/Vector.h"

namespace Arboria {
	class Texture;
	class Renderer {
		public:
			virtual ~Renderer();
			virtual void flush() = 0;
			virtual void draw(Texture* image, Vector2<float> position) = 0;
	};
}

#endif