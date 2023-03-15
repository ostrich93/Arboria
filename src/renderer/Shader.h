#ifndef __SHADER_H__
#define __SHADER_H__

#include <SDL_opengl.h>
#include "../framework/String.h"

namespace Arboria {
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