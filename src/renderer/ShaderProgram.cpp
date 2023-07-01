#include "ShaderProgram.h"
#include "../framework/String.h"
#include "../definitions.h"
#include "../FileSystem.h"
#include <physfs.h>
#include <iostream>

namespace Arboria {
	const char* vertexShaderExtension = ".vert";
	const char* fragmentShaderExtension = ".frag";

	Shader::Shader(const char* name, GLenum _type) {
		type = _type;
		auto sourceFileName = getShaderFileName(name);
		if (!PHYSFS_exists(sourceFileName)) {
			Engine::fatalError("ShaderProgram::compileShader: The shader you're attempting to attach does not exist: %s", sourceFileName.c_str());
		}
		String shaderSource = readShaderFile(sourceFileName.c_str());

		id = glCreateShader(type);
		GLint length = shaderSource.size();
		const char* sourcePtr = shaderSource.c_str();
		glShaderSource(id, 1, &sourcePtr, &length);
		glCompileShader(id);

		GLint compileResult;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
		if (!compileResult) {
			GLint logLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
			if (!compileResult || logLength > 0) {
				char* infoLog = new char[logLength];
				glGetShaderInfoLog(id, logLength, &logLength, infoLog);
				infoLog[logLength] = 0;
				if (logLength > 1) {
					Engine::printLog("ShaderProgram::compileShader: Failed to compile shader %s: %s\n", name, infoLog);
				}
			}
		}

		if (!compileResult) {
			glDeleteShader(id);
		}
	}

	String Shader::getShaderFileName(const String& name) {
		if (type == GL_VERTEX_SHADER)
			return name + vertexShaderExtension;

		return name + fragmentShaderExtension;
	}

	Shader::~Shader() {
		glDeleteShader(id);
	}

	ShaderProgram::ShaderProgram(const String& _name) : name(_name), programId(0), currentBuffer(0) {
		vertexShader = new Shader(_name.c_str(), GL_VERTEX_SHADER);
		fragmentShader = new Shader(_name.c_str(), GL_FRAGMENT_SHADER);
		initialize();
		glAttachShader(programId, vertexShader->getShaderId());
		glAttachShader(programId, fragmentShader->getShaderId());
		glBindFragDataLocation(programId, 0, "color");

		if (!link()) {
			char errBuffer[512];
			GLsizei blen;
			glGetProgramInfoLog(programId, sizeof(errBuffer), &blen, errBuffer);
			Engine::printError("ShaderProgram::ShaderProgram(): Failure to link %s", name.c_str());
			Engine::printError(errBuffer);
		}
	}

	ShaderProgram::~ShaderProgram() {
		destroy();
	}

	bool ShaderProgram::link() {
		glLinkProgram(programId);

		GLint linkStatus;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
		return linkStatus == GL_TRUE;
	}

	void ShaderProgram::loadAndAttachShader(GLenum type)
	{
		if (programId == 0) {
			Engine::fatalError("ShaderProgram::loadAndAttachShader: Tried to attach a shader to an uninitialized program: %s", name.c_str());
		}

		const char* extension = type == GL_VERTEX_SHADER ? vertexShaderExtension : fragmentShaderExtension;
		GLuint shader = compileShader(type, name + extension);
		
		if (shader == 0) {
			Engine::printError("ShaderProgram::loadAndAttachShader: Failed to attach shader %s to program %s", extension, name.c_str());
			return;
		}

		glAttachShader(programId, shader);
		glDeleteShader(shader);

	}

	void ShaderProgram::loadAndAttachShader(GLenum type, const char* sourceName) //includes file extension in the sourcename
	{
		if (programId == 0) {
			Engine::fatalError("ShaderProgram::loadAndAttachShader: Tried to attach a shader to an uninitialized program: %s", name.c_str());
		}

		GLuint shader = compileShader(type, sourceName);

		if (shader == 0) {
			Engine::printError("ShaderProgram::loadAndAttachShader: Failed to attach shader %s to program %s", sourceName, name.c_str());
			return;
		}

		glAttachShader(programId, shader);
		glDeleteShader(shader);
	}

	void ShaderProgram::attachVertexShader()
	{
		loadAndAttachShader(GL_VERTEX_SHADER);
	}

	void ShaderProgram::attachVertexShader(const char* sourceName) {
		loadAndAttachShader(GL_VERTEX_SHADER, sourceName);
	}

	void ShaderProgram::attachFragmentShader()
	{
		loadAndAttachShader(GL_FRAGMENT_SHADER);
	}

	void ShaderProgram::attachFragmentShader(const char* sourceName) {
		loadAndAttachShader(GL_FRAGMENT_SHADER, sourceName);
	}

	void ShaderProgram::initialize() {
		programId = glCreateProgram();
	}

	void ShaderProgram::activate() {
		if (currentShaderProgram != programId) {
			currentShaderProgram = programId;
			glUseProgram(programId);
		}
	}

	void ShaderProgram::deactivate() {
		if (currentShaderProgram == programId) {
			currentShaderProgram = UINT32_MAX;
			glUseProgram(0);
		}
	}

	void ShaderProgram::destroy() {
		if (vertexShader != NULL) {
			glDetachShader(programId, vertexShader->getShaderId());
		}
		if (fragmentShader != NULL) {
			glDetachShader(programId, fragmentShader->getShaderId());
		}
		deactivate();
		glDeleteProgram(programId);
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

	void ShaderProgram::setVector2i(const char* name, GLint x, GLint y)
	{
		glUniform2i(glGetUniformLocation(programId, name), x, y);
	}

	void ShaderProgram::setVector2i(const char* name, Vector2<int>& value) {
		glUniform2i(glGetUniformLocation(programId, name), value.x, value.y);
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
	
	GLuint ShaderProgram::compileShader(GLenum type, const char* sourceFilename) {
		if (!PHYSFS_exists(sourceFilename)) {
			Engine::fatalError("ShaderProgram::compileShader: The shader you're attempting to attach does not exist: %s", sourceFilename);
			//Mem_Free(&fullFilename);
			//exit(0);
		}
		String shaderSource = readShaderFile(sourceFilename);
		if (shaderSource.isEmpty()) {
			return 0;
		}

		GLuint shader = glCreateShader(type);
		GLint length = shaderSource.size();
		const char* sourcePtr = shaderSource.c_str();
		glShaderSource(shader, 1, &sourcePtr, &length);
		glCompileShader(shader);

		GLint compileResult;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
		if (!compileResult) {
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			if (!compileResult || logLength > 0) {
				char* infoLog = new char[logLength];
				glGetShaderInfoLog(shader, logLength, &logLength, infoLog);
				infoLog[logLength] = 0;
				if (logLength > 1) {
					Engine::printLog("ShaderProgram::compileShader: Failed to compile shader %s: %s\n", sourceFilename, infoLog);
				}
			}
		}

		if (!compileResult) {
			glDeleteShader(shader);
			return 0;
		}

		return shader;

	}

	String readShaderFile(const char* filename) {
		void* buffer = nullptr;
		int len = readFileFromPhysFS(filename, &buffer);
		if (buffer == nullptr) {
			Engine::printWarning("ShaderProgram::readShaderFile: Could not open shader file %s", filename);
			//Mem_Free(&fullFilename);
			return "";
		}
		String contents(static_cast<char*>(buffer), 0, len);
		Mem_Free(buffer);
		return contents;
	}
}