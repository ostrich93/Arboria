#ifndef __INPUT_ACTION_TYPE_H__
#define __INPUT_ACTION_TYPE_H__

namespace Arboria{
	enum InputActionType {
		INVALID = -1,
		CANCEL = 0,
		CONFIRM,
		UNIT_MENU,
		HELP,
		SKIP,
		START,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		LSHIFT,
		RSHIFT,
		QUIT,
		ACTION_COUNT
	};
}

#endif