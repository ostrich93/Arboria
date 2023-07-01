#include "MainMenu.h"
#include "OptionsMenu.h"
#include "../../framework/ScreenManager.h"
#include "../../renderer/Texture.h"
#include "../../framework/InputManager.h"
#include "../../framework/ResourceManager.h"
#include "../../renderer/Font.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/Label.h"

namespace Arboria {
	MainMenuScreen::MainMenuScreen() : UIContext()
	{
		/*
			1. build ListBox MAIN_MENU_SELECT, 4 options, set labels as "NEW_GAME", "LOAD_GAME", "OPTIONS", and "QUIT"
			2. set the canvas as listbox's parent.
		*/
		canvas.rect = { 0, 0, 800, 640 };
		canvas.setSurface(resourceManager->loadTexture("mainBackground.png"));
		canvas.name = "CANVAS";
		canvas.setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_MENU | WidgetStateFlags::WIDGET_ACTIVE);
		canvas.setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		canvas.setFlag(WidgetStateFlags::WIDGET_CONTAINER);
		canvas.enabled = true;

		ListBoxWidget* optionsList = canvas.createChild<ListBoxWidget>(4, 1);
		optionsList->name = "MAIN_MENU_SELECT";
		optionsList->rect = { 320, 400, 200, 180 };
		optionsList->itemSize = 40;
		optionsList->itemSpacing = 5;
		optionsList->setBackgroundColor(*new Color( 20, 45, 210, 192));
		optionsList->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		optionsList->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		optionsList->setFlag(WidgetStateFlags::WIDGET_MENU);
		optionsList->setFlag(WidgetStateFlags::WIDGET_NOBORDER);
		optionsList->setFlag(WidgetStateFlags::WIDGET_FOCUSED);
		
		Font* font = fontManager->getFont("logotypegothicregular", "", 24);

		Texture* bTexture = resourceManager->loadTexture("Button01.png");
		Widget* newGameButton = new Widget();
		newGameButton->rect = { 0, 0, 200, 40 };
		newGameButton->setSurface(bTexture);
		newGameButton->name = "NEW_GAME_BUTTON";
		newGameButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_ACTIVE);
		Label* newGameLabel = newGameButton->createChild<Label>("NEW_GAME", font);
		newGameLabel->tint = { (uint8_t)30, (uint8_t)100, (uint8_t)15, (uint8_t)255 };
		newGameLabel->setFlag(WidgetStateFlags::WIDGET_VISIBLE);

		Widget* continueButton = new Widget();
		continueButton->rect = { 0, 0, 200, 40 };
		continueButton->setSurface(bTexture);
		continueButton->name = "CONTINUE_BUTTON";
		continueButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_ACTIVE);
		Label* continueLabel = continueButton->createChild<Label>("CONTINUE_GAME", font);
		Widget* optionButton = new Widget();
		optionButton->rect = { 0, 0, 200, 40 };
		optionButton->setSurface(bTexture);
		optionButton->name = "OPTION_BUTTON";
		optionButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_ACTIVE);
		optionButton->addCallback(1, &pushOptionsMenu);
		Label* optionLabel = optionButton->createChild<Label>("OPTIONS", font);
		Widget* quitButton = new Widget();
		quitButton->rect = { 0, 0, 200, 40 };
		quitButton->setSurface(bTexture);
		quitButton->name = "QUIT_BUTTON";
		quitButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_ACTIVE);
		quitButton->addCallback(1, &setQuit);
		Label* quitLabel = quitButton->createChild<Label>("QUIT", font);

		optionsList->addItem(newGameButton);
		optionsList->addItem(continueButton);
		optionsList->addItem(optionButton);
		optionsList->addItem(quitButton);
	}

	MainMenuScreen::~MainMenuScreen() = default;

	void MainMenuScreen::enter() {}

	void MainMenuScreen::pause() {}

	void MainMenuScreen::resume() {}

	void MainMenuScreen::leave() {}

	//void MainMenuScreen::eventOccured(SystemEvent* ev) {

	//	canvas.eventOccured(ev);

	//	if (ev->type == EventType::EVENT_UI_INTERACTION) {
	//		auto widgetEventData = dynamic_cast<GuiEventData*>(ev->data);
	//		//KeyboardEventData keyEventData = widgetEventData->keyboardData;
	//		if (widgetEventData == NULL) {
	//			printf("The GuiEventData stored was lost. This is likely a bug.\n");
	//			ev->isHandled = true;
	//		}
	//		else if (widgetEventData != NULL && widgetEventData->raisedBy->name == "MAIN_MENU_SELECT") {
	//			ListBoxWidget* target = canvas.findWidget<ListBoxWidget>(widgetEventData->raisedBy->name);
	//			if (widgetEventData->guiEventType == WidgetEventType::LIST_BOX_CHANGE_HOVER) {
	//				//play cursor move sound when audio is implemented. For now, just print to the console.
	//				printf("Changed index to %d", target->cursor);
	//				ev->isHandled = true;
	//			}
	//			else if (widgetEventData->guiEventType == WidgetEventType::LIST_BOX_CHANGE_SELECT) {
	//				if (target->getHovered()->enabled) {
	//					target->setSelected(target->getHovered());
	//					//play select sound
	//					printf("Selected %d", target->cursor);
	//					//when options menu is screen is made, send screen command to place it on top of the screen stack

	//					//when quit is selected, exit the game
	//					if (target->cursor == 3) {
	//						_isQuit = true;
	//					}
	//				}
	//				else {
	//					//play cancel sound
	//					printf("Can not select %d because it is disabled!", target->cursor);
	//				}
	//				ev->isHandled = true;
	//			}
	//			else if (widgetEventData->guiEventType == WidgetEventType::LIST_BOX_CHANGE_CANCEL) {
	//				//if hovered target is not selected, do nothing
	//				
	//				//if hovered target is selected, cancel it
	//				if (target->getSelected() != NULL) {
	//					//play cancel sound
	//					printf("Option %d in list is not chosen", target->cursor);
	//				}
	//				ev->isHandled = true;
	//			}
	//		}
	//	}
	//	return;
	//}

	bool MainMenuScreen::onEvent(AEvent* ev)
	{
		return canvas.onEvent(ev);
	}

	void MainMenuScreen::run() { canvas.run(); }

	void MainMenuScreen::draw() { canvas.render(); }

	bool MainMenuScreen::isTransitioning() const { return false; }

	void pushOptionsMenu(Widget* w) {
		screenManager->push(new OptionsMenu());
	}

	void setQuit(Widget* w) {
		_isQuit = true;
	}
}