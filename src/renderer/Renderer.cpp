#include "Renderer.h"
#include "Texture.h"
#include "../containers/List.h"

namespace Arboria {

	SpriteRenderer::~SpriteRenderer() = default;

	SpriteRenderer::SpriteRenderer() : spriteShader("baseSurfaceShader.vert", "baseSurfaceShader.frag") {
		initRenderData();
	}

	SpriteRenderer::~SpriteRenderer() {
		glDeleteVertexArrays(1, &this->vao_id);
		glDeleteBuffers(1, &this->vbo_id);
	}

	void SpriteRenderer::initRenderData() {
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
}