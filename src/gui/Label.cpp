#include "Label.h"
#include "../Heap.h"

namespace Arboria {
	Label::Label(const String& textContent, Font* _font) : text(textContent), font(_font), valign(VerticalAlignment::BOTTOM), halign(HorizontalAlignment::LEFT), wordWrap(true){}

	Label::~Label() = default;

	void Label::eventOccured(Event* e) {
		Widget::eventOccured(e);
	}

	void Label::onRender() {
		Widget::onRender();

		//go through each character in the text. Each time, 
		
	}
}