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
#include "../../utils/math.h"

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
		{640, 480},
		{800, 600},
		{1024, 768},
		{1280, 960}
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

	void resetDefaultBinding(Widget* wd, int act)
	{
		int actKey = inputManager->getKeyFromAction(act);
		int actButton = inputManager->getGamepadButtonFromAction(act);
		BindingOption* boundAction = (BindingOption*)wd->getData();
		BindingData* boundData = boundAction->bindingData;
		boundData->key = actKey;
		boundData->gamepad = actButton;
		GraphicButton* actWidget = dynamic_cast<GraphicButton*>(wd->getChild(1));
		actWidget->setImage(resourceManager->loadTexture(actKey + 3));
		boundAction->command.clear();
		actWidget->setDirty();
	}

	OptionsMenu::OptionsMenu() : Window()
	{
		selectTab = 0;
		inBindWindow = false;
		waitingBind = false;

		currentWindow = NULL;
		tabbedList = NULL;
		bindWindow = NULL;
		selectedResolutionOption = NULL;

		if (bindingDataList.getLength() == 0)
			initializeBindingDataList();
	}

	OptionsMenu::~OptionsMenu() = default;

	bool OptionsMenu::postParse() //parse the options list after getting options string
	{
		tabbedList = root->findWidget<ListBoxWidget>("tabbedList");
		tabbedWindows[0] = root->findWidget<ListBoxWidget>("displayOptions");
		tabbedWindows[1] = root->findWidget<ListBoxWidget>("bindingOptions");

		Widget* resolutionWindow = tabbedWindows[0]->getChild(0);
		Spinner* resOptionSpinner = resolutionWindow->findWidget<Spinner>("resolutionOptions");
		if (!resOptionSpinner)
			return false;

		setCursor(tabbedList->getChild(0)->getActualPosition().x, tabbedList->getChild(0)->getActualPosition().y);

		Widget* vSyncRow = tabbedWindows[0]->getChild(1);
		Spinner* vSyncSpinner = vSyncRow->findWidget<Spinner>("vsyncSpinner");
		if (!vSyncSpinner)
			return false;

		parseSpinnerOptions<ResolutionOptions>(resOptionSpinner, resolutionData);
		parseSpinnerOptions<bool>(vSyncSpinner, vsyncOptionData);

		//set selected Option
		int actualSelResIdx = searchForClosestResolutionOption();
		resOptionSpinner->setSelectedOption(actualSelResIdx);

		if (systemConfig->vSync->getBool())
			vSyncSpinner->setSelectedOption(0);
		else
			vSyncSpinner->setSelectedOption(1);

		//resOptionSpinner->addCallback(ACTION_CANCEL, returnToDisplayList);
		//vSyncSpinner->addCallback(ACTION_CANCEL, returnToDisplayList);

		tabbedList->addCallback(ACTION_CANCEL, exitOptionsMenu);
		tabbedList->getChild(0)->addCallback(ACTION_RIGHTARROW, handleSelectedTabChange);
		tabbedList->getChild(0)->addCallback(ACTION_CONFIRM, handleNavigateToDisplayWindow);
		tabbedList->getChild(1)->addCallback(ACTION_LEFTARROW, handleSelectedTabChange);
		tabbedList->getChild(1)->addCallback(ACTION_CONFIRM, handleNavigateToBindingsWindow);

		//ListBoxWidget* displayOptions = tabbedWindows[0]->findWidget<ListBoxWidget>("displayOptions");
		tabbedWindows[0]->addCallback(ACTION_CANCEL, returnToTabList);
		//ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		tabbedWindows[1]->addCallback(ACTION_CANCEL, returnToTabList);

		Widget* applyRow = tabbedWindows[0]->findWidget<Widget>("applyRow");
		TextButton* applyButton = applyRow->findWidget<TextButton>("applyButton");
		applyButton->addCallback(ACTION_CONFIRM, handleApplyDisplayChanges);
		Widget* restoreDisplayRow = tabbedWindows[0]->findWidget<Widget>("restoreDisplayRow");
		TextButton* defaultSettingsButton = restoreDisplayRow->findWidget<TextButton>("defaultDisplayButton");
		defaultSettingsButton->addCallback(ACTION_CONFIRM, handleRestoreDefaultDisplaySettings);

		Widget* confirmRow = tabbedWindows[1]->findWidget<Widget>("confirmRow");
		confirmRow->setData(new BindingOption(bindingDataList[ACTION_CONFIRM]));
		confirmRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* cancelRow = tabbedWindows[1]->findWidget<Widget>("cancelRow");
		cancelRow->setData(new BindingOption(bindingDataList[ACTION_CANCEL]));
		cancelRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* unitMenuRow = tabbedWindows[1]->findWidget<Widget>("unitMenuRow");
		unitMenuRow->setData(new BindingOption(bindingDataList[ACTION_TOGGLE_DISPLAY]));
		unitMenuRow->addCallback(ACTION_CONFIRM, handleBindingSelect);

		Widget* helpRow = tabbedWindows[1]->findWidget<Widget>("helpRow");
		helpRow->setData(new BindingOption(bindingDataList[ACTION_HELP]));
		helpRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* lShiftRow = tabbedWindows[1]->findWidget<Widget>("leftShiftRow");
		lShiftRow->setData(new BindingOption(bindingDataList[ACTION_L_TRIGGER_1]));
		lShiftRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* rShiftRow = tabbedWindows[1]->findWidget<Widget>("rightShiftRow");
		rShiftRow->setData(new BindingOption(bindingDataList[ACTION_R_TRIGGER_1]));
		rShiftRow->addCallback(ACTION_CONFIRM, handleBindingSelect);
		
		Widget* restoreDefaultBindingsRow = tabbedWindows[1]->findWidget<Widget>("restoreDefaultBindingsRow");
		TextButton* restoreDefaultBindingsButton = restoreDefaultBindingsRow->findWidget<TextButton>("restoreDefaultBindingsButton");
		restoreDefaultBindingsButton->addCallback(ACTION_CONFIRM, handleRestoreDefaultBindings);

		return true;
	}

	bool OptionsMenu::parseBindings()
	{
		//ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		GraphicButton* confirmOption = tabbedWindows[1]->getChild(0)->findWidget<GraphicButton>("confirmOption");
		if (!confirmOption)
			return false;
		GraphicButton* cancelOption = tabbedWindows[1]->getChild(1)->findWidget<GraphicButton>("cancelOption");
		if (!cancelOption)
			return false;
		GraphicButton* unitMenuOption = tabbedWindows[1]->getChild(2)->findWidget<GraphicButton>("unitMenuOption");
		if (!unitMenuOption)
			return false;
		GraphicButton* helpOption = tabbedWindows[1]->getChild(3)->findWidget<GraphicButton>("helpOption");
		if (!helpOption)
			return false;
		GraphicButton* leftShiftOption = tabbedWindows[1]->getChild(4)->findWidget<GraphicButton>("leftShiftOption");
		if (!leftShiftOption)
			return false;
		GraphicButton* rightShiftOption = tabbedWindows[1]->getChild(5)->findWidget<GraphicButton>("rightShiftOption");
		if (!rightShiftOption)
			return false;
		TextButton* defaultBindingsButton = tabbedWindows[1]->getChild(6)->findWidget<TextButton>("restoreDefaultBindingsButton");
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
			tabbedWindows[1]->getSelected()->executeCallback(ACTION_CONFIRM, e);
		}

		Window::onEvent(e);
	}

	void OptionsMenu::resetOptions()
	{
		ResolutionOptions actualResolution{ systemConfig->windowViewportX->getInteger(), systemConfig->windowViewportY->getInteger() };
		//ListBoxWidget* displayOptions = static_cast<ListBoxWidget*>(tabbedWindows[0]->getChild(0));
		Spinner* resolutionSpinner = tabbedWindows[0]->getChild(0)->findWidget<Spinner>("resolutionOptions");
		List<SpinnerOption>& spinnerOptions = resolutionSpinner->getOptions();

		for (int i = 0; i < spinnerOptions.getLength(); i++) {
			ResolutionOptions* resolutionData = (ResolutionOptions*)spinnerOptions[i].optionData;
			if (resolutionData->x == actualResolution.x && resolutionData->y == actualResolution.y) {
				resolutionSpinner->setSelectedOption(i);
				break;
			}
		}

		Spinner* vsyncSpinner = tabbedWindows[0]->getChild(1)->findWidget<Spinner>("vsyncSpinner");
		List<SpinnerOption>& vsyncOptions = vsyncSpinner->getOptions();
		for (int j = 0; j < vsyncOptions.getLength(); j++) {
			bool vsyncData = *(bool*)vsyncOptions[j].optionData;
			if (vsyncData == systemConfig->vSync->getBool()) {
				vsyncSpinner->setSelectedOption(j);
				break;
			}
		}
		systemConfig->updateConfigFile();
	}

	void OptionsMenu::restoreDefaultSettings() {
		//ResolutionOptions defaultResolution{ systemConfig->defaultWindowViewportX->getInteger(), systemConfig->defaultFullscreenWindowViewportY->getInteger() };
		Widget* resolutionRow = tabbedWindows[0]->findWidget<Widget>("resolutionRow");
		Spinner* resolutionSpinner = resolutionRow->findWidget<Spinner>("resolutionOptions");
		List<SpinnerOption>& spinnerOptions = resolutionSpinner->getOptions();

		for (int i = 0; i < spinnerOptions.getLength(); i++) {
			ResolutionOptions* resolutionData = (ResolutionOptions*)spinnerOptions[i].optionData;
			if (resolutionData->x == systemConfig->defaultWindowViewportX->getInteger() && resolutionData->y == systemConfig->defaultWindowViewportY->getInteger()) {
				resolutionSpinner->setSelectedOption(i);
				break;
			}
		}

		Widget* vSyncRow = tabbedWindows[0]->findWidget<Widget>("vsyncRow");
		Spinner* vsyncSpinner = (Spinner*)vSyncRow->getChild(1);
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
		//ListBoxWidget* bindingOptions = tabbedWindows[1]->findWidget<ListBoxWidget>("bindingOptions");
		
		Widget* confirmRow = tabbedWindows[1]->findWidget<Widget>("confirmRow");
		resetDefaultBinding(confirmRow, ACTION_CONFIRM);

		Widget* cancelRow = tabbedWindows[1]->findWidget<Widget>("cancelRow");
		resetDefaultBinding(cancelRow, ACTION_CANCEL);

		Widget* unitMenuRow = tabbedWindows[1]->findWidget<Widget>("unitMenuRow");
		resetDefaultBinding(unitMenuRow, ACTION_TOGGLE_DISPLAY);

		Widget* helpRow = tabbedWindows[1]->findWidget<Widget>("helpRow");
		resetDefaultBinding(helpRow, ACTION_HELP);

		Widget* leftShiftRow = tabbedWindows[1]->findWidget<Widget>("leftShiftRow");
		resetDefaultBinding(leftShiftRow, ACTION_L_TRIGGER_1);

		Widget* rightShiftRow = tabbedWindows[1]->findWidget<Widget>("rightShiftRow");
		resetDefaultBinding(rightShiftRow, ACTION_R_TRIGGER_1);

		tabbedWindows[1]->setSelected(NULL);
	}

	uint8_t OptionsMenu::searchForClosestResolutionOption()
	{
		if (resolutionData.getLength() == 0) {
			return -1;
		}
		if (resolutionData.getLength() == 1)
			return 0;

		int lBound = 0;
		int rBound = resolutionData.getLength();
		int midpoint = 0;
		int xScore, yScore = 0;
		int scoreSum = -1;

		int actualX = systemConfig->windowViewportX->getInteger();
		int actualY = systemConfig->windowViewportY->getInteger();

		int* scoreSums = (int*)Mem_Alloc(rBound * sizeof(int));
		int retVal = 0;
		int retInd = 0;

		while (lBound <= rBound && scoreSum != 0) {
			midpoint = lBound + Math::ftoi(Math::floor(rBound - lBound) / 2);
			xScore = resolutionData[midpoint].x - actualX;
			yScore = resolutionData[midpoint].y - actualY;

			scoreSum = xScore + yScore;
			scoreSums[midpoint] = scoreSum;

			if (actualX >= resolutionData[midpoint].x)
				lBound = midpoint;
			else
				rBound = midpoint;
		}

		if (scoreSum == 0) {
			delete scoreSums;
			return midpoint;
		}

		retVal = scoreSums[0];

		for (int a = 0; a < resolutionData.getLength(); a++) {
			if (abs(0 - scoreSums[a]) <= abs(0 - retVal)) {
				retVal = scoreSums[a];
				retInd = a;
			}
		}

		delete scoreSums;
		return retInd;
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
				|| ((ev->eventType == EventType::EVENT_KEY) && (ev->eventValue < ArboriaKey_LeftArrow || ev->eventValue > ArboriaKey_DownArrow) && ev->eventValue2 > 0)) {
				//update data in the row
				//update the graphic
				BindingOption* chosenBinding = (BindingOption*)w->getData();

				String bindCmd = String::format("bind %i, %i, %i", ev->eventValue, chosenBinding->bindingData->action, session->getLastInputDevice());
				if (bindCmd.compare(chosenBinding->command.c_str()) == 0) {
					ListBoxWidget* parentOptionsWindow = (ListBoxWidget*)w->getParent();
					parentOptionsWindow->setSelected(NULL);
					session->isBindWait = false;
					return true;
				}

				chosenBinding->command = bindCmd;
				commandSystem->pushCommand(chosenBinding->command.c_str());
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
		if (tabList->getHovered()) {
			w->getGui()->setCursor(tabList->getHovered()->getActualPosition().x, tabList->getHovered()->getActualPosition().y);
		}
		else {
			w->getGui()->setCursor(tabList->getActualPosition().x, tabList->getActualPosition().y);
		}
		return true;
	}

	bool exitOptionsMenu(Widget* w, AEvent* ev)
	{
		commandSystem->pushCommand("mainMenu");
		return true;
	}

	bool handleSelectedTabChange(Widget* w, AEvent* ev) //with non base widgets, Widget::onEvent called first, so callbacks executed before rest of the function triggers
	{
		ListBoxWidget* lst = dynamic_cast<ListBoxWidget*>(w->getParent());
		int action = inputManager->getBinding(ev->eventValue);
		Widget* hoveredTab = lst->getHovered();
		if (hoveredTab == NULL) {
			hoveredTab = lst->getChild(0);
		}
		ListBoxWidget* displayWindow = w->getGui()->getRoot()->findWidget<ListBoxWidget>("displayOptions");
		ListBoxWidget* bindingWindow = w->getGui()->getRoot()->findWidget<ListBoxWidget>("bindingOptions");
		if (hoveredTab && hoveredTab->getName() == "displayTab") {
			if (action == ACTION_LEFTARROW) { //do nothing because current tab will not change after input
				return false;
			}
			else if (action == ACTION_RIGHTARROW){ //right arrow, so the hovered element will be the bindingTab, which means the displayWindow should be the bindWindow
				displayWindow->setVisibility(false);
				displayWindow->setDirty();
				bindingWindow->setVisibility(true);
				bindingWindow->setDirty();
			}
		}
		else if (hoveredTab && hoveredTab->getName() == "bindingTab") {
			if (action == ACTION_RIGHTARROW)
				return false;
			else if (action == ACTION_LEFTARROW){
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
			//Widget* inputScreen = optMenu->getRoot()->findWidget<Widget>("inputScreen");
			ListBoxWidget* bindingOptions = optMenu->getRoot()->findWidget<ListBoxWidget>("bindingOptions");
			Widget* confirmRow = bindingOptions->getChild(0);
			resetDefaultBinding(confirmRow, ACTION_CONFIRM);

			Widget* cancelRow = bindingOptions->getChild(1);
			resetDefaultBinding(cancelRow, ACTION_CANCEL);

			Widget* unitMenuRow = bindingOptions->getChild(2);
			resetDefaultBinding(unitMenuRow, ACTION_TOGGLE_DISPLAY);

			Widget* helpRow = bindingOptions->getChild(3);
			resetDefaultBinding(helpRow, ACTION_HELP);

			Widget* leftShiftRow = bindingOptions->getChild(4);
			resetDefaultBinding(leftShiftRow, ACTION_L_TRIGGER_1);

			Widget* rightShiftRow = bindingOptions->getChild(5);
			resetDefaultBinding(rightShiftRow, ACTION_R_TRIGGER_1);

			bindingOptions->setSelected(NULL);

			return true;
		}
		return false;
	}

	bool handleNavigateToDisplayWindow(Widget* w, AEvent* ev)
	{
		Window* window = w->getGui();
		//Widget* displayScreen = window->getRoot()->findWidget<Widget>("displayScreen");
		ListBoxWidget* dOptions = window->getRoot()->findWidget<ListBoxWidget>("displayOptions");
		window->getRoot()->setFocus(dOptions);
		window->setCursor(dOptions->getActualPosition().x, dOptions->getActualPosition().y);
		if (!dOptions->getHovered())
			dOptions->setHovered();
		return true;
	}

	bool handleNavigateToBindingsWindow(Widget* w, AEvent* ev)
	{
		Window* window = w->getGui();
		//Widget* bindingScreen = window->getRoot()->findWidget<Widget>("inputScreen");
		ListBoxWidget* bOptions = window->getRoot()->findWidget<ListBoxWidget>("bindingOptions");
		window->setCursor(bOptions->getActualPosition().x, bOptions->getActualPosition().y);
		bOptions->setHovered();
		return true;
	}

	void initializeBindingDataList()
	{
		bindingDataList.clear();
		bindingDataList = {
			{ inputManager->getKeyFromAction(ACTION_CONFIRM), inputManager->getGamepadButtonFromAction(ACTION_CONFIRM), ACTION_CONFIRM },
			{ inputManager->getKeyFromAction(ACTION_CANCEL), inputManager->getGamepadButtonFromAction(ACTION_CANCEL), ACTION_CANCEL },
			{ inputManager->getKeyFromAction(ACTION_TOGGLE_DISPLAY), inputManager->getGamepadButtonFromAction(ACTION_TOGGLE_DISPLAY), ACTION_TOGGLE_DISPLAY },
			{ inputManager->getKeyFromAction(ACTION_HELP), inputManager->getGamepadButtonFromAction(ACTION_HELP), ACTION_HELP },
			{ inputManager->getKeyFromAction(ACTION_L_TRIGGER_1), inputManager->getGamepadButtonFromAction(ACTION_L_TRIGGER_1), ACTION_L_TRIGGER_1},
			{ inputManager->getKeyFromAction(ACTION_R_TRIGGER_1), inputManager->getGamepadButtonFromAction(ACTION_R_TRIGGER_1), ACTION_R_TRIGGER_1},
		};
	}

	template<typename T>
	void OptionsMenu::parseSpinnerOptions(Spinner* spinner, const List<T>& optionData) {
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