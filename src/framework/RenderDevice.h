#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

namespace Arboria {
	class Camera;
	class RenderDevice {
		private:
			//SDL_DisplayMode displayMode;
			SDL_Window* window;
			SDL_GLContext context;
			Camera* camera;

			int winWidth;
			int winHeight;
			int displayX; //size of the viewport
			int displayY; //size of the viewport
		public:
			RenderDevice();
			~RenderDevice();
			void initialize();
			void shutdown();
			inline SDL_Window* getWindow() const { return window; }
			inline Camera* getCamera() const { return camera; }
			inline int getWindowWidth() const { return winWidth; }
			inline int getWindowHeight() const { return winHeight; }
			inline int getDisplayX() const { return displayX; }
			inline void setDisplayX(int x) { displayX = x; }
			inline int getDisplayY() const { return displayY; }
			inline void setDisplayY(int y) { displayY = y; }
			void resize(size_t scaleValue);
			//void setGamma(float g);
			//void resetGamma();
			static const unsigned char BITS_PER_PIXEL = 32;

	};
}

#endif