#include "Label.h"
#include "../renderer/Font.h"
#include "../renderer/Renderer.h"
#include "WidgetEnums.h"
#include "../framework/Lexer.h"
#include "../framework/ResourceManager.h"

namespace Arboria {
	Label::Label(Window* gui, const String& textContent, Font* _font) : Widget(gui), text(textContent), font(_font), valign(VerticalAlignment::VERT_BOTTOM), halign(HorizontalAlignment::HOR_LEFT){}

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

	bool Label::parseInternalValue(const char* _name, Lexer* src)
	{
		bool ret = Widget::parseInternalValue(_name, src);
		if (!ret) {
			if (String::iCompare(_name, "tint") == 0) {
				parseColor(src, tint);
				return true;
			}
			if (String::iCompare(_name, "text") == 0) {
				parseString(src, text);
				return true;
			}
			if (String::iCompare(_name, "textScale") == 0) {
				textScale = src->parseFloat();
				return true;
			}
			if (String::iCompare(_name, "alignment") == 0) {
				parseAlignment(src, valign, halign);
				return true;
			}
			if (String::iCompare(_name, "font") == 0) {
				parseFont(src, font);
				return true;
			}
			if (String::iCompare(_name, "wordWrap") == 0) {
				bool wordWrap = src->parseBool();
				if (!wordWrap)
					setFlag(WIDGET_NOWRAP);
				return true;
			}
		}
		return false;
	}

	void Label::parseFont(Lexer* src, Font* out)
	{
		Token tok;
		int fontSize = 0;
		Token tok2;
		if (!src->expectTokenType(TOKENTYPE_STRING, 0, &tok)) {
			Engine::printError("Could not read the expected font id value");
			return;
		}
		tok2 = tok;
		src->expectAnyToken(&tok);
		if (String::iCompare(tok, "fontSize")) {
			src->readToken(&tok);
			fontSize = atoi(tok);
		}

		out = resourceManager->loadFont(tok2, fontSize);
		return;
	}
}