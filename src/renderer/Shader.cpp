#include "Shader.h"
#include <SDL_opengles2.h>
#include <iostream>

namespace Arboria {
	Shader::Shader(const String& name, GLenum _type) : type(_type) {
		id = glCreateShader(type);
		const GLchar* stringSource = name.c_str();
		const GLint sourceLength = name.length();
		glShaderSource(id, 1, &stringSource, &sourceLength);
		glCompileShader(id);

		GLint compileStatus;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus != GL_TRUE) {
			GLint logLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(id, logLength, NULL, infoLog);

			printf("Failed to compile shader:%s\n", infoLog);
			glDeleteShader(id);

			throw std::runtime_error("Error compiling shader");
		}
	}

	Shader::~Shader() {
		glDeleteShader(id);
	}

	Shader& Shader::use() {
		glUseProgram(id);
		return *this;
	}
}