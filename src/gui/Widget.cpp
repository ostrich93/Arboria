#include "./Widget.h"
#include "../framework/InputActionType.h"
#include "../framework/ActionManager.h"
#include "../renderer/Renderer.h"

namespace Arboria {
	bool Widget::onEvent(AEvent* e)
	{
		if (flags & WidgetStateFlags::WIDGET_VISIBLE) {
			if (flags & WidgetStateFlags::WIDGET_INTERACTIVE) {
				for (int i = 0; i < children.getLength(); i++) {
					children[i]->onEvent(e);
					if (e->isHandled) {
						setDirty();
						return true;
					}
				}
				
				int action = actionManager->getAction(*e);
				if (action > 0 && callbacks[action - 1] != NULL) {
					setDirty();
					callbacks[action - 1](this);
					return true;
				}

				return false;
			}
		}
		return e->isHandled;
	}

	void Widget::onRender() {}

	Widget::Widget() : deviceContext(renderDevice), maxScale({ 1,1 }), name(""), parent(NULL), preRenderFunction(NULL), flags(0), borderSize(0), borderColor({ 0, 0, 0, 0 }), data(NULL) {
	}

	void Widget::setFlag(unsigned int f)
	{
		flags |= static_cast<uint32_t>(f);
	}

	void Widget::clearFlag(unsigned int f) {
		flags & ~static_cast<uint32_t>(f);
	}

	void Widget::toggleFlag(unsigned int f) {
		flags ^ static_cast<uint32_t>(f);
	}

	void Widget::setDirty() {
		setFlag(WidgetStateFlags::WIDGET_DIRTY);
		if (parent) {
			parent->setDirty();
		}
	}

	void Widget::resolveLocation() {
		auto previousLocation = resolvedLocation;
		auto parentControl = parent;

		if (parentControl == NULL) {
			resolvedLocation.x = rect.x;
			resolvedLocation.y = rect.y;
		}
		else {
			if (rect.x > parentControl->rect.w || rect.y > parentControl->rect.h) {
				resolvedLocation.x = -99999;
				resolvedLocation.y = -99999;
			}
			else {
				resolvedLocation.x = rect.x + parentControl->resolvedLocation.x;
				resolvedLocation.y = rect.y + parentControl->resolvedLocation.y;
			}
		}
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = *children[i];
			child.resolveLocation();
		}
		if (previousLocation != resolvedLocation) {
			setDirty();
		}
	}

	void Widget::render()
	{
		if (!(flags & WidgetStateFlags::WIDGET_VISIBLE) || rect.w == 0 || rect.h == 0) {
			return;
		}

		if (flags & WidgetStateFlags::WIDGET_DIRTY) {
			onRender();
			postRender();
		}

		clearFlag(WidgetStateFlags::WIDGET_DIRTY);

		renderer->draw(surface, { rect.x, rect.y }, { backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a });
		//spriteRenderer->draw(surface, { rect.x, rect.y }, { rect.w, rect.h }, { backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a });
		
	}

	void Widget::preRender()
	{
		for (int i = 0; i < children.getLength(); i++) {
			children[i]->preRender();
		}
		if (preRenderFunction != NULL) {
			preRenderFunction(this);
		}
	}

	void Widget::postRender() {
		Widget child;
		for (int i = 0; i < children.getLength(); i++) {
			child = *children[i];
			if (child.isVisible()) {
				child.render();
			}
		}
	}

	Widget::~Widget()
	{
	}

	void Widget::setParent(Widget* _parent) {
		if (_parent) {
			_parent->children.append(this);
			_parent->setDirty();
		}
		parent = _parent;
	}

	int Widget::align(HorizontalAlignment _halign, int parentWidth, int childWidth) {
		int x = 0;
		switch (_halign) {
			case (HorizontalAlignment::HOR_LEFT):
				x = 0;
				break;
			case (HorizontalAlignment::HOR_CENTER):
				x = (parentWidth / 2) - (childWidth / 2);
				break;
			case (HorizontalAlignment::HOR_RIGHT):
				x = parentWidth - childWidth;
				break;
		}
		return x;
	}

	int Widget::align(VerticalAlignment _valign, int parentHeight, int childHeight) {
		int y = 0;
		switch (_valign) {
			case (VerticalAlignment::VERT_TOP):
				y = 0;
				break;
			case (VerticalAlignment::VERT_CENTER):
				y = (parentHeight / 2) - (childHeight / 2);
				break;
			case (VerticalAlignment::VERT_BOTTOM):
				y = parentHeight - childHeight;
				break;
		}
		return y;
	}

	void Widget::align(HorizontalAlignment _halign) {
		rect.x = this->align(_halign, parent->rect.w, rect.w);
	}

	void Widget::align(VerticalAlignment _valign) {
		rect.y = this->align(_valign, parent->rect.h, rect.h);
	}

	void Widget::align(HorizontalAlignment _halign, VerticalAlignment _valign) {
		align(_halign);
		align(_valign);
	}

	Widget* Widget::getPointOfAncestry(Widget* parent)
	{
		Widget* poa = this;
		while (poa != NULL) {
			if (poa->getParent() == parent) break;
			else poa = poa->getParent();
		}
		return poa;
	}

	void Widget::addCallback(int actionType, windowCallback callback)
	{
		if (actionType > 0) {
			callbacks[actionType - 1] = callback;
		}
	}

	void Widget::run() {
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = *children[i];
			child.run();
		}
	}
}