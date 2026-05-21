#include "Session.h"
#include "../game/scenes/MainMenu.h"
#include "../gui/Window.h"
#include "CommandSystem.h"
#include "InputManager.h"

namespace Arboria {

	Session* session = new Session();

	Session::~Session()
	{
	}

	Session::Session()
	{
		mainMenu = NULL;
		guiActive = NULL;
		lastInputDevice = 0;
		activeScene = 0;
	}

	void Session::init() {
		mainMenu = dynamic_cast<MainMenu*>(guiManager->findWindow(String{"mainmenu.gui"}, true, false));
		guiActive = NULL;
		activeScene = 0;
	}

	void Session::update()
	{
		if (guiActive) {
			return;
		}
	}

	void Session::render() {
		if (guiActive && !guiActive->isGameGui) {
			guiActive->render();
		}
	}

	bool Session::processEvent(AEvent* ev)
	{
		if (guiActive != NULL) {
			handleMenuEvent(ev);
			return ev->isHandled;
		}
		return false;
	}
}