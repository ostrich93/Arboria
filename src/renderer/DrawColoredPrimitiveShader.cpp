#include "DrawColoredPrimitiveShader.h"
#include "../framework/Camera.h"

namespace Arboria {
	DrawColoredPrimitiveShader::DrawColoredPrimitiveShader() : ShaderProgram("coloredPrimitiveShader"), uModel(0)
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		glGenBuffers(1, & _vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DrawColoredPrimitiveCommand), NULL, GL_STREAM_DRAW);

		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, sizeof(DrawColoredPrimitiveVertex), reinterpret_cast<void*>(offsetof(DrawColoredPrimitiveVertex, position)));
		glVertexAttribDivisor(vPosition, 1);
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, sizeof(DrawColoredPrimitiveVertex), reinterpret_cast<void*>(offsetof(DrawColoredPrimitiveVertex, color)));
		glVertexAttribDivisor(vColor, 1);

		use();
	}

	DrawColoredPrimitiveShader::~DrawColoredPrimitiveShader() {
		if (_vbo) glDeleteBuffers(1, &_vbo);
		if (_vao) glDeleteVertexArrays(1, &_vao);
	}

	void DrawColoredPrimitiveShader::drawLineInstances(const List<DrawColoredPrimitiveCommand>& instances)
	{
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DrawColoredPrimitiveCommand) * instances.getLength(), instances.getPtr(), GL_STREAM_DRAW);

		setMatrix4("uModel", glm::mat4(1.0f));
		setMatrix4("uViewport", renderDevice->getCamera()->getViewMatrix());
		setMatrix4("uView", renderDevice->getCamera()->getViewProjection());

		glDrawArraysInstanced(GL_LINE_STRIP, 0, 2, instances.getLength());
	}

	void DrawColoredPrimitiveShader::getLocations()
	{
		uModel = getUniformLocation("uModel");
		uViewport = getUniformLocation("uViewport");
		uProjection = getUniformLocation("uProjection");
		vPosition = getAttributeLocation("vPosition");
		vColor = getAttributeLocation("vColor");
	}
}