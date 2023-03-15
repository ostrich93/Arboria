#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

#include "../utils/Vector.h"
#include "FontManager.h"
#include "ShaderProgram.h"
#include <SDL_opengles2.h>

namespace Arboria {
	struct TextVertexDesc {
		Vector2<float> screenPosition;
		Vector2<float> texCoord;
		Vector2<float> texSize;
		Vector3<float> tint;
	};

	class TextRenderer {
		private:
			ShaderProgram textShader;

			unsigned int vao_id;
			unsigned int vbo_id;
			unsigned int ibo_id;
			GLint textureAtlasLocation;
			GLint pColorLocation;

		public:
			TextRenderer();
			~TextRenderer();
			void draw(Font* ft, const Vector2<int>* pos, Vector3<float> color, String& text);
	};
}

#endif