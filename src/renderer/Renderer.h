#ifndef RENDERER_H
#define RENDERER_H

#include "ShaderProgram.h"

namespace Arboria {
	class Texture;

	class SpriteRenderer {
	private:
		unsigned int vao_id;
		unsigned int vbo_id;
		ShaderProgram spriteShader;
		GLint modelLocation;
		GLint spriteColorLocation;
	protected:
		void initRenderData();
	public:
		SpriteRenderer();

		~SpriteRenderer();

		void draw(Texture* texture, Vector2<int> position, Vector2<int> size, Vector4<float> tint, float rotate = 0);
	};
}

#endif