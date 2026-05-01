#include "Spinner.h"
#include "Label.h"
#include "Graphic.h"
#include "../framework/Lexer.h"

namespace Arboria {
	Spinner::Spinner(Window* gui) : Widget(gui)
	{
	}

	void Spinner::setSelectedOption(int idx)
	{
		if (idx > 0 && idx < options.getLength()) {
			selIndex = idx;
			setDirty();
			displayLabel->setText(options[idx].optionName);
		}
	}

	void Spinner::setOption(int idx, const char* optName, void* _data)
	{
		if (idx >= options.getLength()) {
			Engine::printError("Spinner::setOption: %i index out of bounds", idx);
			return;
		}
		options.insert({ optName, _data }, idx);
	}

	//
	bool Spinner::parseInternalValue(const char* _name, Lexer* src)
	{
		bool ret = Widget::parseInternalValue(_name, src);
		if (String::iCompare(_name, "options") == 0) {
			parseString(src, optionsString);
		}
		return ret;
	}

}