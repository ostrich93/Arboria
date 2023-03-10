#include "ShaderProgram.h"
#include <SDL_opengles2.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Arboria {
	ShaderProgram::ShaderProgram(const char* vName, const char* fName) : programId(0), currentBuffer(0)
	{
		std::string vertexSource;
		std::string fragmentSource;

		try {
			std::ifstream vertexShaderFile(vName);
			std::ifstream fragmentShaderFile(fName);
			std::stringstream vertexShaderStream, fragmentShaderStream;
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();
			vertexShaderFile.close();
			fragmentShaderFile.close();
			vertexSource = vertexShaderStream.str();
			fragmentSource = fragmentShaderStream.str();
		}
		catch (std::exception e) {
			printf("Error loading shader files %s and %s\n", vName, fName);
			throw std::runtime_error("Failed to load vertex and fragment shaders");
		}

		vertexShader = new Shader(vertexSource.c_str(), GL_VERTEX_SHADER);
		fragmentShader = new Shader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

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
	void ShaderProgram::setVector2f(const char* name, Vector2<float>& value)
	{
		glUniform2f(glGetUniformLocation(programId, name), value.x, value.y);
	}
	void ShaderProgram::setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z)
	{
		glUniform3f(glGetUniformLocation(programId, name), x, y, z);
	}
	void ShaderProgram::setVector3f(const char* name, Vector3<float>& value)
	{
		glUniform3f(glGetUniformLocation(programId, name), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glUniform4f(glGetUniformLocation(programId, name), x, y, z, w);
	}
	void ShaderProgram::setVector4f(const char* name, Vector4<GLfloat>& value)
	{
		glUniform4f(glGetUniformLocation(programId, name), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setMatrix4(const char* name, glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, false, glm::value_ptr(value));
	}
}