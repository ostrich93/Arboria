#include "MainMenu.h"
#include "../../framework/CommandSystem.h"
#include "../../framework/InputManager.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/TextButton.h"

namespace Arboria {
	MainMenu::MainMenu() : Window()
	{
		
	}

	MainMenu::~MainMenu() = default;

	void MainMenu::onEvent(AEvent* ev)
	{
		root->onEvent(ev);
	}

	bool MainMenu::postParse()
	{
		ListBoxWidget* mainMenuSelect = root->findWidget<ListBoxWidget>("mainMenuSelect");
		if (!mainMenuSelect)
			return false;
		setCursor(mainMenuSelect->position.x, mainMenuSelect->position.y);
		TextButton* optionsButton = mainMenuSelect->findWidget<TextButton>("optionsButton");
		if (!optionsButton)
			return false;
		TextButton* quitButton = mainMenuSelect->findWidget<TextButton>("quitButton");
		if (!quitButton)
			return false;
		root->setFocus(mainMenuSelect);
		optionsButton->addCallback(ACTION_CONFIRM, pushOptionsMenu);
		quitButton->addCallback(ACTION_CONFIRM, setQuit);

		return true;
	}


	void MainMenu::render() { root->render(); }


	bool pushOptionsMenu(Widget* w, AEvent* e) {
		commandSystem->pushCommand("options");
		return true;
	}

	bool setQuit(Widget* w, AEvent* e) {
		commandSystem->pushCommand("quit");
		return true;
	}
}