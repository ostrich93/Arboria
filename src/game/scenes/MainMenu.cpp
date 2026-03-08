#include "MainMenu.h"
#include "OptionsMenu.h"
#include "../../framework/ScreenManager.h"
#include "../../renderer/Texture.h"
#include "../../framework/InputManager.h"
#include "../../framework/ResourceManager.h"
#include "../../renderer/Font.h"
#include "../../gui/Graphic.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/Label.h"
#include "../../gui/TextButton.h"

namespace Arboria {
	MainMenuScreen::MainMenuScreen() : UIContext()
	{
		/*
			1. build ListBox MAIN_MENU_SELECT, 4 options, set labels as "NEW_GAME", "LOAD_GAME", "OPTIONS", and "QUIT"
			2. set the canvas as listbox's parent.
		*/
		
		root.position = { 0, 0 };
		root.size = { 640, 360 };
		root.name = "ROOT";
		root.setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_MENU);
		root.setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		root.setFlag(WidgetStateFlags::WIDGET_CONTAINER);
		root.enabled = true;

		Graphic* canvas = root.createChild<Graphic>(resourceManager->loadTexture(0));
		canvas->enabled = true;
		canvas->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_MENU
			| WidgetStateFlags::WIDGET_CONTAINER);
		canvas->size = { 640, 360 };
		canvas->enabled = true;

		Font* font = resourceManager->loadFont("logotypegothicregular.ttf", 24);

		Label* titleLabel = canvas->createChild<Label>("ARBORIA ENGINE", font);
		titleLabel->position = { 100 , 200 };
		titleLabel->size = { 48, 48 };
		titleLabel->enabled = true;
		titleLabel->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_MENU);
		
		Label* versionLabel = canvas->createChild<Label>("V.1.0", font);
		versionLabel->position = { 100 , 270 };
		versionLabel->size = { 48, 48 };
		versionLabel->enabled = true;
		versionLabel->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_MENU);

		ListBoxWidget* optionsList = root.createChild<ListBoxWidget>(4, 1);
		optionsList->name = "MAIN_MENU_SELECT";
		optionsList->position = { 320, 400 };
		optionsList->size = { 200, 200 };
		optionsList->itemSize = 40;
		optionsList->itemSpacing = 5;
		optionsList->enabled = true;
		optionsList->setBackgroundColor(*new Color( 20, 45, 210, 192));
		optionsList->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		optionsList->setFlag(WidgetStateFlags::WIDGET_MENU);
		optionsList->setFlag(WidgetStateFlags::WIDGET_NOBORDER);
		optionsList->setFlag(WidgetStateFlags::WIDGET_FOCUSED);
		

		TextButton* newGameButton = new TextButton("NEW GAME", font);
		newGameButton->size = { 200, 48 };
		newGameButton->position = { 0, 0 };
		newGameButton->name = "NEW_GAME_BUTTON";
		newGameButton->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);

		TextButton* continueButton = new TextButton("CONTINUE", font);
		continueButton->size = { 200, 48 };
		continueButton->position = { 0, 0 };
		continueButton->name = "CONTINUE_BUTTON";
		continueButton->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		
		TextButton* optionButton = new TextButton("OPTIONS", font);
		optionButton->size = { 200, 48 };
		optionButton->position = { 0, 0 };;
		optionButton->name = "OPTION_BUTTON";
		optionButton->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		optionButton->addEventCallback(GUIEventType::BUTTON_CLICK, pushOptionsMenu);
		//optionButton->addCallback(1, &pushOptionsMenu);
		
		TextButton* quitButton = new TextButton("QUIT", font);
		quitButton->size = { 200, 48 };
		quitButton->position = { 0, 0 };
		quitButton->name = "QUIT_BUTTON";
		quitButton->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		quitButton->addEventCallback(GUIEventType::BUTTON_CLICK, setQuit);
		//quitButton->addCallback(1, &setQuit);

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

	bool MainMenuScreen::onEvent(AEvent* ev)
	{
		return root.onEvent(ev);
	}

	void MainMenuScreen::run() { root.run(); }

	void MainMenuScreen::draw() { root.render(); }

	bool MainMenuScreen::isTransitioning() const { return false; }

	void pushOptionsMenu(AEvent* e, void* context) {
		screenManager->push(new OptionsMenu());
	}

	void setQuit(AEvent* e, void* context) {
		_isQuit = true;
	}
}