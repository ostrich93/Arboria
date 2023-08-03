#include "Renderer.h"
#include "Texture.h"
#include "../framework/Camera.h"
#include "../framework/ResourceManager.h"

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
		//spriteShader.setMatrix4("model", renderDevice->getCamera()->getViewMatrix());
		spriteShader.setMatrix4("projection", renderDevice->getCamera()->getViewProjection());


		float vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &vao_id);
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(vao_id);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SpriteRenderer::draw(Texture* texture, Vector2<int> position, Vector2<int> size, Vector4<float> tint, float rotate) {
		spriteShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f)); //move to position
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //move origin of rotation to the center of the quad.
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		spriteShader.setMatrix4("uModel", model);
		spriteShader.setVector4f("spriteColor", tint);

		glActiveTexture(GL_TEXTURE0 + texture->getTextureId());
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::initialize()
	{
		surfaceShader = new DrawSurfaceShader(200);
		textShader = new DrawTextShader();
		coloredPrimitiveShader = new DrawColoredPrimitiveShader();
		glViewport(0, 0, renderDevice->getWindowWidth(), renderDevice->getWindowHeight());
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void Renderer::draw(Texture* _texture, Vector2<GLfloat> _screenPosition, Color _color, GLfloat _scale)
	{
		Vector2<GLfloat> texSize = { _texture->getWidth(), _texture->getHeight() };
		surfaceShader->drawTexture(_texture, _screenPosition, texSize, texSize, { 0,0 }, 0, _color);
		/*DrawSurfaceCommand& drawCmd = surfaceCommandBuffer.allocateNewElement();
		drawCmd.screenSize = { renderDevice->getWindowWidth(), renderDevice->getWindowHeight() };
		drawCmd.screenPosition = _screenPosition;
		drawCmd.texturePosition = { 0, 0 };
		drawCmd.textureSize = { _texture->getWidth(), _texture->getHeight() };
		drawCmd.boundaries = { 0, 0, _texture->getWidth(), _texture->getHeight() };
		drawCmd.color = { _color.r, _color.g, _color.b, _color.a };
		drawCmd.scaleValue = _scale;*/
	}

	void Renderer::drawText(const char* text, Font* font, Vector2<GLfloat> _screenPosition, Color _color, GLfloat _scale)
	{
		DrawTextBatchCommand& textBatchCmd = textCommandBuffers.allocateNewElement();
		textBatchCmd.font = font;
		textBatchCmd.color = _color;
		textBatchCmd.renderBatches.append({ _screenPosition.x, _screenPosition.y, text, _scale });
		
	}

	void Renderer::drawLine(Vector2<GLfloat> _positions[2], Color _colors[2])
	{
		DrawColoredPrimitiveCommand& lineCmd = lineCommandBuffers.allocateNewElement();
		lineCmd.vertices[0] = { _positions[0], {_colors[0].r, _colors[0].g, _colors[0].b, _colors[0].a} };
		lineCmd.vertices[1] = { _positions[1], {_colors[1].r, _colors[1].g, _colors[1].b, _colors[1].a} };
	}

	void Renderer::drawRectangle(Vector2<GLfloat> _positions[4], Color _colors[4]) {
		DrawColoredPrimitiveCommand& rectCmd = rectCommandBuffers.allocateNewElement();
		rectCmd.vertices[0] = { _positions[0], {_colors[0].r, _colors[0].g, _colors[0].b, _colors[0].a} };
		rectCmd.vertices[1] = { _positions[1], {_colors[1].r, _colors[1].g, _colors[1].b, _colors[1].a} };
		rectCmd.vertices[2] = { _positions[2], {_colors[2].r, _colors[2].g, _colors[2].b, _colors[2].a} };
		rectCmd.vertices[3] = { _positions[3], {_colors[2].r, _colors[3].g, _colors[3].b, _colors[3].a} };
	}

	void Renderer::flush() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		flushLines();
		//flushRectangles();
		flushSurfaceCommands();
		flushText();
	}

	void Renderer::flushSurfaceCommands() {
		if (!surfaceCommandBuffer.getLength()) return;
		
		//renderDevice->setTexture(0, GL_TEXTURE_2D_ARRAY, resourceManager->)

		surfaceShader->use();
		//surfaceShader->drawInstances();

		surfaceCommandBuffer.clearFree();
	}

	void Renderer::flushText() {
		if (!textCommandBuffers.getLength()) return;

		textShader->use();
		textShader->drawInstances(textCommandBuffers);

		textCommandBuffers.clearFree();
	}

	void Renderer::flushLines() {
		if (!lineCommandBuffers.getLength()) return;

		coloredPrimitiveShader->use();
		coloredPrimitiveShader->drawLineInstances(lineCommandBuffers);
		lineCommandBuffers.clearFree();
	}

	/*void Renderer::flushRectangles() {
		if (!rectCommandBuffers.getLength()) return;

		coloredPrimitiveShader->use();
		coloredPrimitiveShader->drawRectInstances(rectCommandBuffers);
		rectCommandBuffers.clearFree();
	}*/
}