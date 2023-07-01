#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../utils/Rectangle.h"

namespace Arboria {
	class Camera;
	class RenderDevice {
		private:
			SDL_DisplayMode displayMode;
			SDL_Window* window;
			SDL_GLContext context;
			Camera* camera;

			int winWidth;
			int winHeight;

			Rectangle m_clip;
			Rectangle m_dest;

			uint16_t gamma_r[256];
			uint16_t gamma_g[256];
			uint16_t gamma_b[256];
		public:
			RenderDevice();
			~RenderDevice();
			void initialize();
			void shutdown();
			inline SDL_Window* getWindow() const { return window; }
			inline Camera* getCamera() const { return camera; }
			inline int getWindowWidth() const { return winWidth; }
			inline int getWindowHeight() const { return winHeight; }
			//void setGamma(float g);
			//void resetGamma();
			void setTexture(GLuint index, GLenum type, GLuint texture);
			void resetGLState();
			static const unsigned char BITS_PER_PIXEL = 32;

	};

	extern GLuint currentShaderProgram;
	extern GLuint activeTexture;
}

#endif