#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include "Shader.h"
#include "../utils/Vector.h"

namespace Arboria {
	class ShaderProgram {
		private:
			Shader* vertexShader;
			Shader* fragmentShader;
			GLuint programId;
			GLuint currentBuffer;
			bool link();
		public:
			GLuint usedBuffers = 0;
			ShaderProgram(const char* vName, const char* fName);
			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram(ShaderProgram&&) = default;
			virtual ~ShaderProgram();

			GLuint getAttributeLocation(const char* name);
			GLuint getUniformLocation(const char* name);
			GLuint getProgramId() { return programId; }
			const GLuint getProgramId() const { return programId; }
			ShaderProgram& use();

			void setFloat(const char* name, GLfloat value);
			void setInteger(const char* name, GLint value);
			void setVector2f(const char* name, GLfloat x, GLfloat y);
			void setVector2f(const char* name, Vector2f& value);
			void setVector3f(const char* name, GLfloat x, GLfloat y, GLfloat z);
			void setVector3f(const char* name, Vector3f& value);
			void setVector4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
			void setVector4f(const char* name, Vector4<GLfloat>& value);
	};
}

#endif