#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "ShaderProgram.h"

namespace Arboria {
	class Font;
	struct TextVertexDesc {
		Vector2<float> screenPosition;
		Vector2<float> texCoord;
		Vector2<float> texSize;
		Vector4<float> tint;
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
			void draw(Font* ft, const Vector2<int>* pos, Vector4<float> color, String& text);
	};
}

#endif