#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "WidgetEnums.h"
#include "../containers/HashTable.h"
#include "../containers/List.h"
#include "../utils/Vector.h"

namespace Arboria {
	class WidgetEvent;
	class Event;
	class Texture;
	typedef void (*widgetCallback)(WidgetEvent*);
	class Widget {
		private:
			HashTable<WidgetEventType, List<widgetCallback>> callbacks;
			Texture* surface;
			void* data;
		protected:
			Widget* parent;
			Vector2i resolvedLocation; //actual location derived from the parent.
			bool clickable;
			bool visible;
			bool dirty;
			bool mouseInside;
			bool mousePressed;
			void submitGuiEvent(WidgetEventType weType, Event* _parent);
			void triggerCallbacks(WidgetEvent* event);
		public:
			bool enabled;
			char* name;
			Vector2i location; //relative
			Vector2i size;
			Vector2i selectableSize;
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
			Vector2i getParentSize() const;
			static int align(HorizontalAlignment _halign, int parentWidth, int childWidth);
			static int align(VerticalAlignment _valign, int parentWidth, int childWidth);
			int align(HorizontalAlignment _halign);
			int align(VerticalAlignment _valign);
			int align(HorizontalAlignment _halign, VerticalAlignment _valign);
			template<typename T> T* getData() const { return static_cast<T*>(data); }
			void setData(void* _data) { data = _data; }
			void setDirty();
			void resolveLocation();
			void render();
			void addCallback(WidgetEventType eType, widgetCallback callback);
			virtual void run();
			virtual void eventOccured(Event* e);
			virtual void onRender();
			virtual void preRender();
			virtual void postRender();
			virtual ~Widget();
	};
}

#endif