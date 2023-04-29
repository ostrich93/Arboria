#include "Renderer.h"
#include "Texture.h"
#include "../containers/List.h"

namespace Arboria {
	SpriteRenderer::SpriteRenderer() : spriteShader("baseSurfaceShader") {
		initRenderData();
	}

	SpriteRenderer::~SpriteRenderer() {
		glDeleteVertexArrays(1, &this->vao_id);
		glDeleteBuffers(1, &this->vbo_id);
		spriteShader.destroy();
	}

	void SpriteRenderer::initRenderData() {
		spriteShader.initialize();
		spriteShader.attachVertexShader();
		spriteShader.attachFragmentShader();
		spriteShader.link();
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

	void SpriteRenderer::draw(Texture* texture, Vector2<int> position, Vector2<int> size, Vector4<float> tint, float rotate) {
		spriteShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f)); //move to position
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //move origin of rotation to the center of the quad.
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		spriteShader.setMatrix4("model", model);
		spriteShader.setVector4f("spriteColor", tint);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}