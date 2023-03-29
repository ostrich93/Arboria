#ifndef __LISTBOX_WIDGET_H__
#define __LISTBOX_WIDGET_H__

#include "ScrollAreaWidget.h"

namespace Arboria {
	class ListBoxWidget : public ScrollAreaWidget {
		private:
			Widget* hovered;
			Widget* selected;
			Vector4<int> hoverColor;
			Vector4<int> selectColor;
			Vector4<int> regularColor;
		public:
			ListBoxWidget();
			ListBoxWidget(ScrollbarWidget* widget);
			~ListBoxWidget() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void postRender() override;
			void run() override;

			void addItem(Widget* item);
			Widget* removeItem(Widget* item);
			Widget* removeItem(int index);
			Widget* operator[](int index) { return &children[index]; }

			void setSelected(Widget* widget);
			Widget* getSelected() { return selected; }
			Widget* getHovered() { return hovered; }
			void setHoverColor(Vector4<int> newColor) { hoverColor = newColor; }
			Vector4<int> getHoverColor() const { return hoverColor; }
			Vector4<int> setSelectColor(Vector4<int> newColor) { selectColor = newColor; }
			Vector4<int> getSelectColor() const { return selectColor; }
			Vector4<int> setRegularColor(Vector4<int> newColor) { regularColor = newColor; }
			Vector4<int> getRegularColor() const { return regularColor; }
			
			int cursor;
			int itemSize;
			int itemSpacing;
	};
}

#endif