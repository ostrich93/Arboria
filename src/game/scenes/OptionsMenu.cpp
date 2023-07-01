#include "OptionsMenu.h"
#include "../../gui/ListBoxWidget.h"
#include "../../gui/Label.h"
#include "../../gui/SliderWidget.h"

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

	OptionsMenu::OptionsMenu()
	{
		selectTab = 0;
		inBindWindow = false;
		waitingBind = false;

		container.rect = { 0, 0, 640, 480 };
		container.name = "OPTIONS_CONTAINER";
		container.setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_MENU | WidgetStateFlags::WIDGET_ACTIVE);
		container.setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		container.setFlag(WidgetStateFlags::WIDGET_CONTAINER);
		container.enabled = true;

		Font* font = fontManager->getFont("logotypegothicregular", "", 24);
		Label* displayTab = container.createChild<Label>("DISPLAY_TAB", font);
		displayTab->rect = { 0, 0, 80, 30 };
		displayTab->setBorderSize(1);
		displayTab->setText("Display");
		displayTab->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_ACTIVE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_NOCURSOR);
		tabList[0] = displayTab;

		Label* inputTab = container.createChild<Label>("INPUT_TAB", font);
		inputTab->rect = { 81, 0, 80, 30 };
		inputTab->setBorderSize(1);
		inputTab->setText("Input");
		inputTab->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_ACTIVE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_NOCURSOR);
		tabList[1] = inputTab;

		Label* audioTab = container.createChild<Label>("AUDIO_TAB", font);
		audioTab->rect = { 162, 0, 80, 30 };
		audioTab->setBorderSize(1);
		audioTab->setText("Audio");
		audioTab->setFlag(WidgetStateFlags::WIDGET_VISIBLE | WidgetStateFlags::WIDGET_ACTIVE | WidgetStateFlags::WIDGET_INTERACTIVE | WidgetStateFlags::WIDGET_NOCURSOR);
		tabList[2] = audioTab;

		//DISPLAY WINDOW
		ListBoxWidget* displayConfigWindow = container.createChild<ListBoxWidget>(6, 1);
		displayConfigWindow->rect = { 0, 31, 640, 449 };
		displayConfigWindow->setBorderSize(1);
		displayConfigWindow->itemSize = 30;
		displayConfigWindow->itemSpacing = 0;

		//WINDOW MODE SELECTOR: LABEL + SELECT TOGGLER
		Widget* windowModeSelector = new Widget();
		windowModeSelector->name = "WINDOW_MODE_SELECTOR";
		windowModeSelector->rect = { 0, 0, 640, 30 };
		windowModeSelector->setBorderSize(1);
		WindowModeSelectData* modeOptionsData = new WindowModeSelectData();
		modeOptionsData->modeOptions.append({ "Windowed", 0 });
		modeOptionsData->modeOptions.append({ "Fullscreen", SDL_WINDOW_FULLSCREEN });
		windowModeSelector->setData(modeOptionsData);
		Label* selectModeLabel = windowModeSelector->createChild<Label>("WINDOW_MODE_SELECTOR_LABEL", font);
		selectModeLabel->setText("Screen Mode");
		selectModeLabel->align(HorizontalAlignment::HOR_LEFT);
		Label* modeToggler = windowModeSelector->createChild<Label>("WINDOW_MODE_TOGGLER", font);
		modeToggler->rect = { 320, 0, 200, 30 };
		modeToggler->align(HorizontalAlignment::HOR_CENTER);
		modeToggler->setData(&modeOptionsData->modeOptions[0]);
		displayConfigWindow->addItem(modeToggler);

		//CURRENT_MODE_OPTION = TWO LABELS

		//RESOLUTION_OPTION: LABEL + SELECT TOGGLER

		//GAMMA
		Widget* gammaOption = new Widget();
		gammaOption->name = "GAMMA_OPTION";
		gammaOption->rect = { 0, 150, 640, 30 };
		Label* gammaLabel = gammaOption->createChild<Label>("Gamma", font);
		SliderWidget* gammaSlider = gammaOption->createChild<SliderWidget>("GammaValue", font);
		displayConfigWindow->addItem(gammaOption);

		//RESTORE_DEFAULTS
		Widget* restoreDefaultDisplay = new Widget();
		restoreDefaultDisplay->name = "RESTORE_DEFAULT_DISPLAY_OPTION";
		restoreDefaultDisplay->rect = { 0, 180, 640, 30 };
		Label* restoreDefaultDisplayLabel = restoreDefaultDisplay->createChild<Label>("RESTORE_DEFAULT_DISPLAY_LABEL", font);
		restoreDefaultDisplayLabel->setText("Restore Defaults");
		displayConfigWindow->addItem(restoreDefaultDisplay);

		tabWindows[0] = displayConfigWindow;

		//INPUT WINDOW
		ListBoxWidget* inputConfigWindow = container.createChild<ListBoxWidget>(4, 1);
		inputConfigWindow->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);

		tabWindows[1] = inputConfigWindow;

		//AUDIO WINDOW
		ListBoxWidget* audioConfigWindow = container.createChild<ListBoxWidget>(3, 1);
		audioConfigWindow->setFlag(WidgetStateFlags::WIDGET_INTERACTIVE);
		tabWindows[2] = audioConfigWindow;
	}

	OptionsMenu::~OptionsMenu() = default;

	void OptionsMenu::enter() {}
	void OptionsMenu::pause() {}
	void OptionsMenu::resume() {}
	void OptionsMenu::leave() {}
	bool OptionsMenu::onEvent(AEvent* ev) { return container.onEvent(ev); }
	void OptionsMenu::run() { container.run(); }
	void OptionsMenu::draw() { container.render(); }
	bool OptionsMenu::isTransitioning() const { return false; }
}