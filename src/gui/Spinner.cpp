#include "Spinner.h"
#include "Label.h"
#include "Graphic.h"
#include "GraphicButton.h"
#include "Window.h"
#include "../framework/Lexer.h"
#include "../framework/InputManager.h"
#include "../framework/ResourceManager.h"
#include "../renderer/Font.h"

namespace Arboria {
	Spinner::Spinner(Window* gui) : Widget(gui), leftArrow(NULL), rightArrow(NULL), displayLabel(NULL)
	{
		leftArrow = new GraphicButton(gui, resourceManager->loadTexture(2));
		leftArrow->size = { 32, 32 };
		leftArrow->setParent(this);
		leftArrow->setClickable(true);
		rightArrow = new GraphicButton(gui, resourceManager->loadTexture(3));
		rightArrow->size = { 32, 32 };
		rightArrow->setParent(this);
		rightArrow->align(HorizontalAlignment::HOR_RIGHT);
		rightArrow->setClickable(true);
	}

	Spinner::~Spinner() = default;

	bool Spinner::onEvent(AEvent* e)
	{
		Widget::onEvent(e);

		int action = inputManager->getBinding(e->eventValue);
		if (e->eventType >= EventType::EVENT_KEY && e->eventType <= EventType::EVENT_CONTROLLER_BUTTON && e->eventValue2 == 1) {
			switch (action) {
			case ACTION_LEFTARROW:
				setSelectedOption(selIndex - 1);
				e->isHandled = true;
				break;
			case ACTION_RIGHTARROW:
				setSelectedOption(selIndex + 1);
				e->isHandled = true;
				break;
			default:
				break;
			}

			if (e->eventValue == ArboriaKey_MouseLeft) {
				if (leftArrow && leftArrow->isVisible() && leftArrow->containsPos(gui->getCursorX(), gui->getCursorY())) {
					setSelectedOption(selIndex - 1);
					e->isHandled = true;
				}
				if (rightArrow && rightArrow->isVisible() && rightArrow->containsPos(gui->getCursorX(), gui->getCursorY())) {
					setSelectedOption(selIndex + 1);
					e->isHandled = true;
				}
			}
		}

		return e->isHandled;
	}

	void Spinner::onRender()
	{
		Widget::onRender();
	}

	void Spinner::run()
	{
		Widget::run();
	}

	void Spinner::unloadResources()
	{
		Widget::unloadResources();
	}

	void Spinner::setSelectedOption(int idx)
	{
		selIndex = idx < 0 ? options.getLength() - 1 : idx % options.getLength();
		setDirty();
		displayLabel->setText(options[idx].optionName);
	}

	void Spinner::insertOption(const char* optName, void* _data)
	{
		options.insert({ optName, _data });
	}

	void Spinner::setOption(int idx, const char* optName, void* _data)
	{
		if (idx >= options.getLength()) {
			Engine::printError("Spinner::setOption: %i index out of bounds", idx);
			return;
		}
		options[idx] = { optName, _data };
	}

	bool Spinner::parse(Lexer* src, bool rebuild)
	{
		bool ret = Widget::parse(src, rebuild);
		rightArrow->position.x = size.x - 32;
		return ret;
	}

	//
	bool Spinner::parseInternalValue(const char* _name, Lexer* src)
	{
		bool ret = Widget::parseInternalValue(_name, src);
		if (String::iCompare(_name, "options") == 0) {
			parseString(src, optionsString);
		}
		else if (String::iCompare(_name, "font") == 0) {
			if (displayLabel == NULL) {
				displayLabel = new Label(gui);
				displayLabel->size = size - 64;
				//displayLabel->setDirty();
			}
			parseFont(src);
			displayLabel->setParent(this);
			displayLabel->halign = HorizontalAlignment::HOR_CENTER;
			displayLabel->valign = VerticalAlignment::VERT_CENTER;
			displayLabel->align(HorizontalAlignment::HOR_CENTER);
			displayLabel->setVisibility(true);
		}
		return ret;
	}

	void Spinner::parseFont(Lexer* src) {
		Token tok;
		int fontSize = 0;
		Token tok2;

		if (!src->expectTokenType(TOKENTYPE_STRING, 0, &tok)) {
			Engine::printError("Could not read the expected font id value");
			return;
		}
		tok2 = tok.c_str();
		src->expectAnyToken(&tok);
		if (String::iCompare(tok, "fontSize") == 0) {
			src->readToken(&tok);
			fontSize = atoi(tok);
		}

		Font* out = resourceManager->loadFont(tok2, fontSize);
		displayLabel->setFont(out);
		return;
	}

}