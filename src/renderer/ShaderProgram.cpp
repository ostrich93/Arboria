#include "ShaderProgram.h"
#include <SDL_opengles2.h>
#include <iostream>

namespace Arboria {
	ShaderProgram::ShaderProgram(const char* vName, const char* fName) : programId(0), currentBuffer(0)
	{
		vertexShader = new Shader(vName, GL_VERTEX_SHADER);
		fragmentShader = new Shader(fName, GL_FRAGMENT_SHADER);

		programId = glCreateProgram();
		glAttachShader(programId, vertexShader->id);
		glAttachShader(programId, fragmentShader->id);
		if (!link()) {
			char buffer[1024];
			GLsizei length;
			glGetProgramInfoLog(programId, sizeof(buffer), &length, buffer);

			printf("Error linking %s and %s", vName, fName);
			throw std::runtime_error("Failed to link shader program");
		}
	}

	ShaderProgram::~ShaderProgram() {
		if (vertexShader != NULL) glDetachShader(programId, vertexShader->id);
		if (fragmentShader != NULL) glDetachShader(programId, fragmentShader->id);
		glDeleteProgram(programId);
	}

	bool ShaderProgram::link() {
		glLinkProgram(programId);

		GLint linkStatus;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
		return linkStatus == GL_TRUE;
	}

	GLuint ShaderProgram::getAttributeLocation(const char* name) {
		return glGetAttribLocation(programId, name);
	}

	GLuint ShaderProgram::getUniformLocation(const char* name) {
		return glGetUniformLocation(programId, name);
	}

	ShaderProgram& ShaderProgram::use() {
		glUseProgram(programId);
		return *this;
	}

	void ShaderProgram::setFloat(const char* name, GLfloat value) {
		glUniform1f(glGetUniformLocation(programId, name), value);
	}

	void ShaderProgram::setInteger(const char* name, GLint value) {
		glUniform1i(glGetUniformLocation(programId, name), value);
	}
	void ShaderProgram::setVector2f(const char* name, GLfloat x, GLfloat y)
	{
		glUniform2f(glGetUniformLocation(programId, name), x, y);
	}
	void ShaderProgram::setVector2f(const char* name, Vector2f& value)
	{
		glUniform2f(glGetUniformLocation(programId, name), value.getX(), value.getY());
	}
	void ShaderProgram::setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z)
	{
		glUniform3f(glGetUniformLocation(programId, name), x, y, z);
	}
	void ShaderProgram::setVector3f(const char* name, Vector3f& value)
	{
		glUniform3f(glGetUniformLocation(programId, name), value.getX(), value.getY(), value.getZ());
	}
	void ShaderProgram::setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(programId, name), x, y, z, w);
	}
	void ShaderProgram::setVector4f(const char* name, Vector4<GLfloat>& value)
	{
		glUniform4f(glGetUniformLocation(programId, name), value.getX(), value.getY(), value.getZ(), value.getW());
	}
}