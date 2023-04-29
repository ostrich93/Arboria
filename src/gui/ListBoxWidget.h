#ifndef LISTBOX_WIDGET_H
#define LISTBOX_WIDGET_H

#include "ScrollAreaWidget.h"

namespace Arboria {
	class ListBoxWidget : public ScrollAreaWidget {
		private:
			Widget* hovered;
			Widget* selected;
			Vector4<float> hoverColor;
			Vector4<float> selectColor;
			Vector4<float> regularColor;
		public:
			ListBoxWidget();
			ListBoxWidget(ScrollbarWidget* widget);
			~ListBoxWidget() override;
			void eventOccured(Event* e) override;
			void onRender() override;
			void postRender() override;
			void run() override;

			void addItem(Widget* item);
			//Widget* removeItem(Widget* item);
			//Widget* removeItem(int index);
			Widget* operator[](int index) { return &children[index]; }

			void setSelected(Widget* widget);
			Widget* getSelected() { return selected; }
			Widget* getHovered() { return hovered; }
			void setHoverColor(Vector4<float> newColor) { hoverColor = newColor; }
			Vector4<float> getHoverColor() const { return hoverColor; }
			Vector4<float> setSelectColor(Vector4<float> newColor) { selectColor = newColor; }
			Vector4<float> getSelectColor() const { return selectColor; }
			Vector4<float> setRegularColor(Vector4<float> newColor) { regularColor = newColor; }
			Vector4<float> getRegularColor() const { return regularColor; }
			
			int cursor;
			int itemSize;
			int itemSpacing;
	};
}

#endif