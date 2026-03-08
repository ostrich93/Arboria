#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../../framework/UIContext.h"
#include "../../gui/Widget.h"

namespace Arboria {
	class MainMenuScreen : public UIContext {
		private:
			Widget root; //canvas has one child, which is a listbox.
		public:
			MainMenuScreen();
			~MainMenuScreen() override;
			void draw() override;
			void enter() override;
			//void eventOccured(SystemEvent* ev) override;
			bool onEvent(AEvent* ev) override;
			void leave() override;
			void pause() override;
			void resume() override;
			void run() override;
			bool isTransitioning() const override;
		protected:
			bool compare(const UIContext& other) const override {
				if (MainMenuScreen* ptr = dynamic_cast<MainMenuScreen*>(const_cast<UIContext*>(&other))) {
					return (root == ptr->root);
				}
			}
	};

	void pushOptionsMenu(AEvent* e, void* context);
	void setQuit(AEvent* e, void* context);
}

#endif