#ifndef SESSION_H
#define SESSION_H

#include "Engine.h"
#include "CommandSystem.h"

namespace Arboria {
	class AEvent;
	class Window;
	class MainMenu;
	class OptionsMenu;

	class Session {
		private:
			Window* guiActive;
			MainMenu* mainMenu;
			OptionsMenu* optionsMenu;
			int activeScene;
			int lastInputDevice; // used to keep track of the type of the previous user input
			void handleMainMenuCommands(Window* gui, const char* command);
			bool handleOptionsMenuCommands(CommandArgs& args, int& icmd);
		public:
			~Session();
			explicit Session();
			void init();
			void shutdown();
			void update();
			void render();
			bool processEvent(AEvent* ev);
			void dispatchCommand(Window* gui, const char* command);
			void handleMenuEvent(AEvent* ev);
			bool lastInputDeviceMatches(const AEvent* e);

			Window* getActiveMenu() { return guiActive; }
			Window* setGui(Window* gui);
			MainMenu* getMainMenu() { return mainMenu; }

			int getLastInputDevice() const { return lastInputDevice; }
			void setLastInputDevice(const int inputDevice) { lastInputDevice = inputDevice; }

			int getCurrentScene() const { return activeScene; }
			void setActiveScene(int scene) { activeScene = scene; }
			void handleStartMenu();
			void exitMenu() { guiActive = NULL; }

			//bool menuActive;
			bool isBindWait = false;
	};

	extern Session* session;
}

#endif