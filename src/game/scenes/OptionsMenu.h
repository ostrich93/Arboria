#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "../../gui/Window.h"

namespace Arboria {
	class ListBoxWidget;
	class Spinner;
	class GraphicButton;

	enum class OptionsMenuState {
		TabSelect,
		ListSelect,
		ResolutionSelect,
		BindingSelect,
		BindingWait
	};

	struct ResolutionOptions {
		String description;
		int32_t x;
		int32_t y;
	};

	struct VsyncOption {
		String description;
		bool isOn;
	};

	struct BindingData {
		int key;
		int gamepad;
		int action;
	};

	class OptionsMenu : public Window {
		private:
			ListBoxWidget* tabbedList;
			Widget* tabbedWindows[2];
			Widget* currentWindow;
			Widget* bindWindow;
			bool inBindWindow;
			bool waitingBind; //when changing the key/button bindings
			uint8_t selectTab;
			OptionsMenuState menuState = OptionsMenuState::TabSelect;
			uint8_t selectResolutionIndex = 0;
			ResolutionOptions* selectedResolutionOption;
			bool displayPending = false; //true if display changes are made but not applied/saved
			void setKeyImageFromAction(int action, GraphicButton* bindBtn);
			void setButtonImageFromAction(int action, GraphicButton* bindBtn);
		public:
			OptionsMenu();
			~OptionsMenu() override;
			void render() override;
			void onEvent(AEvent* ev) override;
			bool postParse() override;
			bool parseBindings();
			void resetOptions();
			void restoreDefaultSettings();
			void restoreDefaultBindings();
			bool isDisplayChangePending() const { return displayPending; }
			void setDisplayChangePending(bool _dis) { displayPending = _dis; }

			ListBoxWidget* getTabbedList() { return tabbedList; }

			uint8_t getSelectedResolutionIndex() { return selectResolutionIndex; }
			void setSelectedResolutionIndex(int8_t v) { selectResolutionIndex = v; }

			template<typename T>
			void parseSpinnerOptions(Spinner* spinner, List<T> optionData);
		protected:
			bool compare(const Window& other) const override {
				if (OptionsMenu* ptr = dynamic_cast<OptionsMenu*>(const_cast<Window*>(&other))) {
					return root == ptr->root;
				}
			}
	};

	bool handleApplyDisplayChanges(Widget* w, AEvent* ev);
	bool handleRestoreDefaultDisplaySettings(Widget* w, AEvent* ev);
	bool handleBindingSelect(Widget* w, AEvent* ev);
	bool returnToTabList(Widget* w, AEvent* ev);
	bool exitOptionsMenu(Widget* w, AEvent* ev);
	bool handleSelectedTabChange(Widget* w, AEvent* ev);
	bool handleReturnToTabList(Widget* w, AEvent* ev);
	bool returnToDisplayList(Widget* w, AEvent* ev);
	bool handleRestoreDefaultBindings(Widget* w, AEvent* ev);
}

#endif