#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../../framework/Scene.h"
#include "../../gui/Widget.h"

namespace Arboria {
	class MainMenuScreen : public Scene {
		private:
			Widget canvas; //canvas has one child, which is a listbox.
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
			bool compare(const Scene& other) const override {
				if (MainMenuScreen* ptr = dynamic_cast<MainMenuScreen*>(const_cast<Scene*>(&other))) {
					return (canvas == ptr->canvas);
				}
			}
	};

	void pushOptionsMenu(Widget* w);
	void setQuit(Widget* w);
}

#endif