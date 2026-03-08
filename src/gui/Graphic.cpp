#include "../renderer/Renderer.h"
#include "../renderer/Texture.h"
#include "Graphic.h"

namespace Arboria {
	Graphic::Graphic(Image* img) : Widget(), image(img), hAlign(HorizontalAlignment::HOR_LEFT),
		vAlign(VerticalAlignment::VERT_TOP), imageFill(FillMethod::FIT), autoSize(false)
	{
	}

	Graphic::~Graphic() = default;

	void Graphic::run()
	{
		Widget::run();
		if (image && autoSize) {
			size = image->getSize();
		}
	}

	void Graphic::onRender()
	{
		Widget::onRender();

		if (!image)
			return;

		Vector2<float> pos = { 0, 0 };
		if (Vector2<unsigned int>(size) == image->getSize()) {
			renderer->draw(image, pos);
		}
		else {
			if (imageFill == FillMethod::STRETCH) {
				renderer->drawScaled(image, pos, size);
			}
			else if (imageFill == FillMethod::FIT) {
				switch (vAlign) {
					case VerticalAlignment::VERT_TOP:
						pos.y = 0;
						break;
					case VerticalAlignment::VERT_CENTER:
						pos.y = (size.y - image->getHeight())/2;
						break;
					case VerticalAlignment::VERT_BOTTOM:
						pos.y = size.y - image->getHeight();
						break;
					default:
						break;
				}
				switch (hAlign) {
					case HorizontalAlignment::HOR_LEFT:
						pos.x = 0;
						break;
					case HorizontalAlignment::HOR_CENTER:
						pos.x = (size.x - image->getWidth()) / 2;
						break;
					case HorizontalAlignment::HOR_RIGHT:
						pos.x = size.x - image->getWidth();
						break;
					default:
						break;
				}

				renderer->draw(image, pos);
			}
		}
	}

	bool Graphic::onEvent(AEvent* e)
	{
		return Widget::onEvent(e);
	}

	void Graphic::unloadResources()
	{
		image = nullptr;
		Widget::unloadResources();
	}

	Image* Graphic::getImage() const
	{
		return image;
	}

	void Graphic::setImage(Image* img)
	{
		image = img;
	}


}