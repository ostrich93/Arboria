#include "ColoredPrimitiveShader.h"
#include "../framework/Camera.h"

namespace Arboria {
	ColoredPrimitiveShader::ColoredPrimitiveShader(unsigned int bufferCount) : ShaderProgram("coloredPrimitiveShader"), currentBuffer(0)
	{
		assert(bufferCount > 0);
		compileShader(name);
		use();
		
		getLocations();

		for (unsigned int i = 0; i < bufferCount; i++) {
			ColoredPrimitiveBuffer b;

			glGenVertexArrays(1, &b.vao_id);
			glBindVertexArray(b.vao_id);
			glGenBuffers(1, &b.vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, b.vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredPrimitiveVDStruct), NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredPrimitiveVertex), reinterpret_cast<void*>(offsetof(ColoredPrimitiveVertex, position)));
			glVertexAttribDivisor(vPosition, 1);
			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, sizeof(ColoredPrimitiveVertex), reinterpret_cast<void*>(offsetof(ColoredPrimitiveVertex, color)));
			glVertexAttribDivisor(vColor, 1);

			buffers.append(b);
		}

	}

	ColoredPrimitiveShader::~ColoredPrimitiveShader() {
		if (programId) {
			glDeleteProgram(programId);
		}
		for (auto& buf : buffers) {
			glDeleteVertexArrays(1, &buf.vao_id);
			glDeleteBuffers(1, &buf.vertex_buffer);
		}
		buffers.clearFree();
	}

	void ColoredPrimitiveShader::drawQuad(Vector2<float> positions[4], Color colors[4], Vector2<unsigned int> viewportSize)
	{
		auto& buf = buffers[currentBuffer];

		for (int i = 0; i < 4; i++) {
			buf.data.vertices[i].position = positions[i];
			buf.data.vertices[i].color = colors[i];
		}

		glBindVertexArray(buf.vao_id);
		use();
		glUniform2f(uViewportSize, viewportSize.x, viewportSize.y);

		glBindBuffer(GL_ARRAY_BUFFER, buf.vertex_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ColoredPrimitiveVDStruct), &buf.data);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		currentBuffer = (currentBuffer + 1) % buffers.getLength();	
		usedBuffers++;
	}

	void ColoredPrimitiveShader::drawLine(Vector2<float> positions[2], Color colors[2], Vector2<unsigned int> viewportSize, float thickness)
	{
		auto& buf = buffers[currentBuffer];

		for (int i = 0; i < 2; i++) {
			buf.data.vertices[i].position = positions[i];
			buf.data.vertices[i].color = colors[i];
		}

		glBindVertexArray(buf.vao_id);
		use();
		glUniform2f(uViewportSize, viewportSize.x, viewportSize.y);
		glLineWidth(thickness);

		glBindBuffer(GL_ARRAY_BUFFER, buf.vertex_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ColoredPrimitiveVDStruct), &buf.data);
		glDrawArrays(GL_LINE_STRIP, 0, 2);

		currentBuffer = (currentBuffer + 1) % buffers.getLength();
		usedBuffers++;
	}

	void ColoredPrimitiveShader::getLocations()
	{
		uViewportSize = getUniformLocation("uViewportSize");
		vPosition = getAttributeLocation("vPosition");
		vColor = getAttributeLocation("vColor");
	}
}