#ifndef SPINNER_H
#define SPINNER_H

#include "Widget.h"

namespace Arboria {
	class GraphicButton;
	class Label;

	class Spinner : public Widget {
	private:
		Label* displayLabel;
		GraphicButton* leftArrow;
		GraphicButton* rightArrow;
		List<SpinnerOption> options;

		String optionsString;
		int selIndex = 0;

	public:
		Spinner(Window* gui);
		~Spinner() override;

		bool onEvent(AEvent* e) override;
		void onRender() override;
		void run() override;
		void unloadResources() override;

		int getSelectedIndex() const { return selIndex; }
		SpinnerOption& getSelectedOption() { return options[selIndex]; }
		void* getSelectedData() const { return options[selIndex].optionData; }
		void setSelectedOption(int idx);

		String& getOptionsString() { return optionsString; }
		void setOptionsString(String& val) { optionsString = val; }
		void setOptionsString(const char* val) { optionsString = val; }

		void setOption(int idx, const char* optName, void* _data);
		List<SpinnerOption>& getOptions() { return options; }

		bool parseInternalValue(const char* _name, Lexer* src) override;
	};

	struct SpinnerOption {
		const char* optionName;
		void* optionData;
		
		SpinnerOption(const char* str) : optionName(str), optionData(NULL) {}
		SpinnerOption(const char* str, void* data) : optionName(str), optionData(data) {}
	};
}

#endif