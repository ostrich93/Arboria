#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "InputManager.h"
#include "ScreenManager.h"
//#include "../renderer/RenderSystem.h"

namespace Arboria {
	class Engine {
		private:
			InputManager* inputManager;
			ScreenManager* screenManager;
		public:
			InputManager*& getInputManager() { return inputManager; }
			ScreenManager*& getScreenManager() { return screenManager; }
			void init(); //used to initialize all the systems and managers necessary to run the engine
			void run(); //this is the core loop of the game engine.
			void processEvents();
			void draw();
			void shutdown();
			bool isQuit;
	};
}

#endif