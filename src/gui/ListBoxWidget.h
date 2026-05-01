#ifndef LISTBOX_WIDGET_H
#define LISTBOX_WIDGET_H

#include "ScrollbarWidget.h"

namespace Arboria {
	class ListBoxWidget : public Widget {
		private:
			Widget* hovered;
			Widget* selected;
			Color hoverColor;
			Color selectColor;
			Color regularColor;
			uint32_t itemCount = 0;
			uint32_t displayCount = 1;
			uint32_t indexOffset = 0;
			int scrollOffset = 0;
		public:
			ListBoxWidget(Window* _gui, Orientation _orientation = Orientation::VERTICAL, uint32_t elemCount = 0);
			~ListBoxWidget() override;
			bool onEvent(AEvent* e) override;
			void onRender() override;
			void postRender() override;
			void run() override;

			void clear();
			void addItem(Widget* item);
			//Widget* removeItem(Widget* item);
			//Widget* removeItem(int index);
			Widget* operator[](int index) { return children[index]; }

			void configureScrollbar(Orientation _orientation);

			void setSelected(Widget* widget);
			Widget* getSelected() { return selected; }
			Widget* getHovered() { return hovered; }
			void setHoverColor(Color newColor) { hoverColor = newColor; }
			Color getHoverColor() const { return hoverColor; }
			Color setSelectColor(Color newColor) { selectColor = newColor; }
			Color getSelectColor() const { return selectColor; }
			Color setRegularColor(Color newColor) { regularColor = newColor; }
			Color getRegularColor() const { return regularColor; }

			bool parseInternalValue(const char* _name, Lexer* src) override;
			void updateScrollbarInfo();
			
			int cursor;
			int itemSize;
			int itemSpacing;
			Orientation viewOrientation;
			ScrollbarViewPolicy viewPolicy;
			ScrollbarWidget* scrollbar;
	};
}

#endif