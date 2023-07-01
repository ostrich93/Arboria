#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "../../framework/UIContext.h"
#include "../../gui/Widget.h"

namespace Arboria {
	class OptionsMenu : public UIContext {
		private:
			Widget container;
			Widget* currentWindow;
			Widget* tabList[3];
			Widget* tabWindows[3];
			Widget* bindWindow;
			bool inBindWindow;
			bool waitingBind; //when changing the key/button bindings
			uint8_t selectTab;
		public:
			OptionsMenu();
			~OptionsMenu() override;
			void draw() override;
			void enter() override;
			bool onEvent(AEvent* ev) override;
			void leave() override;
			void pause() override;
			void resume() override;
			void run() override;
			bool isTransitioning() const override;
	};
}

#endif