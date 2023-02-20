#include "MainMenu.h"
#include "../../gui/ButtonWidget.h"
#include "../../globals.h"
#include <string>

namespace Arboria {
	MainMenuScreen::MainMenuScreen() : Screen(), currentButtonIndex(0)
	{
		canvas.children.resize(4);
		for (int i = 0; i < 4; i++) {
			ButtonWidget* button = new ButtonWidget("NEW_GAME");
			button->size = { 80, 80 };
			button->setVisible(true);
			button->setClickable(true);
			button->enabled = true;
			button->setParent(&canvas);
		}
	}
	void MainMenuScreen::eventOccured(Event* ev) {

		if (ev->getEventType() == EventType::EVENT_KEY_DOWN) {
			auto keyEvent = dynamic_cast<KeyboardEvent*>(ev);
			InputActionType actionType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_KEYBOARD, keyEvent->getData().keyCode);
			Widget targetButton;
			switch (actionType) {
				case InputActionType::DOWN:
					currentButtonIndex = (currentButtonIndex + 1) & (canvas.children.getLength());
					targetButton = canvas.children[currentButtonIndex];
					targetButton.eventOccured(keyEvent);
					break;
				case InputActionType::UP:
					currentButtonIndex = (currentButtonIndex - 1) & (canvas.children.getLength());
					targetButton = canvas.children[currentButtonIndex];
					targetButton.eventOccured(keyEvent);
					break;
				default:
					canvas.eventOccured(keyEvent);
					break;
			}
		}
		if (ev->getEventType() == EventType::EVENT_BUTTON_DOWN) {
			auto keyEvent = dynamic_cast<ControllerEvent*>(ev);
			InputActionType actionType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_CONTROLLER_BUTTON, keyEvent->getData().button);
			Widget targetButton;
			switch (actionType) {
			case InputActionType::DOWN:
				currentButtonIndex = (currentButtonIndex + 1) & (canvas.children.getLength());
				targetButton = canvas.children[currentButtonIndex];
				targetButton.eventOccured(keyEvent);
				break;
			case InputActionType::UP:
				currentButtonIndex = (currentButtonIndex - 1) & (canvas.children.getLength());
				targetButton = canvas.children[currentButtonIndex];
				targetButton.eventOccured(keyEvent);
				break;
			default:
				targetButton = canvas.children[currentButtonIndex];
				targetButton.eventOccured(keyEvent);
				break;
			}
		}

		if (ev->getEventType() != EventType::EVENT_UI_INTERACTION && ev->getEventType() != EventType::EVENT_BUTTON_DOWN && ev->getEventType() != EventType::EVENT_KEY_DOWN) {
			canvas.eventOccured(ev);
		}

		if (ev->getEventType() == EventType::EVENT_UI_INTERACTION) {
			auto widgetEvent = dynamic_cast<WidgetEvent*>(ev);
			WidgetEventType wType = widgetEvent->getData().guiEventType;
			InputActionType aType = InputActionType::INVALID;
			if (wType == MOUSE_MOVE) {
				currentButtonIndex = canvas.children.findIndex(*(widgetEvent->getData().raisedBy));
				ev->isHandled = true;
				return;
			}

			switch (wType) {
				case WidgetEventType::KEY_DOWN:
				case WidgetEventType::KEY_PRESS:
					aType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_KEYBOARD, widgetEvent->getData().keyboardData.keyCode);
					break;
				case WidgetEventType::BUTTON_DOWN:
				case WidgetEventType::BUTTON_PRESS:
					aType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_CONTROLLER_BUTTON, widgetEvent->getData().controllerData.button);
					break;
				case WidgetEventType::MOUSE_DOWN:
				case WidgetEventType::MOUSE_CLICK:
					aType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_MOUSE, widgetEvent->getData().mouseData.button);
					break;
				default:
					break;
			}
			switch (aType) {
				/*case InputActionType::CONFIRM:
					if (currentButtonIndex == 3 && canvas->children[currentButtonIndex] == widgetEvent->getData().raisedBy) {
						//open options menu
					}
					break;
				*/
				default:
					ev->isHandled = true;
					break;
			}
			//InputActionType aType = engine->getInputManager()->getInputConfiguration().getActionFromInputValue(InputBindType::BIND_KEYBOARD, widgetEvent->getData().keyboardData.keyCode);
		}
		return;
	}
}