#ifndef __WIDGET_ENUMS_H__
#define __WIDGET_ENUMS_H__

namespace Arboria {
	enum WidgetEventType {
		GOT_FOCUS,
		LOST_FOCUS,
		MOUSE_ENTER,
		MOUSE_LEAVE,
		MOUSE_DOWN,
		MOUSE_UP,
		MOUSE_MOVE,
		MOUSE_CLICK,
		KEY_DOWN,
		KEY_UP,
		KEY_PRESS,
		BUTTON_DOWN,
		BUTTON_UP,
		BUTTON_PRESS,

		SCROLLBAR_CHANGE,
		LIST_BOX_CHANGE_HOVER,
		LIST_BOX_CHANGE_SELECT,
		LIST_BOX_CHANGE_CANCEL
	};

	enum HorizontalAlignment {
		HOR_LEFT,
		HOR_CENTER,
		HOR_RIGHT
	};

	enum VerticalAlignment {
		VERT_TOP,
		VERT_CENTER,
		VERT_BOTTOM
	};

	enum ListAlignment {
		LIST_VERTICAL,
		LIST_HORIZONTAL
	};

	enum Orientation {
		VERTICAL,
		HORIZONTAL
	};
}

#endif