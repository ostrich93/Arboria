#include "Label.h"

namespace Arboria {
	Label::Label(const char* textContent) : text(textContent), valign(VerticalAlignment::BOTTOM), halign(HorizontalAlignment::LEFT), wordWrap(true){}

	Label::~Label() = default;

	void Label::eventOccured(Event* e) {
		Widget::eventOccured(e);
	}

	void Label::onRender() {
		Widget::onRender();

		int ypos = align(valign);
	}
}