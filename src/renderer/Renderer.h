#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "ShaderProgram.h"
#include <SDL_opengl.h>

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

		template<typename T>
		inline void draw(Texture* texture, Vector2<T> position, Vector2<T> size, Vector4<float> tint, float rotate = 0) {
			spriteShader.use();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(position, 0.0f)); //move to position
			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //move origin of rotation to the center of the quad.
			model = glm::rotate(model, glm::radians(rotate), vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
			model = glm::scale(model, glm::vec3(size, 1.0f));

			spriteShader.setMatrix4("model", model);
			spriteShader.setVector4f("spriteColor", tint);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

			glBindVertexArray(vao_id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

		}
	};
}

#endif