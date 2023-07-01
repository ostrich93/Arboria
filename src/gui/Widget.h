#ifndef WIDGET_H
#define WIDGET_H

#include "../containers/HashTable.h"
#include "../events/Event.h"
#include "../framework/String.h"
#include "WidgetEnums.h"
#include "../utils/Vector.h"
#include "../utils/Rectangle.h"
#include "../utils/Color.h"

namespace Arboria {
	class Texture;
	typedef void (*windowCallback)(Widget*);
	
	class Widget {
	private:
		typedef void (*preRenderCallback)(Widget*);
		Texture* surface;
		void* data;
		preRenderCallback preRenderFunction;
	protected:
		Widget* parent;
		windowCallback callbacks[12]{ NULL };
		Vector2<int> resolvedLocation; //actual location derived from the parent.
		Color backgroundColor;
		Color foregroundColor;
		Color selectColor;
		Color borderColor;
		bool isContainer;
		float borderSize;
		Vector2<float> maxScale;
		RenderDevice* deviceContext;
		uint32_t flags;
	public:
		Widget();
		bool enabled;
		String name;
		Rectangle rect;
		List<Widget*> children;

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
		bool isDirty() const { return flags & WidgetStateFlags::WIDGET_DIRTY != 0; }
		bool isVisible() const { return flags & WidgetStateFlags::WIDGET_VISIBLE != 0; }
		void setParent(Widget* _parent);
		Widget* getParent() const { return parent; }
		Texture* getSurface() const { return surface; }
		void setSurface(Texture* _surface) { surface = _surface; }
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

		void addCallback(int actionType, windowCallback callback);
		virtual void run();
		virtual bool onEvent(AEvent* e);
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