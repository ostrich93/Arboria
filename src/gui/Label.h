#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"
#include "../framework/String.h"
#include "../renderer/Font.h"

namespace Arboria {
	class Label : public Widget {
		private:
			String text;
			Font* font;
		public:
			VerticalAlignment valign;
			HorizontalAlignment halign;
			Color tint{ 255, 255, 255, 255 };
			float textScale = 1.0f;
			//font stuff

			Label(Window* gui, const String& textContent = "", Font* _font = NULL);
			~Label() override;
			bool onEvent(AEvent* e) override;
			void onRender() override;
			void run() override;

			String getText() const;
			void setText(String& _text);
			void setText(const char* _text);

			Font* getFont() const;
			void setFont(Font* _font);

			bool parseInternalValue(const char* _name, Lexer* src) override;
			void parseFont(Lexer* src);
	};
}

#endif