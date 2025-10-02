#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "../containers/List.h"
#include "Engine.h"

namespace Arboria {
	class Scene;
	struct ScreenCommand;
	class ScreenManager {
		private:
			List<Scene*> stack;
			List<ScreenCommand> screenCommands;

		public:
			void clear();
			Scene* getCurrent();
			Scene* getPrevious(Scene& from);
			bool isEmpty();
			Scene* pop();
			void push(Scene& screen);
			void push(Scene* screen);
			List<ScreenCommand>& getScreenCommands();
			const List<ScreenCommand>& getScreenCommands() const;
	};
}

#endif