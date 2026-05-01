#include "Session.h"
#include "../game/scenes/MainMenu.h"
#include "../game/scenes/OptionsMenu.h"
#include "../gui/GraphicButton.h"
#include "../gui/ListBoxWidget.h"
#include "../gui/Spinner.h"
#include "InputManager.h"
#include "SystemCVars.h"
#include "ResourceManager.h"


namespace Arboria {
	void Session::handleStartMenu() {
		if (guiActive == mainMenu) {
			return;
		}

		setGui(mainMenu);
	}

	void Session::handleMenuEvent(AEvent* ev)
	{
		if (guiActive == NULL) return;
		guiActive->onEvent(ev);
		const char* command = commandSystem->getCommand();
		if (!command || !command[0]) {
			return;
		}

		dispatchCommand(guiActive, command);
	}

	bool Session::lastInputDeviceMatches(const AEvent* e)
	{
		switch (e->eventType) {
		case EventType::EVENT_KEY:
			return lastInputDevice == 0;
		case EventType::EVENT_CONTROLLER_BUTTON:
		case EventType::EVENT_CONTROLLER_AXIS:
			return lastInputDevice == 1;
		default:
			break;
		}
		return false;
	}

	void Session::dispatchCommand(Window* gui, const char* command)
	{
		if (!gui) gui = guiActive;

		if (activeScene == 0) { //TITLE
			handleMainMenuCommands(gui, command);
			return;
		}
	}

	void Session::handleMainMenuCommands(Window* gui, const char* command) {
		int icmd;
		CommandArgs commandArgs;

		commandArgs.tokenizeString(command, false);

		for (icmd = 0; icmd < commandArgs.getArgc();) {
			const char* cmd = commandArgs.getArgv(icmd++);

			if (String::iCompare(command, "quit") == 0) {
				exitMenu();
				_isQuit = true;
				return;
			}
			if (String::iCompare(command, "options") == 0) {
				Window* optionsMenu = guiManager->findWindow(String("optionsmenu.gui"), true);
				if (optionsMenu) {
					guiActive = optionsMenu;
					ListBoxWidget* tabList = guiActive->getRoot()->findWidget<ListBoxWidget>("tabbedList");
					if (tabList) {
						guiActive->getRoot()->setVisibility(true);
						guiActive->getRoot()->setFocus(tabList);
						return;
					}
				}
			}
			if (handleOptionsMenuCommands(commandArgs, icmd)) {
				continue;
			}
		}

	}

	bool Session::handleOptionsMenuCommands(CommandArgs& args, int& icmd)
	{
		const char* cmd = args.getArgv(icmd - 1);

		if (String::iCompare(cmd, "bind") == 0) {
			if (args.getArgc() >= 3) {
				int newKey = atoi(args.getArgv(icmd++));
				int bind = atoi(args.getArgv(icmd++));
				int device = atoi(args.getArgv(icmd++));

				int oldKey = ArboriaKey_Unknown;

				if (device == 0) {
					oldKey = inputManager->getKeyFromAction(bind);
				}
				else {
					oldKey = inputManager->getGamepadButtonFromAction(bind);
				}

				if (oldKey != newKey) {
					inputManager->unbindKeyAction(oldKey, bind);
					inputManager->bindAction(newKey, bind);
				}
				OptionsMenu* optMenu = dynamic_cast<OptionsMenu*>(guiActive);
				if (optMenu) {
					ListBoxWidget* inputScreen = optMenu->getRoot()->findWidget<ListBoxWidget>("bindingOptions");
					Widget* selectedRow = inputScreen->getSelected();
					BindingData* selectedBinding = (BindingData*)selectedRow->getData();
					if (device == 0)
						selectedBinding->key = newKey;
					else
						selectedBinding->gamepad = newKey;

					GraphicButton* optionButton = dynamic_cast<GraphicButton*>(selectedRow->getChildren()[1]);
					if (oldKey != newKey) {
						optionButton->setImage(resourceManager->loadTexture(newKey + 3));
						optionButton->setDirty();
						selectedRow->setDirty();
						inputScreen->setDirty();

						inputManager->saveUserBindings();
					}
					inputScreen->setSelected(NULL);
					optMenu->getRoot()->setFocus(inputScreen);

					return true;
				}
			}
		}
		if (String::iCompare(cmd, "mainMenu") == 0) {
			auto optMenu = dynamic_cast<OptionsMenu*>(guiActive);
			if (optMenu && optMenu->isDisplayChangePending()) {
				optMenu->getRoot()->setVisibility(false);
				optMenu->resetOptions();
				optMenu->setDisplayChangePending(false);
				guiActive = mainMenu;
				return true;
			}
		}
		if (String::iCompare(cmd, "applyDisplaySettings") == 0) {
			ListBoxWidget* optionsList = guiActive->getRoot()->findWidget<ListBoxWidget>("displayOptions");
			if (optionsList) {
				for (auto& row : optionsList->getChildren()) {
					if (row->getName() == "resolutionRow") {
						Spinner* resOptions = row->findWidget<Spinner>("resolutionOptions");
						ResolutionOptions* resData = static_cast<ResolutionOptions*>(resOptions->getSelectedData());
						systemConfig->windowViewportX->setInteger(resData->x);
						systemConfig->windowViewportY->setInteger(resData->y);
						renderDevice->setDisplayX(resData->x);
						renderDevice->setDisplayY(resData->y);
						renderDevice->resize();
						continue;
					}
					if (row->getName() == "vSyncRow") {
						Spinner* vSyncOptions = row->findWidget<Spinner>("vSyncSpinner");
						bool* vSyncer = static_cast<bool*>(vSyncOptions->getSelectedData());
						systemConfig->vSync->setBool(*vSyncer);
						continue;
					}
					else
						break;
				}
				auto optMenu = dynamic_cast<OptionsMenu*>(guiActive);
				optMenu->resetOptions();
				optMenu->setDisplayChangePending(false);
				optionsList->getSelected()->clearFlag(WIDGET_SELECTED);
				optionsList->setSelected(nullptr);
				optMenu->getRoot()->setFocus(optionsList);
				return true;
			}
		}
		if (String::iCompare(cmd, "restoreDefaultDisplay") == 0) {
			auto optMenu = dynamic_cast<OptionsMenu*>(guiActive);
			if (optMenu) {
				optMenu->restoreDefaultSettings(); //restore cvars to default values in this function
				optMenu->setDisplayChangePending(false);
				ListBoxWidget* optionsList = optMenu->getRoot()->findWidget<ListBoxWidget>("displayOptions");
				optionsList->getSelected()->clearFlag(WIDGET_SELECTED);
				optionsList->setSelected(nullptr);
				optMenu->getRoot()->setFocus(optionsList);
				return true;
			}
		}
		if (String::iCompare(cmd, "restoreDefaultBindings") == 0) {
			auto optMenu = dynamic_cast<OptionsMenu*>(guiActive);
			if (optMenu) {
				inputManager->restoreDefaults();
				optMenu->restoreDefaultBindings();
				ListBoxWidget* bindingOptions = guiActive->getRoot()->findWidget<ListBoxWidget>("bindingOptions");
				bindingOptions->getSelected()->clearFlag(WIDGET_SELECTED);
				bindingOptions->setSelected(nullptr);
				optMenu->getRoot()->setFocus(bindingOptions);
				return true;
			}
		}
		return false;
	}
}