#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "../../framework/Screen.h"
#include "../../gui/WidgetContainer.h"

namespace Arboria {
	class MainMenuScreen : public Screen {
		private:
			WidgetContainer canvas;
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
			int getActionTranslation(WidgetEvent& widgetEvent) override;
			bool isTransitioning() const override;
			unsigned int currentButtonIndex;
	};
}

#endif