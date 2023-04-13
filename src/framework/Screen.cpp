#include "Screen.h"
#include "../events/Event.h"

namespace Arboria {
	ScreenCommand::ScreenCommand(ScreenCommandType _commandType, Screen* next)
	{
		commandType = _commandType;
		nextScene = next;
	}
}