#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "../../framework/UIContext.h"
#include "../../gui/Widget.h"

namespace Arboria {
	class ListBoxWidget;

	enum class OptionsMenuState {
		TabSelect,
		ListSelect,
		ResolutionSelect,
		BindingSelect,
		BindingWait
	};

	struct ResolutionOptions {
		String description;
		int32_t xScale;
		int32_t yScale;
	};

	class OptionsMenu : public UIContext {
		private:
			Widget container;
			ListBoxWidget* tabbedList;
			ListBoxWidget* tabbedWindows[2];
			ListBoxWidget* currentWindow;
			Widget* bindWindow;
			bool inBindWindow;
			bool waitingBind; //when changing the key/button bindings
			uint8_t selectTab;
			OptionsMenuState menuState = OptionsMenuState::TabSelect;
			uint8_t selectResolutionIndex = 0;
			ResolutionOptions selectedResolutionOption;
			void buildDisplayWidow();
			void buildInputWindow();
			void buildInputRow(ListBoxWidget* configWindow, String actionName, int actionValue);
			void buildTabList();
		public:
			OptionsMenu();
			~OptionsMenu() override;
			void draw() override;
			void enter() override;
			bool onEvent(AEvent* ev) override;
			void leave() override;
			void pause() override;
			void resume() override;
			void run() override;
			bool isTransitioning() const override;
			void sendSelectOption(Widget* w);

			ListBoxWidget* getTabbedList() { return tabbedList; }
			Widget& getContainer() { return container; }

			uint8_t getSelectedResolutionIndex() { return selectResolutionIndex; }
			void setSelectedResolutionIndex(int8_t v) { selectResolutionIndex = v; }
	};

	void handleOptionsHoverTabChange(AEvent* e, void* context);
	void handleOptionsSelectTabChange(AEvent* e, void* context);
	void handleTabbedWindowCancelChange(AEvent* e, void* context);
	void handleDisplayWindowOptionSelectChange(AEvent* e, void* context);
	void handleApplyResolutionChanges(AEvent* e, void* context);
	void handleOptionsRestoreDefaultBindings(AEvent* e, void* context);
}

#endif