#ifndef UICONTEXT_H
#define UICONTEXT_H

#include <typeinfo>

namespace Arboria {
	class AEvent;
	class UIContext {
		public:
			UIContext() {}
			virtual ~UIContext() = default;
			virtual void draw() = 0;
			virtual void enter() = 0;
			virtual bool onEvent(AEvent* ev) = 0;
			virtual bool isTransitioning() const = 0;
			virtual void leave() = 0;
			virtual void pause() = 0;
			virtual void resume() = 0;
			virtual void run() = 0;
			bool operator==(const UIContext& other) const {
				if (typeid(*this) != typeid(other))
					return false;
				return compare(other);
			}
		protected:
			virtual bool compare(const UIContext& other) const {
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
		UIContext* nextScene;
		ScreenCommand() : commandType(ScreenCommandType::CONTINUE){}
		ScreenCommand(ScreenCommandType _commandType, UIContext* next);
	};

}
#endif