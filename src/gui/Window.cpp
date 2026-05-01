#include "../game/scenes/MainMenu.h"
#include "../game/scenes/OptionsMenu.h"
#include "../framework/Lexer.h"
#include "../FileSystem.h"

namespace Arboria {
	WindowManager windowManager;
	WindowManager* guiManager = &windowManager;

	Window::Window() {
		active = false;
		interactive = false;
		unique = false;
		root = nullptr;
		name = "";
	}

	Window::~Window() {
		delete root;
		root = nullptr;
	}

	void Window::setCursor(int x, int y)
	{
		cursorPos.x = x;
		cursorPos.y = y;
	}

	void Window::onEvent(AEvent* e) {
		if (e->eventType == EventType::EVENT_MOUSE_MOVE) {
			cursorPos.x = e->eventValue;
			cursorPos.y = e->eventValue2;
		}
		else if (e->eventType == EventType::EVENT_CONTROLLER_AXIS) {
			if (e->eventValue < 4 && e->eventValue2 != 0) {
				float addValue = expf(fabsf(e->eventValue2) * 0.03f) * 0.5f;
				if (e->eventValue2 < 0)
					addValue = -addValue;
				switch (e->eventValue) {
				case AEvent::JoystickAxis::AXIS_LEFT_X:
					cursorPos.x += addValue;
					break;
				case AEvent::JoystickAxis::AXIS_LEFT_Y:
					cursorPos.y += addValue;
					break;
				default:
					break;
				}
			}

			if (cursorPos.x < 0)
				cursorPos.x = 0;
			if (cursorPos.y < 9)
				cursorPos.y = 0;
		}

		if (root) {
			root->onEvent(e);
		}
	}

	void Window::render() {
		if (active) {
			root->render();
		}
	}

	bool Window::initializeFromFile(const char* fpath, bool rebuild)
	{
		Lexer src(fpath);
		src.setFlags(LEXER_ALLOWBACKSLASHSTRINGCONCATS | LEXER_ALLOWNUMBERSTARTNAMES | LEXER_NOSTRINGCONCATS | LEXER_NOFATALERRORS);

		if (src.isLoaded()) {
			Token token;
			while (src.readToken(&token)) {
				if (String::iCompare(token, "widget") == 0) {
					if (root->parse(&src, rebuild)) {
						root->setFlag(WIDGET_ROOT);
					}
					continue;
				}
			}
		}
		else {
			return false;
		}

		postParse();
		if (windowManager.guis.find(this) == NULL)
			windowManager.guis.append(this);

		
		return true;
	}

	bool Window::postParse()
	{
		return true;
	}

	void WindowManager::shutdown() {
		guis.deleteContents(true);
	}

	Widget* WindowManager::getFocusedWidget() {
		if (focusStack.isEmpty())
			return nullptr;
		return focusStack.top().ptr;
	}

	Window* WindowManager::getFocusedWindow()
	{
		if (focusStack.isEmpty())
			return nullptr;
		return getFocusedWidget()->getGui();
	}

	void WindowManager::setFocus(Widget* widget)
	{
		if (widget == nullptr) {
			Engine::printError("WindowManager::setFocus: cannot push a null object to the focusStack.");
			return;
		}
		if (!focusStack.isEmpty()) getFocusedWidget()->clearFlag(WidgetStateFlags::WIDGET_FOCUSED);
		widget->setFlag(WidgetStateFlags::WIDGET_FOCUSED);
		focusStack.push(WidgetPtrWrapper(widget));
	}

	void WindowManager::setFocus(Widget& widget)
	{
		if (!focusStack.isEmpty()) getFocusedWidget()->clearFlag(WidgetStateFlags::WIDGET_FOCUSED);
		widget.setFlag(WidgetStateFlags::WIDGET_FOCUSED);
		focusStack.push(WidgetPtrWrapper(widget));
	}

	void WindowManager::popFocus()
	{
		focusStack.pop();
	}

	bool WindowManager::checkWindow(String& wname) const
	{
		for (int i = 0; i < guis.getLength(); i++) {
			if (guis[i]->getName() == wname)
				return true;
		}
		return false;
	}

	Window* WindowManager::findWindow(String& wname, bool autoLoad, bool forceInteractive)
	{
		for (int i = 0; i < guis.getLength(); i++) {
			if (String::iCompare(guis[i]->getName(), wname.c_str()) == 0) {
				if (forceInteractive && guis[i]->isInteractive()) {
					break;
				}
				return guis[i];
			}
		}

		if (autoLoad) {
			Window* gui;

			if (String::iCompare(wname.c_str(), "optionsmenu.gui") == 0)
				gui = new OptionsMenu();
			else if (String::iCompare(wname.c_str(), "mainmenu.gui") == 0)
				gui = new MainMenu();
			else
				gui = new Window();

			if (gui->initializeFromFile(wname.c_str())) {
				return gui;
			}
			else {
				delete gui;
			}
		}
		return nullptr;
	}


}