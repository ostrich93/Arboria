#ifndef __RENDER_DEVICE_H__
#define __RENDER_DEVICE_H__

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

namespace Arboria {
	class RenderDevice {
		private:
			SDL_DisplayMode displayMode;
			SDL_Window* window;
			SDL_GLContext context;
		public:
			void initialize();
	};
}

#endif