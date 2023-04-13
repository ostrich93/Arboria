#include "MainMenu.h"
#include "../../renderer/Texture.h"
#include "../../framework/InputManager.h"
#include "../../framework/ResourceManager.h"
#include "../../renderer/FontManager.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/ButtonWidget.h"
#include "../../gui/Label.h"

namespace Arboria {
	MainMenuScreen::MainMenuScreen() : Screen()
	{
		/*
			1. build ListBox MAIN_MENU_SELECT, 4 options, set labels as "NEW_GAME", "LOAD_GAME", "OPTIONS", and "QUIT"
			2. set the canvas as listbox's parent.
		*/
		canvas.size = { 800, 640 };
		canvas.setSurface(resourceManager->loadTexture("mainBackground.png"));
		canvas.name = "CANVAS";
		canvas.setVisible(true);
		canvas.enabled = true;
		ListBoxWidget* optionsList = canvas.createChild<ListBoxWidget>();
		optionsList->name = "MAIN_MENU_SELECT";
		optionsList->location = { 320, 400 };
		optionsList->size = { 200, 180 };
		optionsList->itemSize = 40;
		optionsList->itemSpacing = 5;
		
		Font* font = fontManager->getFont("logotypegothicregular", "", 24);

		Texture* bTexture = resourceManager->loadTexture("Button01.png");
		ButtonWidget* newGameButton = new ButtonWidget("NEW_GAME", font);
		newGameButton->setSurface(bTexture);
		ButtonWidget* continueButton = new ButtonWidget("CONTINUE_GAME", font);
		continueButton->setSurface(bTexture);
		ButtonWidget* optionButton = new ButtonWidget("OPTIONS", font);
		optionButton->setSurface(bTexture);
		ButtonWidget* quitButton = new ButtonWidget("QUIT", font);
		quitButton->setSurface(bTexture);

		newGameButton->size = { 200, 40 };
		continueButton->size = { 200, 40 };
		optionButton->size = { 200, 40 };
		quitButton->size = { 200, 40 };

		optionsList->addItem(newGameButton);
		optionsList->addItem(continueButton);
		optionsList->addItem(optionButton);
		optionsList->addItem(quitButton);
		optionsList->enabled = true;
		newGameButton->enabled = true;
		continueButton->enabled = true;
		optionButton->enabled = true;
		quitButton->enabled = true;
	}

	MainMenuScreen::~MainMenuScreen() = default;

	void MainMenuScreen::enter() {}

	void MainMenuScreen::pause() {}

	void MainMenuScreen::resume() {}

	void MainMenuScreen::leave() {}

	void MainMenuScreen::eventOccured(Event* ev) {

		canvas.eventOccured(ev);

		if (ev->getEventType() == EventType::EVENT_UI_INTERACTION) {
			auto widgetEvent = dynamic_cast<WidgetEvent*>(ev);
			KeyboardEventData keyEventData = widgetEvent->getData().keyboardData;
			if (widgetEvent->getData().raisedBy->name == "MAIN_MENU_SELECT") {
				ListBoxWidget* target = canvas.findWidget<ListBoxWidget>(widgetEvent->getData().raisedBy->name);
				if (widgetEvent->getData().guiEventType == WidgetEventType::LIST_BOX_CHANGE_HOVER) {
					//play cursor move sound when audio is implemented. For now, just print to the console.
					printf("Changed index to %d", target->cursor);
					ev->isHandled = true;
				}
				else if (widgetEvent->getData().guiEventType == WidgetEventType::LIST_BOX_CHANGE_SELECT) {
					if (target->getHovered()->enabled) {
						target->setSelected(target->getHovered());
						//play select sound
						printf("Selected %d", target->cursor);
						//when options menu is screen is made, send screen command to place it on top of the screen stack

						//when quit is selected, exit the game
						if (target->cursor == 3) {
							_isQuit = true;
						}
					}
					else {
						//play cancel sound
						printf("Can not select %d because it is disabled!", target->cursor);
					}
					ev->isHandled = true;
				}
				else if (widgetEvent->getData().guiEventType == WidgetEventType::LIST_BOX_CHANGE_CANCEL) {
					//if hovered target is not selected, do nothing
					
					//if hovered target is selected, cancel it
					if (target->getSelected() != NULL) {
						//play cancel sound
						printf("Option %d in list is not chosen", target->cursor);
					}
					ev->isHandled = true;
				}
			}
		}
		return;
	}

	void MainMenuScreen::run() { canvas.run(); }

	void MainMenuScreen::draw() { canvas.render(); }

	bool MainMenuScreen::isTransitioning() const { return false; }
}