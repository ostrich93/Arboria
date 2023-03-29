#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "InputManager.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/FontManager.h"

namespace Arboria {
	class TextRenderer;
	class Engine {
		private:
			InputManager* inputManager;
			ScreenManager* screenManager;
			FontManager* fontManager;
			ResourceManager* resourceManager;
			SpriteRenderer* spriteRenderer;
			TextRenderer* textRenderer;
			SDL_DisplayMode screenMode;
			SDL_Window* window;
			SDL_GLContext context;
		public:
			Engine();
			~Engine();
			InputManager* getInputManager() { return inputManager; }
			ScreenManager* getScreenManager() { return screenManager; }
			FontManager* getFontManager() { return fontManager; }
			ResourceManager* getResourceManager() { return resourceManager; }
			SpriteRenderer* getSpriteRenderer() { return spriteRenderer; }
			TextRenderer* getTextRenderer() { return textRenderer; }
			void init(); //used to initialize all the systems and managers necessary to run the engine
			void run(); //this is the core loop of the game engine.
			void processEvents();
			void initializeDisplay();
			void shutdown();
			bool isQuit;
	};
}

#endif