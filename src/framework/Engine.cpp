#include "Engine.h"
#include "../game/scenes/MainMenu.h"
#include "../renderer/TextRenderer.h"
#include "../globals.h"
#include "../FileSystem.h"
#include <SDL_opengl.h>
#include <GL/glew.h>

namespace Arboria {

	const short MAX_FPS = 60;
	Engine::Engine()
	{
		if (initializeFileSystem() == 0) {
			PHYSFS_ErrorCode errCd = PHYSFS_getLastErrorCode();
			printf("Failed to intialize filesystem. PHYSFS error code %i: %s", (int)errCd, PHYSFS_getErrorByCode(errCd));
			isQuit = true;
			return;
		}
		init();
	}
	Engine::~Engine()
	{
		screenManager->clear();

		freeFileSystem();
		SDL_Quit();
		delete textRenderer;
		delete inputManager;
		delete screenManager;
		delete spriteRenderer;
		delete resourceManager;
	}
	void Engine::init()
	{
		resourceManager = new ResourceManager();
		inputManager = new InputManager();
		screenManager = new ScreenManager();
		fontManager = new FontManager();
		spriteRenderer = new SpriteRenderer();
		textRenderer = new TextRenderer();
	}

	void Engine::run() {
		MainMenuScreen firstScreen;
		screenManager->push(firstScreen);

		float updateTime = 1.0f / MAX_FPS;
		uint64_t currentTimeNs = 0;
		uint64_t elapsedTimeNs = 0;
		int64_t delay_ms = 0;
		uint64_t time_ns = 0;
		while (!isQuit) {
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
						isQuit = true;
						screenManager->clear();
						break;
				}
				if (isQuit) break;
			}
			screenManager->getScreenCommands().clear();

			if (screenManager->getScreenCommands().getLength() > 0) {
				screenManager->getCurrent()->draw();
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
			Event& e = inputManager->getFront();
			inputManager->popFront();
			if (e.getEventType() == EventType::EVENT_WINDOW_CLOSE) {
				shutdown();
			}
			else {
				screenManager->getCurrent()->eventOccured(&e);
			}
		}

		inputManager->clearQueue();
		if (!screenManager->isEmpty()) {
			screenManager->getCurrent()->draw();
			//flush renderer
			//start new frame in renderer
			//SDL_GL_SwapWindow(SDL_Window
		}
	}

	void Engine::initializeDisplay()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
			printf("SDL could not intialize: %s\n", SDL_GetError());
			isQuit = true;
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		glewExperimental = true;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			printf("Warning! Unable to set VSync! SDL_ERROR: %s\n", SDL_GetError());
			isQuit = true;
			return;
		}
		window = SDL_CreateWindow("Arboria", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if (!window) {
			printf("Failed to create the window: %s\n", SDL_GetError());
			isQuit = true;
			return;
		}
		context = SDL_GL_CreateContext(window);
		if (!context) {
			printf("Could not create an OpenGL context: %s\n", SDL_GetError());
			SDL_DestroyWindow(window);
			isQuit = true;
			return;
		}

		SDL_GL_MakeCurrent(window, context);
		SDL_ShowCursor(SDL_DISABLE);
		int width, height;
		SDL_GetWindowSize(window, &width, &height);

		try {
			fontManager = new FontManager();
		}
		catch (std::exception e) {
			printf(e.what());
			isQuit = true;
			return;
		}

		inputManager = new InputManager();
		resourceManager = new ResourceManager();
		spriteRenderer = new SpriteRenderer();
		textRenderer = new TextRenderer();
		screenManager = new ScreenManager();
	}

	void Engine::shutdown()
	{
		screenManager->clear();
		if (!window) return;

		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		isQuit = true;
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