#ifndef WIDGET_H
#define WIDGET_H

#include "../containers/HashTable.h"
#include "../events/Event.h"
#include "../framework/String.h"
#include "WidgetEnums.h"
#include "../utils/Vector.h"
#include "../utils/Color.h"

namespace Arboria {
	class Image;
	class Surface;
	class Palette;
	class Window;
	class Lexer;
	typedef bool (*windowCallback)(Widget*, AEvent*);
	
	class Widget {
	private:
		typedef void (*preRenderCallback)(Widget*);
		Surface* surface;
		Palette* palette;
		void* data;
		preRenderCallback preRenderFunction;
	protected:
		uint32_t flags;
		int lastTimeRun;
		bool clickable;
		bool visible;
		windowCallback callbacks[12]{ NULL };
		
		Color backgroundColor;
		Color foregroundColor;
		Color selectColor;
		Color borderColor;
		float borderSize;
		Vector2<float> maxScale;
		String name;

		Widget* parent;
		List<Widget*> children;
		
		Widget* focusedChild;
		bool hovered = false;
		
		Window* gui;
	public:
		Widget();
		Widget(Window* gui);

		Vector2<int> position, size;

		List<Widget*>& getChildren() { return children; }
		Widget* findChildAtPoint(int x, int y, Widget* below = nullptr);
		int findChildIndex(Widget* widget);
		int getChildCount() const { return children.getLength(); }
		Widget* getChild(int index);
		void removeChild(Widget* widget);
		bool insertChild(Widget* widget, Widget* before);
		Widget* getFocusedChild();
		Widget* setFocus(Widget* widget);

		Window* getGui() { return gui; }
		void setGui(Window* w) { gui = w; }

		uint32_t getFlags() const { return flags; }
		void setFlag(unsigned int f);
		void clearFlag(unsigned int f);
		void toggleFlag(unsigned int f);
		windowCallback* getCallbacks() { return callbacks; }
		Color getBackgroundColor() const { return backgroundColor; }
		void setBackgroundColor(Color& _color) { backgroundColor = _color; }
		Color getForegroundColor() const { return foregroundColor; }
		void setForegroundColor(Color& _color) { foregroundColor = _color; }
		Color getSelectColor() const { return selectColor; }
		void setSelectColor(Color& _color) { selectColor = _color; }
		Color getBorderColor() const { return borderColor; }
		void setBorderColor(Color& _color) { borderColor = _color; }
		float getBorderSize() const { return borderSize; }
		void setBorderSize(float sz) { borderSize = sz; }
		float getMaxScaleX() const { return maxScale.x; }
		void setMaxScaleX(float x) { maxScale.x = x; }
		float getMaxScaleY() const { return maxScale.y; }
		void setMaxScaleY(float y) { maxScale.y = y; }
		Vector2<float> getMaxScale() const { return maxScale; }
		bool isDirty() const { return (flags & WidgetStateFlags::WIDGET_DIRTY) != 0; }
		bool isVisible() const { return visible; }
		void setVisibility(bool visibility) { visible = visibility; }
		void setParent(Widget* _parent);
		void setParent(Widget* _parent, int pos);
		Widget* getParent() const { return parent; }
		Surface* getSurface() const { return surface; }
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
		void render();
		Palette* getPalette() const { return palette; }
		void setPalette(Palette* p) { palette = p; }
		bool containsPos(int x, int y);
		Vector2<int> getActualPosition();
		String& getName() { return name; }

		virtual bool routeCursorCoordinates(); //this is primarily used to deal with mouse position, but it's also meant to be overwritten by tables/lists to handle directional inputs

		template<typename T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool> = true>
		T* findWidget(String widgetId) {
			for (int j = 0; j < children.getLength(); j++) {
				if (children[j]->name == widgetId)
					return static_cast<T*>(children[j]);
			}

			return NULL;
		}

		template<typename T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool> = true, typename... Args>
		T* createChild(Args&&...args) {
			T* newWidget = new T(gui, args...);
			newWidget->setParent(this);
			return newWidget;
		}

		void addCallback(int actionType, windowCallback callback);
		void executeCallback(int actionType, AEvent* ev);
		virtual void run();
		virtual bool onEvent(AEvent* e);
		virtual void onRender();
		virtual void preRender();
		virtual void postRender();
		virtual void unloadResources();
		void setPreRenderFunction(preRenderCallback cb) { preRenderFunction = cb; }
		virtual ~Widget();

		bool operator==(const Widget& other) const {
			return name == other.name;
		}

		bool parse(Lexer* src, bool rebuild = true);
		virtual bool parseInternalValue(const char* name, Lexer* src);
		void parseVect2i(Lexer* src, Vector2<int>& out);
		void parseVect2f(Lexer* src, Vector2<float>& out);
		void parseColor(Lexer* src, Color& out);
		void parseString(Lexer* src, String& out);
		void parsePalette(Lexer* src, Palette* out);
		void parseAlignment(Lexer* src, VerticalAlignment& out1, HorizontalAlignment& out2);
		virtual void parseImage(Lexer* src, Image* img);
		//virtual configureFromFile(const char* fileData);
	};

};

#endif