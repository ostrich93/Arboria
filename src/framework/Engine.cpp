#include "../game/scenes/MainMenu.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "../renderer/Font.h"
#include "ActionManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "../globals.h"
#include "../FileSystem.h"
#include <physfs.h>

unsigned int gCurrentDrawCount = 0;

namespace Arboria {

	double gLastKeyboardPressTime = 0;
	int64_t gCurrentTime = 0;
	int64_t ticksPerSecond = 0;
	constexpr size_t scaleValues[5] = {2, 3, 4, 5, 6};

	InputManager* inputManager = NULL;
	ScreenManager* screenManager = NULL;
	FontManager* fontManager = NULL;
	ResourceManager* resourceManager = NULL;
	RenderDevice* renderDevice = NULL;
	ActionManager* actionManager = NULL;
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
		screenManager->clear();

		freeFileSystem();
		//SDL_Quit();
		delete inputManager;
		delete screenManager;
		delete renderer;
		delete resourceManager;
		delete renderDevice;
	}
	void Engine::init()
	{
		renderDevice = new RenderDevice();
		renderDevice->initialize();
		resourceManager = ResourceManager::createResourceManager();
		inputManager = new InputManager();
		screenManager = new ScreenManager();
		renderer = new Renderer();
		//renderer->initialize();
		actionManager = new ActionManager();
	}

	void Engine::run() {
		MainMenuScreen firstScreen;
		screenManager->push(firstScreen);

		float updateTime = 1.0f / MAX_FPS;
		uint64_t currentTimeNs = 0;
		uint64_t elapsedTimeNs = 0;
		int64_t delay_ms = 0;
		uint64_t time_ns = 0;
		while (!_isQuit) {
			currentTimeNs = getNanoseconds();

			processEvents();

			if (screenManager->isEmpty()) {
				break;
			}

			screenManager->getCurrent()->run();

			for (int i = 0; i < screenManager->getScreenCommands().getLength(); i++) {
				ScreenCommand screenCommand = screenManager->getScreenCommands()[i];
				switch(screenCommand.commandType) {
					case ScreenCommand::ScreenCommandType::CONTINUE:
						break;
					case ScreenCommand::ScreenCommandType::PUSH:
						screenManager->push(*screenCommand.nextScene);
						break;
					case ScreenCommand::ScreenCommandType::POP:
						screenManager->pop();
						break;
					case ScreenCommand::ScreenCommandType::REPLACEHEAD:
						screenManager->pop();
						screenManager->push(*screenCommand.nextScene);
						break;
					case ScreenCommand::ScreenCommandType::REPLACEALL:
						screenManager->clear();
						screenManager->push(*screenCommand.nextScene);
						break;
					case ScreenCommand::ScreenCommandType::QUIT:
						_isQuit = true;
						screenManager->clear();
						break;
				}
				if (_isQuit) break;
			}
			screenManager->getScreenCommands().clear();

			if (screenManager->getCurrent()) {
				screenManager->getCurrent()->draw();
				renderer->flush();
				renderer->newFrame();
				SDL_GL_SwapWindow(renderDevice->getWindow());
			}
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
		while (!inputManager->isQueueEmpty() && !screenManager->isEmpty()) {
			AEvent* e;
			e = &inputManager->getFront();
			inputManager->popFront();
			if (e->eventType == EventType::EVENT_WINDOW_CLOSE) {
				shutdown();
			}
			else {
				switch (e->inputDeviceType) {
				case InputDeviceType::INPUT_DEVICE_KEYBOARD:
					inputManager->updateKeyState(e->keyboardEvent.keyCode, e->eventType == EventType::EVENT_KEY_DOWN);
					break;
				default:
					break;
				}
				screenManager->getCurrent()->onEvent(e);
			}
		}

		inputManager->clearQueue();
		
	}

	UIContext* Engine::getCurrentScreen()
	{
		return screenManager->getCurrent();
	}

	UIContext* Engine::getPreviousScreen()
	{
		return screenManager->getPrevious();
	}

	UIContext* Engine::getPreviousScreen(UIContext* from)
	{
		return screenManager->getPrevious(*from);
	}

	void Engine::pushScreenCommand(const ScreenCommand& cmd)
	{
		screenManager->pushScreenCommand(cmd);
	}

	void Engine::shutdown()
	{
		screenManager->clear();
		_isQuit = true;
	}

	void Engine::resize(int scaleIndex) {
		int idx = Math::clampInt(0, 4, scaleIndex);

		size_t scaleValue = scaleValues[idx];
		renderDevice->resize(scaleValue);
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