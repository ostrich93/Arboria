#ifndef SCENE_H
#define SCENE_H

#include <typeinfo>

namespace Arboria {
	class AEvent;
	class Scene {
		public:
			Scene() {}
			virtual ~Scene() = default;
			virtual void draw() = 0;
			virtual void enter() = 0;
			virtual bool onEvent(AEvent* ev) = 0;
			virtual bool isTransitioning() const = 0;
			virtual void leave() = 0;
			virtual void pause() = 0;
			virtual void resume() = 0;
			virtual void run() = 0;
			bool operator==(const Scene& other) const {
				if (typeid(*this) != typeid(other))
					return false;
				return compare(other);
			}
		protected:
			virtual bool compare(const Scene& other) const {
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
		Scene* nextScene;
		ScreenCommand() : commandType(ScreenCommandType::CONTINUE), nextScene(NULL){}
		ScreenCommand(ScreenCommandType _commandType, Scene* next);
	};

}
#endif