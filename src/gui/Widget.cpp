#include "Widget.h"
#include "../events/Event.h"
#include "../globals.h"

namespace Arboria {
	void Widget::eventOccured(Event* e) {
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = children[i];
			if (child.visible && child.enabled) {
				child.eventOccured(e);
				if (e->isHandled) {
					return;
				}
			}
		}
		
		if (e->getEventType() == EventType::EVENT_MOUSE_MOVE || e->getEventType() == EventType::EVENT_MOUSE_DOWN || e->getEventType() == EventType::EVENT_MOUSE_UP) {
			auto mousey = dynamic_cast<MouseEvent*>(e);
			bool isNewPosInsideBounds = isPointInsideSelectBoundaries(mousey->getData().x, mousey->getData().y);
			if (e->getEventType() == EventType::EVENT_MOUSE_MOVE) {
				if (isNewPosInsideBounds) {
					if (!mouseInside) {
						submitGuiEvent(WidgetEventType::MOUSE_ENTER, e);
					}
					submitGuiEvent(WidgetEventType::MOUSE_MOVE, e);
				}
				else {
					if (mouseInside) {
						submitGuiEvent(WidgetEventType::MOUSE_LEAVE, e);
						mousePressed = false;
					}
				}
			}
			if (e->getEventType() == EventType::EVENT_MOUSE_DOWN) {
				if (isNewPosInsideBounds) {
					submitGuiEvent(WidgetEventType::MOUSE_DOWN, e);
					mousePressed = true;
					if (clickable) {
						e->isHandled = true;
					}
				}
			}
			if (e->getEventType() == EventType::EVENT_MOUSE_UP) {
				if (isNewPosInsideBounds) {
					submitGuiEvent(WidgetEventType::MOUSE_UP, e);
					if (mousePressed) {
						submitGuiEvent(WidgetEventType::MOUSE_CLICK, e);
					}
				}
				mousePressed = false;
			}
			mouseInside = isNewPosInsideBounds;
		}
		if ((e->getEventType() == EventType::EVENT_KEY_DOWN) || (e->getEventType() == EventType::EVENT_BUTTON_UP)) {
			if (hasFocus) {
				submitGuiEvent(e->getEventType() == EventType::EVENT_KEY_DOWN ? WidgetEventType::KEY_DOWN : WidgetEventType::KEY_UP, e);
				e->isHandled = true;
			}
		}
		if (e->getEventType() == EventType::EVENT_KEY_PRESS) {
			if (hasFocus) {
				submitGuiEvent(WidgetEventType::KEY_PRESS, e);
				e->isHandled = true;
			}
		}
		if ((e->getEventType() == EventType::EVENT_BUTTON_DOWN) || (e->getEventType() == EventType::EVENT_BUTTON_UP)
			|| (e->getEventType() == EventType::EVENT_CONTROLLER_AXIS) || (e->getEventType() == EventType::EVENT_CONTROLLER_HAT)) {
			if (hasFocus) {
				submitGuiEvent(
					e->getEventType() == EVENT_BUTTON_DOWN ? WidgetEventType::BUTTON_DOWN : WidgetEventType::BUTTON_UP, e);
				e->isHandled = true;
			}
		}
		if (e->getEventType() == EventType::EVENT_BUTTON_PRESS) {
			if (hasFocus) {
				submitGuiEvent(WidgetEventType::BUTTON_PRESS, e);
				e->isHandled = true;
			}
		}
	}

	void Widget::onRender() {}

	void Widget::submitGuiEvent(WidgetEventType weType, Event* _parent) {
		WidgetEvent* uiEvent = new WidgetEvent();
		uiEvent->setGuiEventType(weType);
		uiEvent->setRaisedBy(*this);
		uiEvent->setParentEventType(_parent->getEventType());
		switch (weType) {
			case WidgetEventType::MOUSE_LEAVE:
			case WidgetEventType::MOUSE_ENTER:
				setDirty();
				break;
			case WidgetEventType::MOUSE_CLICK:
			case WidgetEventType::MOUSE_DOWN:
			case WidgetEventType::MOUSE_UP:
			case WidgetEventType::MOUSE_MOVE:
				auto* mouseEv = dynamic_cast<MouseEvent*>(_parent);
				uiEvent->getData().mouseData = mouseEv->getData();
				if (weType != WidgetEventType::MOUSE_MOVE) {
					setDirty();
				}
				break;
			case WidgetEventType::BUTTON_DOWN:
			case WidgetEventType::BUTTON_UP:
			case WidgetEventType::BUTTON_PRESS:
				auto* ctrlEv = dynamic_cast<ControllerEvent*>(_parent);
				uiEvent->getData().controllerData = ctrlEv->getData();
				setDirty();
				break;
			case WidgetEventType::KEY_DOWN:
			case WidgetEventType::KEY_UP:
			case WidgetEventType::KEY_PRESS:
				auto* keyEv = dynamic_cast<KeyboardEvent*>(_parent);
				uiEvent->getData().keyboardData = keyEv->getData();
				setDirty();
				break;
			default:
				break;
		}

		engine->getInputManager()->submitEvent(uiEvent);
		this->triggerCallbacks(uiEvent);
	}

	void Widget::triggerCallbacks(WidgetEvent* event) {
		for (int j = 0; j < callbacks.get(event->getData().guiEventType).getLength(); j++) {
			callbacks.get(event->getData().guiEventType)[j](event);
		}
	}

	bool Widget::isPointInsideSelectBoundaries(int x, int y) const {
		const Vector2<int>& trueSize = ((selectableSize.x == 0) || (selectableSize.y == 0)) ?
			size : selectableSize; //in case the selectable size isn't set, use default size of ui object

		return x >= resolvedLocation.x && x < resolvedLocation.x + size.x && y >= resolvedLocation.y && y < resolvedLocation.y + size.y;
	}

	void Widget::setDirty() {
		dirty = true;
		if (parent) {
			parent->setDirty();
		}
	}

	void Widget::resolveLocation() {
		auto previousLocation = resolvedLocation;
		auto parentControl = parent;

		if (parentControl == NULL) {
			resolvedLocation.x = location.x;
			resolvedLocation.y = location.y;
		}
		else {
			if (location.x > parentControl->size.x || location.y > parentControl->size.y) {
				resolvedLocation.x = -99999;
				resolvedLocation.y = -99999;
			}
			else {
				resolvedLocation.x = location.x + parentControl->resolvedLocation.x;
				resolvedLocation.y = location.y + parentControl->resolvedLocation.y;
			}
		}
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = children[i];
			child.resolveLocation();
		}
		if (previousLocation != resolvedLocation) {
			setDirty();
		}
	}

	void Widget::render()
	{
		if (!visible || size.x == 0 || size.y == 0) {
			return;
		}

		if (dirty) {
			onRender();
			postRender();
		}

		dirty = false;

		//if enabled, draw normal, else draw tinted.
	}

	void Widget::preRender()
	{
		for (int i = 0; i < children.getLength(); i++) {
			children[i].preRender();
		}
		if (preRenderFunction != NULL) {
			preRenderFunction(this);
		}
	}

	void Widget::postRender() {
		Widget child;
		for (int i = 0; i < children.getLength(); i++) {
			child = children[i];
			if (child.isVisible()) {
				child.render();
			}
		}
	}

	void Widget::setVisible(bool _visibility) {
		if (_visibility != visible) {
			visible = _visibility;
			setDirty();
		}
	}

	void Widget::setParent(Widget* _parent) {
		if (_parent) {
			_parent->children.append(*parent);
			_parent->setDirty();
		}
		parent = _parent;
	}

	Vector2<int> Widget::getParentSize() const
	{
		if (parent != NULL) {
			return parent->size;
		}
		else {
			//return screen size in options
			return Vector2<int>(800, 600);
		}
	}

	int Widget::align(HorizontalAlignment _halign, int parentWidth, int childWidth) {
		int x = 0;
		switch (_halign) {
			case (HorizontalAlignment::LEFT):
				x = 0;
				break;
			case (HorizontalAlignment::CENTER):
				x = (parentWidth / 2) - (childWidth / 2);
				break;
			case (HorizontalAlignment::RIGHT):
				x = parentWidth - childWidth;
				break;
		}
		return x;
	}

	int Widget::align(VerticalAlignment _valign, int parentHeight, int childHeight) {
		int y = 0;
		switch (_valign) {
			case (VerticalAlignment::TOP):
				y = 0;
				break;
			case (VerticalAlignment::CENTER):
				y = (parentHeight / 2) - (childHeight / 2);
				break;
			case (VerticalAlignment::BOTTOM):
				y = parentHeight - childHeight;
				break;
		}
		return y;
	}

	void Widget::align(HorizontalAlignment _halign) {
		location.x = this->align(_halign, parent->size.x, size.x);
	}

	void Widget::align(VerticalAlignment _valign) {
		location.y = this->align(_valign, parent->size.y, size.y);
	}

	void Widget::align(HorizontalAlignment _halign, VerticalAlignment _valign) {
		align(_halign);
		align(_valign);
	}

	void Widget::addCallback(WidgetEventType eType, widgetCallback callback) {
		callbacks.get(eType).append(callback);
	}

	void Widget::run() {
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = children[i];
			child.run();
		}
	}
}