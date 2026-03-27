#ifndef ENGINE_H
#define ENGINE_H

#include "RenderDevice.h"

extern unsigned int gCurrentDrawCount;

namespace Arboria {
	class ScreenManager;
	class FontManager;
	class ResourceManager;
	class InputManager;
	class ActionManager;
	class Renderer;
	class Surface;
	class UIContext;
	struct ScreenCommand;
	class Engine {
		private:
			Surface* scaleSurface;
		public:
			Engine();
			~Engine();
			void init(); //used to initialize all the systems and managers necessary to run the engine
			void run(); //this is the core loop of the game engine.
			void processEvents();
			UIContext* getCurrentScreen();
			UIContext* getPreviousScreen();
			UIContext* getPreviousScreen(UIContext* from);
			void pushScreenCommand(const ScreenCommand& cmd);
			void shutdown();
			void resize(int scaleIndex);
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
	extern RenderDevice* renderDevice;
	extern ActionManager* actionManager;
	extern Renderer* renderer;
	extern bool _isQuit;
	extern int64_t gCurrentTime;
	extern double gLastKeyboardPressTime;
	extern int64_t ticksPerSecond;
}

#endif