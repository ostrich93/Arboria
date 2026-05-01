#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../../gui/Window.h"

namespace Arboria {
	class MainMenu : public Window {
		public:
			MainMenu();
			~MainMenu() override;
			void render() override;
			void onEvent(AEvent* e) override;
			bool postParse() override;
		protected:
			bool compare(const Window& other) const override {
				if (MainMenu* ptr = dynamic_cast<MainMenu*>(const_cast<Window*>(&other))) {
					return (root == ptr->root);
				}
			}
	};

	bool pushOptionsMenu(Widget* w, AEvent* e);
	bool setQuit(Widget* w, AEvent* e);
}

#endif