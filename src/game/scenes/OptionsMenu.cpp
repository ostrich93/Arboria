#include "OptionsMenu.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/Label.h"
#include "../../gui/Graphic.h"
#include "../../gui/TextButton.h"
#include "../../framework/ResourceManager.h"
#include "../../framework/ActionManager.h"
#include "../../framework/ScreenManager.h"

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
		{"640x360 (16:9)", 2, 2},
		{"960x540 (16:9)", 3, 3},
		{"1280x720 (16:9)", 4, 4},
		{"1600x900 (16:9)", 5, 5},
		{"1920x1080 (16:9)", 6, 6}
	};

	void OptionsMenu::buildDisplayWidow()
	{
		ListBoxWidget* displayWindow = container.createChild<ListBoxWidget>(6, 1);
		displayWindow->size = { 600, 292 };
		displayWindow->position = { 0, 48 };
		displayWindow->align(HOR_CENTER);
		displayWindow->name = "DISPLAY_WINDOW";
		displayWindow->setFlag(WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		displayWindow->setBorderSize(1);
		displayWindow->itemSize = 47;
		displayWindow->itemSpacing = 0;
		displayWindow->enabled = true;

		displayWindow->addItem(new Widget());
		displayWindow->children[0]->setFlag(WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		displayWindow->children[0]->size = { 600, 47 };
		displayWindow->children[0]->align(HOR_CENTER);
		displayWindow->children[0]->name = "DISPLAY_ELEM_1";
		displayWindow->children[0]->setData((void*)&resolutionData);

		TextButton* resolutionLabel = displayWindow->children[0]->createChild<TextButton>("Resolution", resourceManager->loadFont("logotypegothicregular.ttf", 24));
		resolutionLabel->size = { 300, 47 };
		resolutionLabel->setBorderSize(1);
		resolutionLabel->textHAlign = HorizontalAlignment::HOR_CENTER;
		resolutionLabel->clickable = false;
		resolutionLabel->name = "VALUE_LABEL";

		TextButton* resolutionValueButton = displayWindow->children[0]->createChild<TextButton>(resolutionData[0].description, resourceManager->loadFont("logotypegothicregular.ttf", 24));
		resolutionValueButton->size = { 300, 47 };
		resolutionValueButton->setBorderSize(1);
		resolutionValueButton->textHAlign = HorizontalAlignment::HOR_CENTER;
		resolutionValueButton->clickable = true;
		resolutionValueButton->name = "VALUE_BUTTON";
		resolutionValueButton->setData((void*)&resolutionData[0]);

		Graphic* leftArrow = resolutionValueButton->createChild<Graphic>(resourceManager->loadTexture(2));
		leftArrow->clearFlag(WIDGET_VISIBLE);
		leftArrow->autoSize = true;
		leftArrow->align(HorizontalAlignment::HOR_LEFT);
		//set position
		//set size
		leftArrow->name = "L_ARROW";

		Graphic* rightArrow = resolutionValueButton ->createChild<Graphic>(resourceManager->loadTexture(3));
		rightArrow->clearFlag(WIDGET_VISIBLE);
		//set position
		//set size
		rightArrow->position = { 240, 0 };
		rightArrow->autoSize = true;
		rightArrow->align(HorizontalAlignment::HOR_RIGHT, VerticalAlignment::VERT_CENTER);
		rightArrow->name = "R_ARROW";

		displayWindow->addItem(new Widget());
		displayWindow->children[1]->setFlag(WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		displayWindow->children[1]->size = { 600, 47 };
		displayWindow->children[1]->align(HOR_CENTER);
		displayWindow->children[1]->name = "DISPLAY_ELEM_2";

		TextButton* applyButton = displayWindow->children[1]->createChild<TextButton>("Apply", resourceManager->loadFont("logotypegothicregular.ttf", 24));
		applyButton->size = { 300, 47 };
		applyButton->setBorderSize(1);
		applyButton->textHAlign = HorizontalAlignment::HOR_RIGHT;
		applyButton->clickable = true;
		applyButton->name = "APPLY_BUTTON";
		applyButton->addEventCallback(GUIEventType::BUTTON_CLICK, &handleApplyResolutionChanges);

		//add event callbacks to list and buttons
		displayWindow->addEventCallback(GUIEventType::LISTBOX_CHANGE_CANCEL, &handleTabbedWindowCancelChange);
		displayWindow->addEventCallback(GUIEventType::LISTBOX_CHANGE_SELECTED, &handleDisplayWindowOptionSelectChange);
		tabbedWindows[0] = displayWindow;
	}

	void OptionsMenu::buildInputWindow()
	{
		ListBoxWidget* inputConfigWindow = container.createChild<ListBoxWidget>(12, 2);
		inputConfigWindow->size = { 600, 292 };
		inputConfigWindow->position = { 0, 48 };
		inputConfigWindow->align(HOR_CENTER);
		inputConfigWindow->setBorderSize(1);
		inputConfigWindow->itemSize = 64;
		inputConfigWindow->itemSpacing = 0;
		inputConfigWindow->enabled = true;
		inputConfigWindow->name = "INPUT_WINDOW";
		inputConfigWindow->setFlag(WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		inputConfigWindow->clearFlag(WidgetStateFlags::WIDGET_VISIBLE);

		buildInputRow(inputConfigWindow, "Confirm", 0);
		buildInputRow(inputConfigWindow, "Cancel", 1);
		buildInputRow(inputConfigWindow, "Unit Menu", 2);
		buildInputRow(inputConfigWindow, "Help", 3);
		buildInputRow(inputConfigWindow, "Start", 4);
		buildInputRow(inputConfigWindow, "Up", 5);
		buildInputRow(inputConfigWindow, "Down", 6);
		buildInputRow(inputConfigWindow, "Left", 7);
		buildInputRow(inputConfigWindow, "Right", 8);
		buildInputRow(inputConfigWindow, "LShift", 9);
		buildInputRow(inputConfigWindow, "RShift", 10);

		//add button callbacks here. 

		//list select change, set focus to button
		//buttonclick, set action binding to input and child graphic element, then set focus back to list

		inputConfigWindow->addItem(new Widget());
		inputConfigWindow->children[12]->setFlag(WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		inputConfigWindow->children[12]->size = { 600, 64 };
		inputConfigWindow->children[12]->align(HOR_CENTER);
		inputConfigWindow->children[12]->name = "INPUT_ELEM_12";

		TextButton* blankLabel = inputConfigWindow->children[12]->createChild<TextButton>();
		//blankLabel->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		blankLabel->setBorderSize(1);
		blankLabel->clickable = false;
		blankLabel->name = "RESTORE_DEFAULTS_LABEL";

		TextButton* restoreDefaultButton = inputConfigWindow->children[12]->createChild<TextButton>("Restore Defaults", resourceManager->loadFont("logotypegothicregular.ttf", 24));
		restoreDefaultButton->size = { 300, 64 };
		//restoreDefaultButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		restoreDefaultButton->textHAlign = HOR_CENTER;
		restoreDefaultButton->clickable = true;
		restoreDefaultButton->name = "RESTORE_DEFAULTS_BUTTON";
		restoreDefaultButton->addEventCallback(GUIEventType::BUTTON_CLICK, &handleOptionsRestoreDefaultBindings);

		tabbedWindows[1] = inputConfigWindow;
	}

	void OptionsMenu::buildInputRow(ListBoxWidget* configWindow, String actionName, int actionValue)
	{
		configWindow->addItem(new Widget());
		configWindow->children[actionValue]->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_CONTAINER | WidgetStateFlags::WIDGET_MENU);
		configWindow->children[actionValue]->size = { 600, 64 };
		configWindow->children[actionValue]->align(HOR_CENTER);
		configWindow->children[actionValue]->name = "INPUT_ELEM_" + actionValue;

		TextButton* bindLabel = configWindow->children[actionValue]->createChild<TextButton>(actionName, resourceManager->loadFont("logotypegothicregular.ttf", 24));
		//bindLabel->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		bindLabel->setBorderSize(1);
		bindLabel->textHAlign = HorizontalAlignment::HOR_CENTER;
		bindLabel->clickable = false;
		bindLabel->name = "VALUE_LABEL";

		TextButton* bindButton = configWindow->children[actionValue]->createChild<TextButton>();
		bindButton->size = { 300, 64 };
		//bindButton->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		bindButton->setBorderSize(1);
		bindButton->textHAlign = HorizontalAlignment::HOR_CENTER;
		bindButton->clickable = true;
		bindButton->name = "VALUE_BUTTON";
		bindButton->setData((void*)actionManager->getActionBinding(actionName));

		Graphic* bindSymbol = bindButton->createChild<Graphic>(
			resourceManager->loadTexture(static_cast<ActionBinding*>(bindButton->getData())->getId()));
		bindSymbol->size = { 64, 64 };
		//bindSymbol->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		bindSymbol->align(HorizontalAlignment::HOR_CENTER);
		bindSymbol->name = "VALUE_SYMBOL";
	}

	void OptionsMenu::buildTabList()
	{
		tabbedList = container.createChild<ListBoxWidget>(1, 2);
		//tabbedList->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		tabbedList->viewOrientation = Orientation::HORIZONTAL;
		tabbedList->size = { 600, 48 };
		tabbedList->align(HorizontalAlignment::HOR_LEFT);
		tabbedList->name = "OPTIONS_TAB_LIST";
		tabbedList->enabled = true;
		
		tabbedList->addItem(new TextButton("Display", resourceManager->loadFont("logotypegothicregular.ttf", 24)));
		tabbedList->children[0]->name = "DISPLAY_TAB";
		//tabbedList->children[0]->setFlag(WidgetStateFlags::WIDGET_VISIBLE);
		tabbedList->children[0]->size = { 96, 48 };
		tabbedList->findWidget<TextButton>("DISPLAY_TAB")->textHAlign = HorizontalAlignment::HOR_CENTER;
		tabbedList->findWidget<TextButton>("DISPLAY_TAB")->textVAlign = VerticalAlignment::VERT_CENTER;
		tabbedList->children[0]->clickable = true;

		tabbedList->addEventCallback(GUIEventType::LISTBOX_CHANGE_HOVER, &handleOptionsHoverTabChange);
		tabbedList->addEventCallback(GUIEventType::LISTBOX_CHANGE_SELECTED, &handleOptionsSelectTabChange);
		tabbedList->addEventCallback(GUIEventType::LISTBOX_CHANGE_CANCEL, [](AEvent* e, void* context) //CANCEL out of TabSelect, return to previous screen
			{
				screenManager->pushScreenCommand({ ScreenCommand::ScreenCommandType::POP });
			});
	}

	OptionsMenu::OptionsMenu()
	{
		selectTab = 0;
		inBindWindow = false;
		waitingBind = false;

		container.position = { 0, 0 };
		container.size = { 600, 340 };
		container.name = "OPTIONS_CONTAINER";
		container.setFlag(WidgetStateFlags::WIDGET_MENU | WidgetStateFlags::WIDGET_ACTIVE);
		container.setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		container.setFlag(WidgetStateFlags::WIDGET_CONTAINER);
		container.enabled = true;

		//DISPLAY WINDOW
		buildDisplayWidow();

		currentWindow = tabbedWindows[selectTab];
	}

	OptionsMenu::~OptionsMenu() = default;

	void OptionsMenu::enter() {}
	void OptionsMenu::pause() {}
	void OptionsMenu::resume() {}
	void OptionsMenu::leave() {}
	bool OptionsMenu::onEvent(AEvent* ev) {
		container.onEvent(ev);

		if (ev->eventType == EventType::EVENT_UI_INTERACTION) {
			if (ev->guiEvent.eventType == GUIEventType::BUTTON_CLICK) {
				if (ev->guiEvent.raisedBy->name == "VALUE_BUTTON") { //When user has clicked on resolution display option
					auto parent = ev->guiEvent.raisedBy->getParent();
					if (parent != nullptr && parent->name == "DISPLAY_ELEM_1") {
						TextButton* button = static_cast<TextButton*>(ev->guiEvent.raisedBy);
						int act = actionManager->getAction(*ev);
						switch (act) {
							case 1: //CONFIRM
								selectedResolutionOption = resolutionData[selectResolutionIndex];
								ev->guiEvent.raisedBy->setData((void*)&selectedResolutionOption);
								button->setText(selectedResolutionOption.description);
								button->clearFlag(WIDGET_FOCUSED);
								tabbedWindows[0]->setFlag(WIDGET_FOCUSED);
								container.findWidget<Graphic>("L_ARROW")->clearFlag(WIDGET_VISIBLE);
								container.findWidget<Graphic>("R_ARROW")->clearFlag(WIDGET_VISIBLE);
								break;
							case 2: //CANCEL: set focus back to the list
								ev->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
								tabbedWindows[0]->setFlag(WIDGET_FOCUSED);
								container.findWidget<Graphic>("L_ARROW")->clearFlag(WIDGET_VISIBLE);
								container.findWidget<Graphic>("R_ARROW")->clearFlag(WIDGET_VISIBLE);
								break;
							case 8: //LEFT
								selectResolutionIndex = (selectResolutionIndex - 1 % resolutionData.getLength() + resolutionData.getLength()) % resolutionData.getLength();
								ev->guiEvent.raisedBy->setData((void*)&resolutionData[selectResolutionIndex]);
								break;
							case 9:
								selectResolutionIndex = (selectResolutionIndex + 1 % resolutionData.getLength() + resolutionData.getLength()) % resolutionData.getLength();
								ev->guiEvent.raisedBy->setData((void*)&resolutionData[selectResolutionIndex]);
								break;
							default:
								break;
						}
					}
					else if (parent != nullptr && String::findString(parent->name, "INPUT_ELEM") != -1) {
						if (waitingBind) {
							actionManager->updateAction(*ev, static_cast<ActionBinding*>(ev->guiEvent.raisedBy->getData())->getName(), ev->inputDeviceType, ev->guiEvent.keyInfo.keyCode);
							actionManager->saveUserBindings();
							waitingBind = false;
							ev->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
							tabbedWindows[1]->setSelected(nullptr);
							tabbedWindows[1]->setFlag(WIDGET_FOCUSED);
						}
						else {
							int acto = actionManager->getAction(*ev);
							switch (acto) {
								case 1:
									waitingBind = true;
									break;
								case 2:
									ev->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
									tabbedWindows[1]->setSelected(nullptr);
									tabbedWindows[1]->setFlag(WIDGET_FOCUSED);
									break;
								default:
									break;
							}
						}
					}
				}
			}
		}
		return ev->isHandled; 
	}
	void OptionsMenu::run() { container.run(); }
	void OptionsMenu::draw() { container.render(); }
	bool OptionsMenu::isTransitioning() const { return false; }

	//bind window
	void OptionsMenu::sendSelectOption(Widget* w)
	{
		AEvent* ev = new AEvent();
		ev->eventType = EventType::EVENT_UI_INTERACTION;
		//ev->guiEvent.
		//inputManager->submitEvent()
	}

	void handleOptionsHoverTabChange(AEvent* e, void* context) {
		auto menu = (OptionsMenu*)context;
		auto hovered = menu->getTabbedList()->getHovered();
		if (e->guiEvent.raisedBy == hovered) {
			if (hovered->name == "DISPLAY_TAB") {
				menu->getContainer().findWidget<ListBoxWidget>("DISPLAY_WINDOW")->setFlag(WIDGET_VISIBLE);
				menu->getContainer().findWidget<ListBoxWidget>("INPUT_WINDOW")->clearFlag(WIDGET_VISIBLE);
			}
			else if (hovered->name == "INPUT_TAB") {
				menu->getContainer().findWidget<ListBoxWidget>("INPUT_WINDOW")->setFlag(WIDGET_VISIBLE);
				menu->getContainer().findWidget<ListBoxWidget>("DISPLAY_WINDOW")->clearFlag(WIDGET_VISIBLE);
			}
		}
	}

	void handleOptionsSelectTabChange(AEvent* e, void* context) {
		auto menu = (OptionsMenu*)context;
		if (e->guiEvent.raisedBy->name == "OPTIONS_TAB_LIST") {
			auto selectedTab = menu->getTabbedList()->getSelected();
			if (selectedTab->name == "DISPLAY_TAB") {
				menu->getTabbedList()->clearFlag(WidgetStateFlags::WIDGET_FOCUSED);
				menu->getContainer().findWidget<ListBoxWidget>("DISPLAY_WINDOW")->setFlag(WIDGET_FOCUSED);
			}
			else if (selectedTab->name == "INPUT_TAB") {
				menu->getTabbedList()->clearFlag(WidgetStateFlags::WIDGET_FOCUSED);
				menu->getContainer().findWidget<ListBoxWidget>("INPUT_WINDOW")->setFlag(WIDGET_FOCUSED);
			}
		}
	}

	void handleTabbedWindowCancelChange(AEvent* e, void* context) {
		auto menu = (OptionsMenu*)context;
		if (e->guiEvent.raisedBy->name == "DISPLAY_WINDOW") {
			e->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
			menu->getTabbedList()->findWidget<ListBoxWidget>("OPTIONS_TAB_LIST")->setFlag(WIDGET_FOCUSED);
			e->isHandled = true;
			return;
		}
		else if (e->guiEvent.raisedBy->name == "INPUT_WINDOW") {
			e->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
			menu->getTabbedList()->findWidget<ListBoxWidget>("OPTIONS_TAB_LIST")->setFlag(WIDGET_FOCUSED);
			e->isHandled = true;
			return;
		}
	}

	void handleDisplayWindowOptionSelectChange(AEvent* e, void* context) {
		auto menu = (OptionsMenu*)context;
		auto displayWindow = menu->getContainer().findWidget<ListBoxWidget>("DISPLAY_WINDOW");
		auto selectedElem = displayWindow->getSelected();
		if (selectedElem->name == "DISPLAY_ELEM_1") {
			selectedElem->findWidget<Graphic>("L_ARROW")->setFlag(WIDGET_VISIBLE);
			selectedElem->findWidget<Graphic>("R_ARROW")->setFlag(WIDGET_VISIBLE);
		}
		if (selectedElem->name != "DISPLAY_ELEM_2") {
			selectedElem->findWidget<TextButton>("VALUE_BUTTON")->setFlag(WIDGET_FOCUSED);
			displayWindow->clearFlag(WIDGET_FOCUSED);
		}
		else {
			selectedElem->findWidget<TextButton>("APPLY_BUTTON")->submitGuiEvent(GUIEventType::BUTTON_CLICK, e);
		}
		e->isHandled = true;
	}

	void handleApplyResolutionChanges(AEvent* e, void* context) {
		//set config/renderdevice and save settings, then change focus)
		auto menu = (OptionsMenu*)context;
		renderDevice->resize(resolutionData[menu->getSelectedResolutionIndex()].xScale);
		e->isHandled = true;
	}

	void handleOptionsRestoreDefaultBindings(AEvent* e, void* context) {
		actionManager->restoreDefaults();
		actionManager->saveUserBindings();
		auto menu = (OptionsMenu*)context;

		auto inWin = menu->getContainer().findWidget<ListBoxWidget>("INPUT_WIDGET");
		for (int i = 0; i < inWin->children.getLength() - 1; i++) {
			auto inRow = inWin->children[i];
			TextButton* inputButton = inRow->findWidget<TextButton>("VALUE_BUTTON");
			if (inputButton != nullptr) {
				inputButton->setData(actionManager->getActionBinding(i + 1));
				Graphic* inputImg = inputButton->findWidget<Graphic>("VALUE_SYMBOL");
				if (inputImg != nullptr) {
					inputImg->setImage(resourceManager->loadTexture(static_cast<ActionBinding*>(inputButton->getData())->getId()));
				}
			}
		}

		e->guiEvent.raisedBy->clearFlag(WIDGET_FOCUSED);
		inWin->setFlag(WIDGET_FOCUSED);
		inWin->setSelected(nullptr);
		e->isHandled = true;
	}
}