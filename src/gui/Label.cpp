#include "Label.h"

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

	String Label::getText() const
	{
		return text;
	}

	void Label::setText(String& _text) { text = _text; }

	Font* Label::getFont() const { return font; }

	void Label::setFont(Font* _font) { font = _font; }
}