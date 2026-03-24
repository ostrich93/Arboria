#ifndef GRAPHIC_WIDGET_H
#define GRAPHIC_WIDGET_H

#include "Widget.h"

namespace Arboria {
	class Image;
	class Graphic : public Widget {
	private:
		Image* image;
	public:
		HorizontalAlignment hAlign;
		VerticalAlignment vAlign;
		FillMethod imageFill;
		bool autoSize;
		Graphic(Image* img = nullptr);
		~Graphic() override;

		void run() override;
		void onRender() override;
		bool onEvent(AEvent* e) override;
		void unloadResources() override;

		Image* getImage() const;
		void setImage(Image* img);
	};
}

#endif