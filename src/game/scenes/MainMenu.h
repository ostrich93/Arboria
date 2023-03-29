#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "../../framework/Screen.h"
#include "../../gui/WidgetContainer.h"

namespace Arboria {
	class MainMenuScreen : public Screen {
		private:
			WidgetContainer canvas; //canvas has one child, which is a listbox.
		public:
			MainMenuScreen();
			~MainMenuScreen() override;
			void draw() override;
			void enter() override;
			void eventOccured(Event* ev) override;
			void leave() override;
			void pause() override;
			void resume() override;
			void run() override;
			bool isTransitioning() const override;
	};
}

#endif