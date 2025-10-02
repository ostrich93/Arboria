#include "../events/Event.h"
#include "./Scene.h"

namespace Arboria {
	ScreenCommand::ScreenCommand(ScreenCommandType _commandType, Scene* next)
	{
		commandType = _commandType;
		nextScene = next;
	}
}