#ifndef RENDERER_OPENGL_API_H
#define RENDERER_OPENGL_API_H

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <cstdint>

namespace Arboria {
	namespace OpenGLAPI {
		void initialize();
		void setTexture(uint16_t index, GLenum type, GLuint texture);
	}

	namespace OpenGLState {
		extern uint16_t activeTexture;
		extern GLuint currentProgram;

		void reset();
	}
}

#endif