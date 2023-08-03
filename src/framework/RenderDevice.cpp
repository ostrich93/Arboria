#include "Engine.h"
#include "SystemCVars.h"
#include "Camera.h"

namespace Arboria {
	GLuint currentShaderProgram = UINT32_MAX;
	GLuint activeTexture = UINT32_MAX;

	RenderDevice::RenderDevice() : winWidth(0), winHeight(0)
	{
		for (int i = 0; i < 256; i++) {
			gamma_r[i] = 0;
			gamma_g[i] = 0;
			gamma_b[i] = 0;
		}

		m_clip.x = 0;
		m_clip.y = 0;
		m_clip.w = 0;
		m_clip.h = 0;
		m_dest.x = 0;
		m_dest.y = 0;
		m_dest.w = 0;
		m_dest.h = 0;
	}

	RenderDevice::~RenderDevice()
	{
		shutdown();
	}
	void RenderDevice::initialize()
	{
		systemConfig = new SystemConfig();
		camera = new Camera(0.0f, 0.0f, 0.0f, 0.0f);
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

		window = SDL_CreateWindow("Arboria", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, systemConfig->windowViewportX->getInteger(), systemConfig->windowViewportY->getInteger(), SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!window) {
			Engine::printError("RenderDevice::initialize: Failed to create the window: %s\n", SDL_GetError());
			_isQuit = true;
			return;
		}
		SDL_SetWindowMinimumSize(window, 640, 480);
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
		int width, height;
		SDL_GetWindowSize(window, &winWidth, &winHeight);
	}
	void RenderDevice::shutdown()
	{
		SDL_GL_DeleteContext(context);
		if (window) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}
	void RenderDevice::setTexture(GLuint index, GLenum type, GLuint texture)
	{
		if (activeTexture != index) {
			glActiveTexture(GL_TEXTURE0 + index);
		}
		glBindTexture(type, texture);
	}

	void RenderDevice::resetGLState() {
		activeTexture = UINT32_MAX;
		currentShaderProgram = UINT32_MAX;
	}
}