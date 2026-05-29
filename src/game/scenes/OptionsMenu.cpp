#include "OptionsMenu.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/Label.h"
#include "../../gui/Graphic.h"
#include "../../gui/GraphicButton.h"
#include "../../gui/TextButton.h"
#include "../../gui/Spinner.h"
#include "../../framework/ResourceManager.h"
#include "../../framework/InputManager.h"
#include "../../framework/Lexer.h"
#include "../../framework/Session.h"
#include "../../framework/SystemCVars.h"

namespace Arboria {
	
	struct WindowModeOption {
		String optionText;
		uint32_t windowFlags;

		WindowModeOption() : optionText(""), windowFlags(0) {}
		WindowModeOption(const char* text, uint32_t _f) : optionText(text), windowFlags(_f) {}
	};

	struct WindowModeSelectData {
		List<WindowModeOption> modeOptions;
		uint32_t modeIndex;

		WindowModeSelectData() : modeIndex(0) {}
	};

	const List<ResolutionOptions> resolutionData = {
		{"640x480", 640, 480},
		{"800x600", 800, 600},
		{"1024x768", 1024, 768},
		{"1280x960", 1280, 960}
	};

	const List<bool> vsyncOptionData = {
		true, false
	};

	void OptionsMenu::setKeyImageFromAction(int action, GraphicButton* bindBtn)
	{
		int key = inputManager->getKeyFromAction(action);
		if (key > 0 && key < ArboriaKey_RGui) {
			bindBtn->setImage(resourceManager->loadTexture(key + 3));
		}
	}

	void OptionsMenu::setButtonImageFromAction(int action, GraphicButton* bindBtn) {
		int btn = inputManager->getGamepadButtonFromAction(action);
	}

	OptionsMenu::OptionsMenu() : Window()
	{
		selectTab = 0;
		inBindWindow = false;
		waitingBind = false;

		currentWindow = NULL;
	}

	OptionsMenu::~OptionsMenu() = default;

	bool OptionsMenu::postParse() //parse the options list after getting options string
	{
		tabbedList = root->findWidget<ListBoxWidget>("tabbedList");
		tabbedWindows[0] = root->findWidget<Widget>("displayScreen");
		tabbedWindows[1] = root->findWidget<Widget>("inputScreen");

		Widget* resolutionWindow = tabbedWindows[0]->findWidget<ListBoxWidget>("displayOptions")->getChild(0);
		Spinner* resOptionSpinner = resolutionWindow->findWidget<Spinner>("resolutionOptions");
		if (!resOptionSpinner)
			return false;

		setCursor(tabbedList->getChild(0)->position.x, tabbedList->getChild(0)->position.y);

		Widget* vSyncRow = tabbedWindows[0]->findWidget<ListBoxWidget>("displayOptions")->getChild(1);
		Spinner* vSyncSpinner = vSyncRow->findWidget<Spinner>("vsyncSpinner");
		if (!vSyncSpinner)
			return false;

		parseSpinnerOptions<ResolutionOptions>(resOptionSpinner, resolutionData);
		parseSpinnerOptions<bool>(vSyncSpinner, vsyncOptionData);

		resOptionSpinner->addCallback(ACTION_CANCEL, returnToDisplayList);
		vSyncSpinner->addCallback(ACTION_CANCEL, returnToDisplayList);

		tabbedList->addCallback(ACTION_CANCEL, exitOptionsMenu);
		tabbedList->addCallback(ACTION_LEFTARROW, handleSelectedTabChange);
		tabbedList->addCallback(ACTION_RIGHTARROW, handleSelectedTabChange);

		ListBoxWidget* displayOptions = tabbedWindows[0]->findWidget<ListBoxWidget>("displayOptions");
		displayOptions->addCallback(ACTION_CANCEL, handleReturnToTabList);
		ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		bindingOptions->addCallback(ACTION_CANCEL, handleReturnToTabList);

		Widget* applyRow = displayOptions->findWidget<Widget>("applyRow");
		TextButton* applyButton = applyRow->findWidget<TextButton>("applyButton");
		applyButton->addCallback(ACTION_CONFIRM, handleApplyDisplayChanges);
		Widget* restoreDisplayRow = displayOptions->findWidget<Widget>("restoreDisplayRow");
		TextButton* defaultSettingsButton = restoreDisplayRow->findWidget<TextButton>("defaultDisplayButton");
		defaultSettingsButton->addCallback(ACTION_CONFIRM, handleRestoreDefaultDisplaySettings);

		Widget* confirmRow = bindingOptions->findWidget<Widget>("confirmRow");
		confirmRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_CONFIRM), inputManager->getGamepadButtonFromAction(ACTION_CONFIRM), ACTION_CONFIRM });
		confirmRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* cancelRow = bindingOptions->findWidget<Widget>("cancelRow");
		cancelRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_CANCEL), inputManager->getGamepadButtonFromAction(ACTION_CANCEL), ACTION_CANCEL });
		cancelRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* unitMenuRow = bindingOptions->findWidget<Widget>("unitMenuRow");
		unitMenuRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_TOGGLE_DISPLAY), inputManager->getGamepadButtonFromAction(ACTION_TOGGLE_DISPLAY), ACTION_TOGGLE_DISPLAY });
		unitMenuRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* helpRow = bindingOptions->findWidget<Widget>("helpRow");
		helpRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_HELP), inputManager->getGamepadButtonFromAction(ACTION_HELP), ACTION_HELP });
		helpRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* lShiftRow = bindingOptions->findWidget<Widget>("leftShiftRow");
		lShiftRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_L_TRIGGER_1), inputManager->getGamepadButtonFromAction(ACTION_L_TRIGGER_1), ACTION_L_TRIGGER_1 });
		lShiftRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* rShiftRow = bindingOptions->findWidget<Widget>("rightShiftRow");
		rShiftRow->setData(new BindingData{ inputManager->getKeyFromAction(ACTION_R_TRIGGER_1), inputManager->getGamepadButtonFromAction(ACTION_R_TRIGGER_1), ACTION_R_TRIGGER_1 });
		rShiftRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* restoreDefaultBindingsRow = bindingOptions->findWidget<Widget>("restoreDefaultBindingsRow");
		TextButton* restoreDefaultBindingsButton = restoreDefaultBindingsRow->findWidget<TextButton>("restoreDefaultBindingsButton");
		restoreDefaultBindingsButton->addCallback(ACTION_CONFIRM, handleRestoreDefaultBindings);

		return true;
	}

	bool OptionsMenu::parseBindings()
	{
		ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		GraphicButton* confirmOption = bindingOptions->getChild(0)->findWidget<GraphicButton>("confirmOption");
		if (!confirmOption)
			return false;
		GraphicButton* cancelOption = bindingOptions->getChild(1)->findWidget<GraphicButton>("cancelOption");
		if (!cancelOption)
			return false;
		GraphicButton* unitMenuOption = bindingOptions->getChild(2)->findWidget<GraphicButton>("unitMenuOption");
		if (!unitMenuOption)
			return false;
		GraphicButton* helpOption = bindingOptions->getChild(3)->findWidget<GraphicButton>("helpOption");
		if (!helpOption)
			return false;
		GraphicButton* leftShiftOption = bindingOptions->getChild(4)->findWidget<GraphicButton>("leftShiftOption");
		if (!leftShiftOption)
			return false;
		GraphicButton* rightShiftOption = bindingOptions->getChild(5)->findWidget<GraphicButton>("rightShiftOption");
		if (!rightShiftOption)
			return false;
		TextButton* defaultBindingsButton = bindingOptions->getChild(6)->findWidget<TextButton>("restoreDefaultBindingsButton");
		if (!defaultBindingsButton)
			return false;

		if (!session->getLastInputDevice()) { //if keyboard input
			setKeyImageFromAction(ACTION_CONFIRM, confirmOption);
			setKeyImageFromAction(ACTION_CANCEL, cancelOption);
			setKeyImageFromAction(ACTION_TOGGLE_DISPLAY, unitMenuOption);
			setKeyImageFromAction(ACTION_HELP, helpOption);
			setKeyImageFromAction(ACTION_L_TRIGGER_1, leftShiftOption);
			setKeyImageFromAction(ACTION_R_TRIGGER_1, rightShiftOption);
		}
		else {
			//setButtonImageFromAction(ACTION_CONFIRM, confirmOption);
		}

		//set action bindings here

		return false;
	}

	void OptionsMenu::render() { root->render(); }

	void OptionsMenu::onEvent(AEvent* e) {
		if (session->isBindWait) {
			tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions")->getSelected()->executeCallback(ACTION_CONFIRM, e);
		}

		Window::onEvent(e);
	}

	void OptionsMenu::resetOptions()
	{
		ResolutionOptions actualResolution{ "", systemConfig->windowViewportX->getInteger(), systemConfig->windowViewportY->getInteger() };
		Spinner* resolutionSpinner = tabbedWindows[0]->getChild(0)->findWidget<Spinner>("resolutionOptions");
		List<SpinnerOption>& spinnerOptions = resolutionSpinner->getOptions();

		for (int i = 0; i < spinnerOptions.getLength(); i++) {
			ResolutionOptions* resolutionData = (ResolutionOptions*)spinnerOptions[i].optionData;
			if (resolutionData->x == actualResolution.x && resolutionData->y == actualResolution.y) {
				resolutionSpinner->setSelectedOption(i);
				break;
			}
		}

		Spinner* vsyncSpinner = tabbedWindows[0]->getChild(0)->findWidget<Spinner>("vsyncSpinner");
		List<SpinnerOption>& vsyncOptions = vsyncSpinner->getOptions();
		for (int j = 0; j < vsyncOptions.getLength(); j++) {
			bool vsyncData = *(bool*)vsyncOptions[j].optionData;
			if (vsyncData == systemConfig->vSync->getBool()) {
				vsyncSpinner->setSelectedOption(j);
				break;
			}
		}
	}

	void OptionsMenu::restoreDefaultSettings() {
		ResolutionOptions defaultResolution{ "", systemConfig->defaultWindowViewportX->getInteger(), systemConfig->defaultFullscreenWindowViewportY->getInteger() };
		Spinner* resolutionSpinner = tabbedWindows[0]->getChild(0)->findWidget<Spinner>("resolutionOptions");
		List<SpinnerOption>& spinnerOptions = resolutionSpinner->getOptions();

		for (int i = 0; i < spinnerOptions.getLength(); i++) {
			ResolutionOptions* resolutionData = (ResolutionOptions*)spinnerOptions[i].optionData;
			if (resolutionData->x == defaultResolution.x && resolutionData->y == defaultResolution.y) {
				resolutionSpinner->setSelectedOption(i);
				break;
			}
		}

		Spinner* vsyncSpinner = tabbedWindows[0]->getChild(1)->findWidget<Spinner>("vsyncSpinner");
		List<SpinnerOption>& vsyncOptions = vsyncSpinner->getOptions();
		for (int j = 0; j < vsyncOptions.getLength(); j++) {
			bool vsyncData = *(bool*)vsyncOptions[j].optionData;
			if (vsyncData == systemConfig->defaultVSync->getBool()) {
				vsyncSpinner->setSelectedOption(j);
				break;
			}
		}
		systemConfig->updateConfigFile();
	}

	void OptionsMenu::restoreDefaultBindings()
	{
		ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		
		Widget* confirmRow = bindingOptions->findWidget<Widget>("confirmRow");
		int confirmKey = inputManager->getKeyFromAction(ACTION_CONFIRM);
		int confirmButton = inputManager->getGamepadButtonFromAction(ACTION_CONFIRM);
		BindingData* confirmBind = (BindingData*)confirmRow->getData();
		confirmBind->key = confirmKey;
		confirmBind->gamepad = confirmButton;
		GraphicButton* confirmOption = dynamic_cast<GraphicButton*>(confirmRow->getChild(1));
		confirmOption->setImage(resourceManager->loadTexture(confirmKey + 3));
		confirmOption->setDirty();

		Widget* cancelRow = bindingOptions->findWidget<Widget>("cancelRow");
		int cancelKey = inputManager->getKeyFromAction(ACTION_CANCEL);
		int cancelButton = inputManager->getGamepadButtonFromAction(ACTION_CANCEL);
		BindingData* cancelBind = (BindingData*)cancelRow->getData();
		cancelBind->key = cancelKey;
		cancelBind->gamepad = cancelButton;
		GraphicButton* cancelOption = dynamic_cast<GraphicButton*>(cancelRow->getChild(1));
		cancelOption->setImage(resourceManager->loadTexture(cancelKey + 3));
		cancelOption->setDirty();

		Widget* unitMenuRow = bindingOptions->findWidget<Widget>("unitMenuRow");
		int unitMenuKey = inputManager->getKeyFromAction(ACTION_TOGGLE_DISPLAY);
		int unitMenuButton = inputManager->getGamepadButtonFromAction(ACTION_TOGGLE_DISPLAY);
		BindingData* unitMenuBind = (BindingData*)unitMenuRow->getData();
		unitMenuBind->key = unitMenuKey;
		unitMenuBind->gamepad = unitMenuButton;
		GraphicButton* unitMenuOption = dynamic_cast<GraphicButton*>(unitMenuRow->getChild(1));
		unitMenuOption->setImage(resourceManager->loadTexture(unitMenuKey + 3));
		unitMenuOption->setDirty();

		Widget* helpRow = bindingOptions->findWidget<Widget>("helpRow");
		int helpKey = inputManager->getKeyFromAction(ACTION_HELP);
		int helpButton = inputManager->getGamepadButtonFromAction(ACTION_HELP);
		BindingData* helpBind = (BindingData*)helpRow->getData();
		helpBind->key = helpKey;
		helpBind->gamepad = helpButton;
		GraphicButton* helpOption = dynamic_cast<GraphicButton*>(helpRow->getChild(1));
		helpOption->setImage(resourceManager->loadTexture(helpKey + 3));
		helpOption->setDirty();

		Widget* leftShiftRow = bindingOptions->findWidget<Widget>("leftShiftRow");
		int leftShiftKey = inputManager->getKeyFromAction(ACTION_L_TRIGGER_1);
		int leftShiftButton = inputManager->getGamepadButtonFromAction(ACTION_L_TRIGGER_1);
		BindingData* leftShiftBind = (BindingData*)leftShiftRow->getData();
		leftShiftBind->key = leftShiftKey;
		leftShiftBind->gamepad = leftShiftButton;
		GraphicButton* leftShiftOption = dynamic_cast<GraphicButton*>(leftShiftRow->getChild(1));
		leftShiftOption->setImage(resourceManager->loadTexture(leftShiftKey + 3));
		leftShiftOption->setDirty();

		Widget* rightShiftRow = bindingOptions->findWidget<Widget>("rightShiftRow");
		int rightShiftKey = inputManager->getKeyFromAction(ACTION_R_TRIGGER_1);
		int rightShiftButton = inputManager->getGamepadButtonFromAction(ACTION_R_TRIGGER_1);
		BindingData* rightShiftBind = (BindingData*)rightShiftRow->getData();
		rightShiftBind->key = rightShiftKey;
		rightShiftBind->gamepad = rightShiftButton;
		GraphicButton* rightShiftOption = dynamic_cast<GraphicButton*>(rightShiftRow->getChild(1));
		rightShiftOption->setImage(resourceManager->loadTexture(rightShiftKey + 3));
		rightShiftOption->setDirty();

		bindingOptions->setSelected(NULL);
	}

	bool handleApplyDisplayChanges(Widget* w, AEvent* ev)
	{
		commandSystem->pushCommand("applyDisplaySettings");
		return true;
	}

	bool handleRestoreDefaultDisplaySettings(Widget* w, AEvent* ev)
	{
		commandSystem->pushCommand("restoreDefaultDisplay");
		return true;
	}

	bool handleBindingSelect(Widget* w, AEvent* ev)
	{
		if (!session->isBindWait) {
			session->isBindWait = true;
			return true;
		}
		if (ev->eventType == EventType::EVENT_KEY && (ev->eventValue == ArboriaKey_MouseLeft
			|| ev->eventValue == ArboriaKey_MouseRight)) {
			return true; //if it's a mouse click, don't do anything
		}

		if (!session->lastInputDeviceMatches(ev)) {
			//change bind icons but don't actually set the binding
			return true;
		}
		else {
			if (((ev->eventType == EventType::EVENT_CONTROLLER_BUTTON) && ev->eventValue < ArboriaKey_GamepadDpadLeft || ev->eventValue > ArboriaKey_GamepadDpadRight)
				|| (ev->eventType == EventType::EVENT_KEY) && ev->eventValue < ArboriaKey_LeftArrow && ev->eventValue > ArboriaKey_DownArrow) {
				//update data in the row
				//update the graphic
				BindingData bindData = *(BindingData*)w->getParent()->getData();

				String bindCmd = "bind " + ev->eventValue;
				bindCmd += ", ";
				bindCmd += bindData.action;
				bindCmd += ", "; 
				bindCmd += session->getLastInputDevice();
				commandSystem->pushCommand(const_cast<char*>(bindCmd.c_str()));
				return true;
			}
		}
		return false;
	}

	bool returnToTabList(Widget* w, AEvent* ev)
	{
		ListBoxWidget* lst = dynamic_cast<ListBoxWidget*>(w);
		ListBoxWidget* tabList = w->getGui()->getRoot()->findWidget<ListBoxWidget>("tabbedList");
		w->getGui()->getRoot()->setFocus(tabList);
		return true;
	}

	bool exitOptionsMenu(Widget* w, AEvent* ev)
	{
		commandSystem->pushCommand("mainMenu");
		return true;
	}

	bool handleSelectedTabChange(Widget* w, AEvent* ev) //with non base widgets, Widget::onEvent called first, so callbacks executed before rest of the function triggers
	{
		ListBoxWidget* lst = dynamic_cast<ListBoxWidget*>(w);
		int action = inputManager->getBinding(ev->eventValue);
		Widget* hoveredTab = lst->getHovered();
		Widget* displayWindow = w->getGui()->getRoot()->findWidget<Widget>("displayScreen");
		Widget* bindingWindow = w->getGui()->getRoot()->findWidget<Widget>("inputScreen");
		if (hoveredTab && hoveredTab->getName() == "displayTab") {
			if (action == ACTION_LEFTARROW) { //do nothing because current tab will not change after input
				return false;
			}
			else { //right arrow, so the hovered element will be the bindingTab, which means the displayWindow should be the bindWindow
				displayWindow->setVisibility(false);
				displayWindow->setDirty();
				bindingWindow->setVisibility(true);
				bindingWindow->setDirty();
			}
		}
		else if (hoveredTab && hoveredTab->getName() == "bindingTab") {
			if (action == ACTION_RIGHTARROW)
				return false;
			else {
				bindingWindow->setVisibility(false);
				bindingWindow->setDirty();
				displayWindow->setVisibility(true);
				displayWindow->setDirty();
			}
		}
		return true;
	}

	bool handleReturnToTabList(Widget* w, AEvent* ev)
	{
		ListBoxWidget* tabbedList = w->getGui()->getRoot()->findWidget<ListBoxWidget>("tabbedList");
		w->getGui()->getRoot()->setFocus(tabbedList);
		if (w->getName().compare("displayOptions"))
			w->getGui()->setCursor(tabbedList->getChild(0)->position.x, tabbedList->getChild(0)->position.y);
		else
			w->getGui()->setCursor(tabbedList->getChild(1)->position.x, tabbedList->getChild(1)->position.y);
		return true;
	}

	bool returnToDisplayList(Widget* w, AEvent* ev)
	{
		ListBoxWidget* displayOpts = w->getGui()->getRoot()->findWidget<ListBoxWidget>("displayOptions");
		w->getGui()->getRoot()->setFocus(displayOpts);
		return true;
	}

	bool handleRestoreDefaultBindings(Widget* w, AEvent* ev)
	{
		commandSystem->pushCommand("restoreDefaultBindings");
		inputManager->restoreDefaults();
		Window* optMenu = w->getGui();
		if (optMenu) {
			Widget* inputScreen = optMenu->getRoot()->findWidget<Widget>("inputScreen");
			ListBoxWidget* bindingOptions = inputScreen->findWidget<ListBoxWidget>("bindingOptions");
			Widget* confirmRow = bindingOptions->getChild(0);
			int confirmKey = inputManager->getKeyFromAction(ACTION_CONFIRM);
			int confirmButton = inputManager->getGamepadButtonFromAction(ACTION_CONFIRM);
			BindingData* confirmBind = (BindingData*)confirmRow->getData();
			confirmBind->key = confirmKey;
			confirmBind->gamepad = confirmButton;
			GraphicButton* confirmOption = dynamic_cast<GraphicButton*>(confirmRow->getChild(1));
			confirmOption->setImage(resourceManager->loadTexture(confirmKey + 3));
			confirmOption->setDirty();

			Widget* cancelRow = bindingOptions->getChild(1);
			int cancelKey = inputManager->getKeyFromAction(ACTION_CANCEL);
			int cancelButton = inputManager->getGamepadButtonFromAction(ACTION_CANCEL);
			BindingData* cancelBind = (BindingData*)cancelRow->getData();
			cancelBind->key = cancelKey;
			cancelBind->gamepad = cancelButton;
			GraphicButton* cancelOption = dynamic_cast<GraphicButton*>(cancelRow->getChild(1));
			cancelOption->setImage(resourceManager->loadTexture(cancelKey + 3));
			cancelOption->setDirty();

			Widget* unitMenuRow = bindingOptions->getChild(2);
			int unitMenuKey = inputManager->getKeyFromAction(ACTION_TOGGLE_DISPLAY);
			int unitMenuButton = inputManager->getGamepadButtonFromAction(ACTION_TOGGLE_DISPLAY);
			BindingData* unitMenuBind = (BindingData*)unitMenuRow->getData();
			unitMenuBind->key = unitMenuKey;
			unitMenuBind->gamepad = unitMenuButton;
			GraphicButton* unitMenuOption = dynamic_cast<GraphicButton*>(unitMenuRow->getChild(1));
			unitMenuOption->setImage(resourceManager->loadTexture(unitMenuKey + 3));
			unitMenuOption->setDirty();

			Widget* helpRow = bindingOptions->getChild(3);
			int helpKey = inputManager->getKeyFromAction(ACTION_HELP);
			int helpButton = inputManager->getGamepadButtonFromAction(ACTION_HELP);
			BindingData* helpBind = (BindingData*)helpRow->getData();
			helpBind->key = helpKey;
			helpBind->gamepad = helpButton;
			GraphicButton* helpOption = dynamic_cast<GraphicButton*>(helpRow->getChild(1));
			helpOption->setImage(resourceManager->loadTexture(helpKey + 3));
			helpOption->setDirty();

			Widget* leftShiftRow = bindingOptions->getChild(4);
			int leftShiftKey = inputManager->getKeyFromAction(ACTION_L_TRIGGER_1);
			int leftShiftButton = inputManager->getGamepadButtonFromAction(ACTION_L_TRIGGER_1);
			BindingData* leftShiftBind = (BindingData*)leftShiftRow->getData();
			leftShiftBind->key = leftShiftKey;
			leftShiftBind->gamepad = leftShiftButton;
			GraphicButton* leftShiftOption = dynamic_cast<GraphicButton*>(leftShiftRow->getChild(1));
			leftShiftOption->setImage(resourceManager->loadTexture(leftShiftKey + 3));
			leftShiftOption->setDirty();

			Widget* rightShiftRow = bindingOptions->getChild(5);
			int rightShiftKey = inputManager->getKeyFromAction(ACTION_R_TRIGGER_1);
			int rightShiftButton = inputManager->getGamepadButtonFromAction(ACTION_R_TRIGGER_1);
			BindingData* rightShiftBind = (BindingData*)rightShiftRow->getData();
			rightShiftBind->key = rightShiftKey;
			rightShiftBind->gamepad = rightShiftButton;
			GraphicButton* rightShiftOption = dynamic_cast<GraphicButton*>(rightShiftRow->getChild(1));
			rightShiftOption->setImage(resourceManager->loadTexture(rightShiftKey + 3));
			rightShiftOption->setDirty();

			bindingOptions->setSelected(NULL);

			return true;
		}
		return false;
	}

	template<typename T>
	void OptionsMenu::parseSpinnerOptions(Spinner* spinner, List<T> optionData) {
		Token tok;
		String str2;
		Lexer src;

		String& optionString = spinner->getOptionsString();

		src.setFlags(LEXER_NOFATALERRORS | LEXER_ALLOWBACKSLASHSTRINGCONCATS);
		src.loadMemory(optionString.c_str(), optionString.length(), "<Options>");
		int optIndex = 0;
		if (src.isLoaded()) {
			while (src.readToken(&tok)) {
				if (tok == ";") {
					if (str2.length()) {
						str2.stripTrailingWhitespace();
						spinner->insertOption(str2.c_str(), (void*)&optionData[optIndex]);
						str2 = "";
						optIndex++;
					}
					continue;
				}
				str2 += tok;
				//str2 += " ";
			}
			spinner->setSelectedOption(spinner->getSelectedIndex());
			if (str2.length()) {
				str2.stripTrailingWhitespace();
				spinner->insertOption(str2.c_str(), (void*)&optionData[optIndex]);
			}
		}
	}
}