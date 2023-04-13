#include "ButtonWidget.h"
#include "WidgetEnums.h"
#include "../renderer/FontManager.h"

namespace Arboria {
	
	ButtonWidget::ButtonWidget(const String& labelName, Font* font) : Widget(), hover(false), textVAlign(VerticalAlignment::VERT_CENTER), textHAlign(HorizontalAlignment::HOR_CENTER) {
		if (font == NULL) {
			font == fontManager->getFont("logotypegothicregular", "", 24);
		}
		label = new Label(labelName, font);
		clickable = true;
	}

	ButtonWidget::~ButtonWidget() = default;

	void ButtonWidget::run() {}

	void ButtonWidget::eventOccured(Event* e)
	{
		Widget::eventOccured(e);

		//This will be important when the audio backend is listed, as this will fire a sound effect and still allow the parent ui elements to process the interaction event.
		/*if (e->getEventType() == EventType::EVENT_UI_INTERACTION && hasFocus) {
			auto widgetEvent = dynamic_cast<WidgetEvent*>(e);
			auto keyboardEventData = widgetEvent->getData().keyboardData;
			InputActionType action = engine->getInputManager()->getActionTranslation(&keyboardEventData);
			if (action == InputActionType::CONFIRM) {
				//play confirm sound
			}
			if (action == InputActionType::CANCEL) {
				//play cancel sound
			}
			if (action == InputActionType::LEFT || action == InputActionType::RIGHT
				|| action == InputActionType::UP || action == InputActionType::DOWN) {
				//play directional sound
			}
		}*/
	}

	void ButtonWidget::onRender()
	{
		Widget::onRender();

		if (label->getParent() == NULL) {
			label->setParent(this);
			label->size = this->size;
			label->halign = textHAlign;
			label->valign = textVAlign;
		}


	}
}