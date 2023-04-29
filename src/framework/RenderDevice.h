#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

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
			~RenderDevice();
			void initialize();
			void shutdown();
			inline SDL_Window* getWindow() const { return window; }

	};
}

#endif