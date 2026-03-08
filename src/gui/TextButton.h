#ifndef TEXT_BUTTON_H
#define TEXT_BUTTON_H

#include "Widget.h"

namespace Arboria {
	class Font;
	class Label;
	class Image;

	class TextButton : public Widget {
	private:
		Label* label;
		Surface* cachedSurface;

		Image* buttonBackground;
	public:
		HorizontalAlignment textHAlign;
		VerticalAlignment textVAlign;

		TextButton(const String& txt = "", Font* font = nullptr);
		~TextButton() override;

		bool onEvent(AEvent* e) override;
		void onRender() override;
		void run() override;
		void unloadResources() override;

		String getText() const;
		void setText(const String& text);

		Font* getFont() const;
		void setFont(Font* font);
	};
}

#endif