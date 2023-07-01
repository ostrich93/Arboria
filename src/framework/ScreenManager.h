#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "../containers/List.h"
#include "Engine.h"

namespace Arboria {
	class UIContext;
	struct ScreenCommand;
	class ScreenManager {
		private:
			List<UIContext*> stack;
			List<ScreenCommand> screenCommands;

		public:
			void clear();
			UIContext* getCurrent();
			UIContext* getPrevious(UIContext& from);
			bool isEmpty();
			UIContext* pop();
			void push(UIContext& screen);
			void push(UIContext* screen);
			List<ScreenCommand>& getScreenCommands();
			const List<ScreenCommand>& getScreenCommands() const;
	};
}

#endif