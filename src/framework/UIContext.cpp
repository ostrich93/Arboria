#include "UIContext.h"
#include "../events/Event.h"

namespace Arboria {
	ScreenCommand::ScreenCommand(ScreenCommandType _commandType, UIContext* next)
	{
		commandType = _commandType;
		nextScene = next;
	}
}