#include "TextButton.h"
#include "Label.h"
#include "../renderer/Texture.h"
#include "../renderer/Font.h"
#include "../renderer/RenderData.h"
#include "../renderer/Renderer.h"
#include "../framework/ResourceManager.h"

namespace Arboria {
	TextButton::TextButton(const String& txt, Font* font) : Widget(), buttonBackground(resourceManager->loadTexture(1)),
	textHAlign(HorizontalAlignment::HOR_CENTER), textVAlign(VerticalAlignment::VERT_CENTER), cachedSurface(nullptr) {
		clickable = true;
		label = new Label(txt, font);
		label->setFlag(WidgetStateFlags::WIDGET_ACTIVE | WidgetStateFlags::WIDGET_ENABLED | WidgetStateFlags::WIDGET_VISIBLE);
		label->setPalette(font->palette);
	}

	TextButton::~TextButton() = default;

	bool TextButton::onEvent(AEvent* e)
	{
		Widget::onEvent(e);

		if (e->eventType == EventType::EVENT_UI_INTERACTION && e->guiEvent.raisedBy == this
			&& e->guiEvent.eventType == GUIEventType::KEY_DOWN) {
			submitGuiEvent(GUIEventType::BUTTON_CLICK, e);
		}

		return e->isHandled;
	}

	void TextButton::onRender()
	{
		Widget::onRender();

		/*int y1, y2;
		y1 = 5;
		y2 = 4;*/

		if (label->getParent() == nullptr) {
			label->setParent(this);
			label->size = size;
			label->halign = textHAlign;
			label->valign = textVAlign;
		}

		if (cachedSurface == nullptr || cachedSurface->getSize() != Vector2<unsigned int>(size)) {
			cachedSurface = new Surface(Vector2<unsigned int>(size));

			RendererFrameBufferBinding b(cachedSurface);
			renderer->clear();

			renderer->drawScaled(buttonBackground, Vector2<float>(0, 0), size);
			renderer->drawBorders(Vector2<float>{3, 3}, size - 6, Color{ 100,100,100 });
		}

		renderer->draw(cachedSurface, Vector2<float>{0, 0});
	}

	void TextButton::run()
	{
	}

	void TextButton::unloadResources()
	{
	}

	String TextButton::getText() const {
		return label->getText();
	}

	void TextButton::setText(const String& text) {
		label->setText(text);
	}

	Font* TextButton::getFont() const {
		return label->getFont();
	}

	void TextButton::setFont(Font* font) {
		label->setFont(font);
	}


}