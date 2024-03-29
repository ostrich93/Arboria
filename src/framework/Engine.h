#ifndef ENGINE_H
#define ENGINE_H

#include "RenderDevice.h"

namespace Arboria {
	class ScreenManager;
	class FontManager;
	//class TextRenderer;
	class ResourceManager;
	//class SpriteRenderer;
	class InputManager;
	class ActionManager;
	class Renderer;
	class Engine {
		public:
			Engine();
			~Engine();
			void init(); //used to initialize all the systems and managers necessary to run the engine
			void run(); //this is the core loop of the game engine.
			void processEvents();
			void shutdown();
			bool isQuit;

			static void printWarning(const char* warning, ...);
			static void printLog(const char* log, ...);
			static void printError(const char* error, ...);
			static void fatalError(const char* error, ...);
	};

	extern InputManager* inputManager;
	extern ScreenManager* screenManager;
	extern FontManager* fontManager;
	extern ResourceManager* resourceManager;
	//extern SpriteRenderer* spriteRenderer;
	//extern TextRenderer* textRenderer;
	extern RenderDevice* renderDevice;
	extern ActionManager* actionManager;
	extern Renderer* renderer;
	extern bool _isQuit;
}

#endif