#ifndef __SCENE_H__
#define __SCENE_H__

#include <typeinfo>

namespace Arboria {
	class Event;

	class Screen {
		public:
			Screen() {}
			virtual ~Screen() = default;
			virtual void draw() = 0;
			virtual void enter() = 0;
			virtual void eventOccured(Event* ev) = 0;
			virtual bool isTransitioning() const = 0;
			virtual void leave() = 0;
			virtual void pause() = 0;
			virtual void resume() = 0;
			virtual void run() = 0;
			bool operator==(const Screen& other) const {
				if (typeid(*this) != typeid(other))
					return false;
				return compare(other);
			}
		protected:
			virtual bool compare(const Screen& other) const {
				return this == &other;
			}
	};

	struct ScreenCommand {
		enum ScreenCommandType {
			CONTINUE,
			POP,
			PUSH,
			REPLACEHEAD,
			REPLACEALL,
			QUIT
		};
		ScreenCommandType commandType;
		Screen* nextScene;
		ScreenCommand() : commandType(ScreenCommandType::CONTINUE){}
		ScreenCommand(ScreenCommandType _commandType, Screen* next);
	};

}
#endif