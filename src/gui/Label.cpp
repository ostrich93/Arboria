#include "Label.h"
#include "../renderer/Font.h"
#include "../renderer/Renderer.h"
#include "WidgetEnums.h"

namespace Arboria {
	Label::Label(const String& textContent, Font* _font) : text(textContent), font(_font), valign(VerticalAlignment::VERT_BOTTOM), halign(HorizontalAlignment::HOR_LEFT), wordWrap(true){}

	Label::~Label() = default;

	bool Label::onEvent(AEvent* e) {
		return Widget::onEvent(e);
	}

	void Label::onRender() {
		Widget::onRender();
		int xpos = align(halign, size.x, font->getTextWidth(text));
		int ypos = align(valign, size.y, font->getMaxHeight(text));

		//go through each character in the text. Each time, 
		renderer->drawText(font, text, xpos, ypos);
		//textRenderer->draw(font, new Vector2<int>(position.x, position.y), tint, text);
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

	void Label::setText(const char* _text)
	{
		text = _text;
		setDirty();
	}

	Font* Label::getFont() const { return font; }

	void Label::setFont(Font* _font) {
		font = _font;
		setDirty();
	}
}