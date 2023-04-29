#include "Engine.h"

namespace Arboria {
	RenderDevice::~RenderDevice()
	{
		shutdown();
	}
	void RenderDevice::initialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
			printf("SDL could not intialize: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		window = SDL_CreateWindow("Arboria", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if (!window) {
			printf("Failed to create the window: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}
		context = SDL_GL_CreateContext(window);
		if (!context) {
			printf("Could not create an OpenGL context: %s\n", SDL_GetError());
			SDL_DestroyWindow(window);
			_isQuit = true;
			return;
		}
		glewExperimental = true;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			printf("Warning! Unable to set VSync! SDL_ERROR: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}

		SDL_GL_MakeCurrent(window, context);
		SDL_ShowCursor(SDL_DISABLE);
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
	}
	void RenderDevice::shutdown()
	{
		SDL_GL_DeleteContext(context);
		if (window) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}
}