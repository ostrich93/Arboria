#ifndef __SCENE_H__
#define __SCENE_H__

#include "../events/Event.h"

namespace Arboria {

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
		ScreenCommand() : commandType(ScreenCommandType::CONTINUE), nextScene(NULL) {}
		ScreenCommand(ScreenCommandType _commandType, Screen* next = NULL) : commandType(_commandType), nextScene(next) {}
	};

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
		virtual int getActionTranslation(WidgetEvent& widgetEvent) = 0;
	};
}
#endif