#include "../game/scenes/MainMenu.h"
#include "ResourceManager.h"
#include "../renderer/Font.h"
#include "InputManager.h"
#include "SystemCVars.h"
#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "../globals.h"
#include "../FileSystem.h"
#include "Session.h"
#include <physfs.h>

unsigned int gCurrentDrawCount = 0;

namespace Arboria {

	double gLastKeyboardPressTime = 0;
	int64_t gCurrentTime = 0;
	int64_t ticksPerSecond = 0;
	constexpr size_t scaleValues[5] = {2, 3, 4, 5, 6};

	InputManager* inputManager = NULL;
	FontManager* fontManager = NULL;
	ResourceManager* resourceManager = NULL;
	RenderDevice* renderDevice = NULL;
	Renderer* renderer = NULL;
	bool _isQuit = false;

	const short MAX_FPS = 60;
	Engine::Engine() : scaleSurface(nullptr)
	{
		if (initializeFileSystem() == 0) {
			PHYSFS_ErrorCode errCd = PHYSFS_getLastErrorCode();
			printf("Failed to intialize filesystem. PHYSFS error code %i: %s", (int)errCd, PHYSFS_getErrorByCode(errCd));
			isQuit = false;

			return;
		}
		if (!QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond)) {
			printf("Failed to initialize Engine: Could not get ticks per second");
			isQuit = false;
			return;
		}
		if (!QueryPerformanceCounter((LARGE_INTEGER*)&gCurrentTime)) {
			printf("Failed to initialize Engine: Could not get current time");
			isQuit = false;
			return;
		}
			
		init();
	}
	Engine::~Engine()
	{

		freeFileSystem();
		//SDL_Quit();
		delete inputManager;
		delete renderer;
		delete resourceManager;
		delete renderDevice;
	}
	void Engine::init()
	{
		systemConfig->initialize();
		renderDevice = new RenderDevice();
		renderDevice->initialize();
		resourceManager = ResourceManager::createResourceManager();
		inputManager = new InputManager();
		renderer = new Renderer();
		//renderer->initialize();

		session->init();
		session->handleStartMenu();
	}

	void Engine::run() {
		float updateTime = 1.0f / MAX_FPS;
		uint64_t currentTimeNs = 0;
		uint64_t elapsedTimeNs = 0;
		int64_t delay_ms = 0;
		uint64_t time_ns = 0;
		while (!_isQuit) {
			currentTimeNs = getNanoseconds();

			processEvents();

			if (_isQuit)
				break;
			session->render();
			renderer->flush();
			renderer->newFrame();
			SDL_GL_SwapWindow(renderDevice->getWindow());
			elapsedTimeNs = getNanoseconds() - currentTimeNs;
			delay_ms = Math::floor(1000.f / MAX_FPS - elapsedTimeNs / 1e6);
			time_ns = (elapsedTimeNs + delay_ms * 1e9 / 1e6);

			//check and call sdl delay if needed
			uint64_t newElapsedTime = getNanoseconds() - currentTimeNs;
			int16_t delay = delay_ms - (newElapsedTime / 1000 - elapsedTimeNs / 1000) / 1000;
			if (delay > 0) {
				SDL_Delay(delay);
			}
		}
	}

	void Engine::processEvents() {
		inputManager->translateSdlEvents();
		int64_t current_time = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
		inputManager->setDeltaTime((float)(current_time - gCurrentTime) / ticksPerSecond);
		gCurrentTime = current_time;
		while (!inputManager->isQueueEmpty()) {
			AEvent* e;
			e = &inputManager->getFront();
			inputManager->popFront();
			if (e->eventType == EventType::EVENT_WINDOW_CLOSE) {
				shutdown();
			}
			else {
				switch (e->eventType) {
				case EventType::EVENT_KEY:
				case EventType::EVENT_CONTROLLER_BUTTON:
					inputManager->updateKeyState(e->eventValue, e->eventValue2);
					break;
				default:
					break;
				}
				session->processEvent(e);
				//screenManager->getCurrent()->onEvent(e);
			}
		}

		inputManager->clearQueue();
		
	}

	void Engine::shutdown()
	{
		
		_isQuit = true;
	}

	void Engine::resize() {

		size_t scaleValue = systemConfig->windowViewportX->getInteger() / systemConfig->windowViewportY->getInteger();
		renderDevice->resize();
		if (renderDevice->getDisplayX() != renderDevice->getWindowWidth() || renderDevice->getDisplayY() != renderDevice->getWindowHeight()) {
			if (scaleSurface) {
				free(scaleSurface);
			}
			scaleSurface = new Surface(Vector2<unsigned int>{Math::iMax(640, renderDevice->getDisplayX()), Math::iMax(360, renderDevice->getDisplayY())});
		}
		else {
			if (scaleSurface) {
				free(scaleSurface);
				scaleSurface = nullptr;
			}
		}
	}

	void Engine::printWarning(const char* warning, ...)
	{
		va_list argptr;
		char text[4096];

		va_start(argptr, warning);
		vsprintf(text, warning, argptr);
		va_end(argptr);

		printf("Warning: %s", text);
		return;
	}

	void Engine::printLog(const char* log, ...)
	{
		va_list argptr;
		char text[4096];

		va_start(argptr, log);
		vsprintf(text, log, argptr);
		va_end(argptr);

		printf("%s", text);
		return;
	}

	void Engine::printError(const char* error, ...)
	{
		va_list argptr;
		char text[4096];

		va_start(argptr, error);
		vsprintf(text, error, argptr);
		va_end(argptr);

		printf("Error: %s", text);
		return;
	}

	void Engine::fatalError(const char* error, ...) {
		va_list argptr;
		char text[4096];

		va_start(argptr, error);
		vsprintf(text, error, argptr);
		va_end(argptr);

		printf("Fatal Error: %s", text);
		exit(0);
	}
	
}