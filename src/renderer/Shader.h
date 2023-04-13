#ifndef __SHADER_H__
#define __SHADER_H__

#include "../framework/Engine.h"

namespace Arboria {
	class String;
	class Shader {
		public:
			GLuint id;
			GLenum type;
			Shader(const String& name, GLenum _type);
			~Shader();
			Shader& use();
	};
}

#endif