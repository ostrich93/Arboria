#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "../framework/Engine.h"
#include "../framework/String.h"
#include "../utils/Vector.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//PFNGLGENVERTEXARRAYSPROC		glGenVertexArrays;
//PFNGLBINDVERTEXARRAYPROC		glBindVertexArray;
//PFNGLDELETEVERTEXARRAYSPROC		glDeleteVertexArrays;
//PFNGLVERTEXATTRIBDIVISORPROC	glVertexAttribDivisor;

namespace Arboria {
	class ShaderProgram {
		protected:
			String name;
			GLuint programId;
			GLuint currentBuffer;
		public:
			GLuint usedBuffers = 0;
			ShaderProgram(const String& _name);
			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram(ShaderProgram&&) = default;
			virtual ~ShaderProgram();

			void initialize();
			void activate();
			void deactivate();
			void destroy();
			bool link();
			void loadAndAttachShader(GLenum type);
			void loadAndAttachShader(GLenum type, const char* sourceName);
			void attachVertexShader();
			void attachVertexShader(const char* sourceName);
			void attachFragmentShader();
			void attachFragmentShader(const char* sourceName);
			GLuint getAttributeLocation(const char* name);
			GLuint getUniformLocation(const char* name);
			GLuint getProgramId() { return programId; }
			const GLuint getProgramId() const { return programId; }
			ShaderProgram& use();

			void setFloat(const char* name, GLfloat value);
			void setInteger(const char* name, GLint value);
			void setVector2f(const char* name, GLfloat x, GLfloat y);
			void setVector2f(const char* name, Vector2<float>& value);
			void setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z);
			void setVector3f(const char* name, Vector3<float>& value);
			void setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
			void setVector4f(const char* name, Vector4<GLfloat>& value);
			void setMatrix4(const char* name, glm::mat4& value);
		protected:
			GLuint compileShader(GLenum type, const char* sourceFilename);
			
	};

	String readShaderFile(const char* filename);
}

#endif