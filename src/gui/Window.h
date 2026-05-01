#ifndef WINDOW_H
#define WINDOW_H

#include "Widget.h"
#include "../containers/Stack.h"
#include <typeinfo>

namespace Arboria {

	class Window {
		friend class WindowManager;
	public:
		Window();
		virtual ~Window();

		const char* getName() const { return name.c_str(); }
		bool isActive() const { return active; }
		bool isInteractive() const { return interactive; }
		bool isUnique() const { return unique; }
		void setActive(bool _active) { active = _active; }
		void setInteractivity(bool _interactive) { interactive = _interactive; }
		void setUnique(bool _unique) { unique = _unique; }
		void setName(String& _name) { name = _name; }
		void setName(const char* _name) { name = _name; }
		int getCursorX() const { return cursorPos.x; }
		int getCursorY() const { return cursorPos.y; }
		void setCursor(int x, int y);

		virtual void onEvent(AEvent* e);
		virtual void render();

		Widget* getRoot() const { return root; }
		void setRoot(Widget* w) { root = w; }
		bool initializeFromFile(const char* fpath, bool rebuild = true);
		virtual bool postParse();
		bool operator==(const Window& other) const {
			if (typeid(*this) != typeid(other))
				return false;
			return compare(other);
		}
		bool isGameGui = false;
	protected:
		virtual bool compare(const Window& other) const {
			return this == &other;
		}
		Widget* root;
	private:
		bool active;
		bool interactive;
		bool unique;
		Vector2<int> cursorPos = { 0,0 };

		String name;
	};

	class WindowManager {
		friend class Window;
	public:
		void shutdown();
		Widget* getFocusedWidget();
		Window* getFocusedWindow();
		void setFocus(Widget* widget); //adjust the focuses of the last widget on the tack and the widget being submitted, then push onto stack
		void setFocus(Widget& widget);
		void popFocus();
		bool checkWindow(String& wname) const;
		Window* findWindow(String& wname, bool autoLoad = false, bool forceInteractive = false);
		List<Window*>& getGuis() { return guis; }
	private:
		List<Window*> guis;
		Stack<WidgetPtrWrapper> focusStack;
	};

	struct WidgetPtrWrapper {
		Widget* ptr;

		WidgetPtrWrapper() : ptr(NULL) {}
		WidgetPtrWrapper(Widget& widget) : ptr(&widget) {}
		WidgetPtrWrapper(Widget* widget) : ptr(widget) {}

		~WidgetPtrWrapper() {
			ptr = nullptr; //have it point to nothing.
		}
	};

	extern WindowManager* guiManager;
}

#endif