#include "Label.h"
#include "../renderer/Font.h"
#include "../renderer/FontManager.h"
#include "../renderer/TextRenderer.h"
#include "WidgetEnums.h"

namespace Arboria {
	Label::Label(const String& textContent, Font* _font) : text(textContent), font(_font), valign(VerticalAlignment::VERT_BOTTOM), halign(HorizontalAlignment::HOR_LEFT), wordWrap(true){}

	Label::~Label() = default;

	void Label::eventOccured(Event* e) {
		Widget::eventOccured(e);
	}

	void Label::onRender() {
		Widget::onRender();
		int xpos = align(halign, size.x, font->getSize().max_width);
		int ypos = align(valign, size.y, font->getSize().max_height);

		//go through each character in the text. Each time, 
		textRenderer->draw(font, &location, tint, text);
	}

	void Label::run()
	{
		Widget::run();
	}

	String Label::getText() const
	{
		return text;
	}

	void Label::setText(String& _text) { 
		text = _text;
		setDirty();
	}

	Font* Label::getFont() const { return font; }

	void Label::setFont(Font* _font) {
		font = _font;
		setDirty();
	}
}