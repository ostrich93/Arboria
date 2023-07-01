#include "../game/scenes/MainMenu.h"
//#include "../renderer/TextRenderer.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "../renderer/Font.h"
#include "ActionManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "../globals.h"
#include "../FileSystem.h"
#include <physfs.h>

namespace Arboria {

	InputManager* inputManager = NULL;
	ScreenManager* screenManager = NULL;
	FontManager* fontManager = NULL;
	ResourceManager* resourceManager = NULL;
	//SpriteRenderer* spriteRenderer = NULL;
	//TextRenderer* textRenderer = NULL;
	RenderDevice* renderDevice = NULL;
	ActionManager* actionManager = NULL;
	Renderer* renderer = NULL;
	bool _isQuit = false;

	const short MAX_FPS = 60;
	Engine::Engine()
	{
		if (initializeFileSystem() == 0) {
			PHYSFS_ErrorCode errCd = PHYSFS_getLastErrorCode();
			printf("Failed to intialize filesystem. PHYSFS error code %i: %s", (int)errCd, PHYSFS_getErrorByCode(errCd));
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
		//delete textRenderer;
		delete inputManager;
		delete screenManager;
		delete renderer;
		//delete spriteRenderer;
		delete resourceManager;
		delete renderDevice;
	}
	void Engine::init()
	{
		renderDevice = new RenderDevice();
		renderDevice->initialize();
		resourceManager = new ResourceManager();
		inputManager = new InputManager();
		screenManager = new ScreenManager();
		fontManager = new FontManager();
		renderer = new Renderer();
		renderer->initialize();
		//spriteRenderer = new SpriteRenderer();
		//textRenderer = new TextRenderer();
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
		while (!inputManager->isQueueEmpty() && !screenManager->isEmpty()) {
			AEvent* e;
			e = &inputManager->getFront();
			inputManager->popFront();
			if (e->eventType == EventType::EVENT_WINDOW_CLOSE) {
				shutdown();
			}
			else {
				screenManager->getCurrent()->onEvent(e);
			}
		}

		inputManager->clearQueue();
		if (!screenManager->isEmpty()) {
			screenManager->getCurrent()->draw();
			//flush renderer
			//start new frame in renderer
		}
	}

	void Engine::shutdown()
	{
		screenManager->clear();
		_isQuit = true;
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
	
	/*
	* void Engine::run() {
	*     //render time stuff
	*
	*	  while (!quit) {
	*		 frameTimeNow = getMicroseconds();
	*
	*		 frame++;
	* 
	*		 * Poll for SDL events, and translate into game events. Then send them to the global event Queue.
	*		 * for each event in the queue: 
	*			* update the cursor
				* check the eventType, handle shutdown if it was a close window event. Otherwise, call the update function on the current scene.
	*			* if the scene has gui objects currently active, call the onEventOccurred on the root, which does it on its children.
	*		* if there is a scene change submitted, pop or push on the stack appropriately
	*		* clear any excess events
	*		* render the current frame
	*		 processEvents();
	* 
	*		if (screenStack.isEmpty()) {
	*			break;
	*		}
	* 
	*		screenStack
	*	  }
	*	  
	* }
	* 
	* void Engine::processEvents() {
	*	 if (screenStack.isEmpty()) {
	*		 quit = true;
	*		 return;
	*	 }
	* 
	*	 inputManager->handleSDLEvents(); //polls sdl events, translates them into game events, then puts them in the global event queue
	*	 while (eventQueue->size() > 0 && !screenStack.isEmpty()) {
	*		GameEvent* e = eventQueue.pop();
	*		cursor->eventOccurred(e);
	*		switch(e->getType()) {
	*			case EVENT_WINDOW_CLOSE:
	*				this->shutdown();
	*				break;
	*			default:
	*				screenStack.peek()->onEventOccurred(e);
	*				break;
	*		}
	*	 }
	* }
	* 
	* void Engine::handle
	*/
}