#ifndef INPUT_ACTION_TYPE_H
#define INPUT_ACTION_TYPE_H

namespace Arboria{
	typedef enum {
		INVALID = 0,
		CONFIRM,
		CANCEL,
		UNIT_MENU,
		HELP,
		START,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		LSHIFT,
		RSHIFT,
		QUIT,
		ACTION_COUNT
	} InputActionType;
}

#endif