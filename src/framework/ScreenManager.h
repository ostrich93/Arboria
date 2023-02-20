#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "../containers/List.h"
#include "Screen.h"

namespace Arboria {
	class ScreenManager {
		private:
			List<Screen> stack;
			List<ScreenCommand> screenCommands;

		public:
			void clear();
			Screen* getCurrent();
			Screen* getPrevious(Screen& from);
			bool isEmpty();
			Screen* pop();
			void push(Screen& screen);
			List<ScreenCommand>& getScreenCommands();
			const List<ScreenCommand>& getScreenCommands() const;
	};
}

#endif