#include "Engine.h"
#include "SystemCVars.h"
#include "Camera.h"
#include "RenderDevice.h"

namespace Arboria {

	//for now, we're targeting only ONE aspect ratio - 16:9
	//we want to target qHD/540p, HD/720p, HD+, and 1080p, which has a common denominator of 320 and 180.
	constexpr unsigned int baseDisplayX = 320;
	constexpr unsigned int baseDisplayY = 180;

	RenderDevice::RenderDevice() : winWidth(0), winHeight(0), displayX(0), displayY(0), camera(NULL)
	{
		
	}

	RenderDevice::~RenderDevice()
	{
		shutdown();
	}
	void RenderDevice::initialize()
	{
		systemConfig = new SystemConfig();
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
			Engine::printError("RenderDevice::Initialize: SDL could not intialize: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}

		systemConfig->windowViewportX->setInteger(systemConfig->defaultWindowViewportX->getInteger());
		systemConfig->windowViewportY->setInteger(systemConfig->defaultWindowViewportY->getInteger());
		systemConfig->windowScale->setFloat(systemConfig->defaultWindowScale->getFloat());

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = SDL_CreateWindow("Arboria", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, baseDisplayX * systemConfig->windowScale->getInteger(), baseDisplayY * systemConfig->windowScale->getInteger(), SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!window) {
			Engine::printError("RenderDevice::initialize: Failed to create the window: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}
		SDL_SetWindowMinimumSize(window, 640, 360);
		context = SDL_GL_CreateContext(window);
		if (!context) {
			Engine::printError("RenderDevice::initialize: Could not create an OpenGL context: %s\n", SDL_GetError());
			SDL_DestroyWindow(window);
			_isQuit = true;
			return;
		}
		SDL_GL_MakeCurrent(window, context);
		glewExperimental = true;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			Engine::printError("RenderDevice::initialize: Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			Engine::printWarning("RenderDevice::initialize: Unable to set VSync! SDL_ERROR: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}

		SDL_ShowCursor(SDL_ENABLE);
		SDL_GetWindowSize(window, &winWidth, &winHeight);
		//camera = new Camera(0.0f, (float)winWidth, (float)winHeight, 0.0f);
	}
	void RenderDevice::shutdown()
	{
		SDL_GL_DeleteContext(context);
		if (window) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}
	void RenderDevice::resize(size_t scaleValue)
	{
		displayX = baseDisplayX * scaleValue;
		displayY = baseDisplayY * scaleValue;
		winWidth = displayX; //for now, this will change once we start enabling other aspect ratios
		winHeight = displayY; //for now

		SDL_SetWindowSize(window, winWidth, winHeight);
	}
}