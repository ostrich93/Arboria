#include "./OpenGLApi.h"

void Arboria::OpenGLAPI::setTexture(uint16_t index, GLenum type, GLuint texture)
{
	if (OpenGLState::activeTexture != index)
		glActiveTexture(GL_TEXTURE0 + index);

	glBindTexture(type, texture);
}

void Arboria::OpenGLState::reset() {
	activeTexture = UINT16_MAX;
	currentProgram = UINT32_MAX;
}

void Arboria::OpenGLAPI::initialize() {
	OpenGLState::reset();
}