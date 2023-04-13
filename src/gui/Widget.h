#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "../containers/HashTable.h"
#include "../framework/String.h"
#include "WidgetEnums.h"

#include "../utils/Vector.h"

namespace Arboria {

	class WidgetEvent;
	class Event;
	class Texture;
	typedef void (*widgetCallback)(WidgetEvent*);
	class Widget {
	private:
		typedef void (*preRenderCallback)(Widget*);
		HashTable<WidgetEventType, List<widgetCallback>> callbacks;
		Texture* surface;
		void* data;
		preRenderCallback preRenderFunction;
	protected:
		Widget* parent;
		Vector2<int> resolvedLocation; //actual location derived from the parent.
		bool clickable;
		bool visible;
		bool dirty;
		//bool mouseInside;
		//bool mousePressed;
		void submitGuiEvent(WidgetEventType weType, Event* _parent);
		void triggerCallbacks(WidgetEvent* event);
	public:
		Widget(bool focused = true);
		bool enabled;
		String name;
		Vector2<int> location; //relative
		Vector2<int> size;
		Vector2<int> selectableSize;
		List<Widget> children;
		bool hasFocus;
		bool isPointInsideSelectBoundaries(int x, int y) const;
		bool isDirty() const { return dirty; }
		bool isVisible() const { return visible; }
		bool isClickable() const { return clickable; }
		void setVisible(bool _visibility);
		void setClickable(bool _clickable) { clickable = _clickable; }
		void setParent(Widget* _parent);
		Widget* getParent() const { return parent; }
		Texture* getSurface() const { return surface; }
		void setSurface(Texture* _surface) { surface = _surface; }
		Vector2<int> getParentSize() const;
		static int align(HorizontalAlignment _halign, int parentWidth, int childWidth);
		static int align(VerticalAlignment _valign, int parentHeight, int childHeight);
		void align(HorizontalAlignment _halign);
		void align(VerticalAlignment _valign);
		void align(HorizontalAlignment _halign, VerticalAlignment _valign);
		template<typename T> T* getData() const { return static_cast<T*>(data); }
		Widget* getPointOfAncestry(Widget* parent);
		void* getData() const { return data; }
		void setData(void* _data) { data = _data; }
		void setDirty();
		void resolveLocation();
		void render();

		template<typename T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool> = true>
		T* findWidget(String& widgetId) {
			for (int j = 0; j < children.getLength(); j++) {
				if (children[j].name == widgetId)
					return static_cast<T*>(&children[j]);
			}

			return NULL;
		}

		template<typename T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool> = true, typename... Args>
		T* createChild(Args&&...args) {
			T* newWidget = new T(args...);
			newWidget->setParent(this);
			return newWidget;
		}

		void addCallback(WidgetEventType eType, widgetCallback callback);
		virtual void run();
		virtual void eventOccured(Event* e);
		virtual void onRender();
		virtual void preRender();
		virtual void postRender();
		void setPreRenderFunction(preRenderCallback cb) { preRenderFunction = cb; }
		virtual ~Widget();

		bool operator==(const Widget& other) const {
			return name == other.name;
		}
	};
};

#endif