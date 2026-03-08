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
	typedef void (*windowCallback)(Widget*);
	
	class Widget {
	private:
		typedef void (*preRenderCallback)(Widget*);
		Surface* surface;
		Palette* palette;
		void* data;
		HashTable<GUIEventType, List<eventCallback>> eventCallbacks;
		preRenderCallback preRenderFunction;
	protected:
		Widget* parent;
		windowCallback callbacks[12]{ NULL };
		Vector2<int> resolvedLocation; //actual location derived from the parent.
		Color backgroundColor;
		Color foregroundColor;
		Color selectColor;
		Color borderColor;
		float borderSize;
		Vector2<float> maxScale;
		uint32_t flags;
	public:
		Widget();
		bool enabled;
		bool clickable;
		String name;
		Vector2<int> position, size;
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
		bool isDirty() const { return (flags & WidgetStateFlags::WIDGET_DIRTY) != 0; }
		bool isVisible() const { return (flags & WidgetStateFlags::WIDGET_VISIBLE) != 0; }
		void setParent(Widget* _parent);
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
		void resolveLocation();
		void render();
		Palette* getPalette() const { return palette; }
		void setPalette(const Palette* p);

		void submitGuiEvent(GUIEventType eventType, AEvent* parentEvent);
		void addEventCallback(GUIEventType eventType, eventCallback callback);
		void executeEventCallbacks(AEvent* e);

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
		virtual void unloadResources();
		void setPreRenderFunction(preRenderCallback cb) { preRenderFunction = cb; }
		virtual ~Widget();

		bool operator==(const Widget& other) const {
			return name == other.name;
		}
	};

	template<typename V>
	class HashNode<GUIEventType, V> {
	public:
		GUIEventType key;
		V value;
		HashNode<GUIEventType, V>* next;

		HashNode(const GUIEventType& _key, const V& _value) : key(_key), value(_value), next(NULL) {}
		HashNode(const GUIEventType& _key, const V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {}
		HashNode(GUIEventType& _key, V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {}

		static int getHash(const GUIEventType& key, const int maskValue) {
			return static_cast<int>(key) & maskValue;
		}

		static int compare(const GUIEventType& keyA, const GUIEventType& keyB) {
			if (keyA < keyB)
				return -1;

			else if (keyA > keyB)
				return 1;

			return 0;
		}
	};
};

#endif