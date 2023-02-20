#include "Engine.h"
#include "../game/scenes/MainMenu.h"

namespace Arboria {
	void Engine::run() {
		size_t frame = 0;
		MainMenuScreen firstScreen;
		screenManager->push(firstScreen);
		while (!isQuit) {
			frame++;

			processEvents();

			if (screenManager->isEmpty()) {
				break;
			}

			screenManager->getCurrent()->resume();

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