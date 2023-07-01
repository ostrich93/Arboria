#ifndef LISTBOX_WIDGET_H
#define LISTBOX_WIDGET_H

#include "ScrollAreaWidget.h"

namespace Arboria {
	class ListBoxWidget : public ScrollAreaWidget {
		private:
			Widget* hovered;
			Widget* selected;
			Color hoverColor;
			Color selectColor;
			Color regularColor;
			uint32_t itemCount = 0;
		public:
			ListBoxWidget();
			ListBoxWidget(ScrollbarWidget* widget);
			ListBoxWidget(int rowNum = 1, int colNum = 1);
			~ListBoxWidget() override;
			bool onEvent(AEvent* e) override;
			void onRender() override;
			void postRender() override;
			void run() override;

			void addItem(Widget* item);
			//Widget* removeItem(Widget* item);
			//Widget* removeItem(int index);
			Widget* operator[](int index) { return children[index]; }

			void setSelected(Widget* widget);
			Widget* getSelected() { return selected; }
			Widget* getHovered() { return hovered; }
			void setHoverColor(Color newColor) { hoverColor = newColor; }
			Color getHoverColor() const { return hoverColor; }
			Color setSelectColor(Color newColor) { selectColor = newColor; }
			Color getSelectColor() const { return selectColor; }
			Color setRegularColor(Color newColor) { regularColor = newColor; }
			Color getRegularColor() const { return regularColor; }
			
			int cursor;
			int itemSize;
			int itemSpacing;
	};
}

#endif