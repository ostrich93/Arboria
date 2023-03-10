#include "Renderer.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "FontManager.h"
#include "../containers/List.h"
#include <SDL_opengles2.h>

namespace Arboria {
	Renderer::~Renderer() = default;

	class SpriteRenderer : public Renderer {
		private:
			unsigned int vao_id;
			unsigned int vbo_id;
			ShaderProgram spriteShader;
			GLint modelLocation;
			GLint spriteColorLocation;

			void initRenderData() {
				spriteShader.use();
				modelLocation = spriteShader.getUniformLocation("model");
				spriteColorLocation = spriteShader.getUniformLocation("spriteColor");

				float vertices[] = {
				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f
				};

				glGenVertexArrays(1, &vao_id);
				glBindVertexArray(vao_id);
				glGenBuffers(1, &vbo_id);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
			}
		public:
			SpriteRenderer() : spriteShader("baseSurfaceShader.vert", "baseSurfaceShader.frag") {
				initRenderData();
			}

			~SpriteRenderer() {
				glDeleteVertexArrays(1, &this->vao_id);
				glDeleteBuffers(1, &this->vbo_id);
			}

			template<typename T>
			void draw(Texture& texture, Vector2<T> position, Vector2<T> size, float rotate = 0, Vector3<float> tint = Vector3<float>(1.0f)) {
				spriteShader.use();

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(position, 0.0f)); //move to position
				model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //move origin of rotation to the center of the quad.
				model = glm::rotate(model, glm::radians(rotate), vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
				model = glm::scale(model, glm::vec3(size, 1.0f));

				spriteShader.setMatrix4("model", model);
				spriteShader.setVector3f("spriteColor", tint);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

				glBindVertexArray(vao_id);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);

			}
	};
}