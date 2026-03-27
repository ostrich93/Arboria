#include "./Widget.h"
#include "../framework/InputActionType.h"
#include "../framework/ActionManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/RenderData.h"

namespace Arboria {
	bool Widget::onEvent(AEvent* e)
	{
		for (auto ctrlIdx = children.begin(); ctrlIdx != children.end();) {
			auto c = *ctrlIdx;
			if ((c->flags & WidgetStateFlags::WIDGET_VISIBLE) && c->enabled) {
				c->onEvent(e);
				if (e->isHandled) {
					return e->isHandled;
				}
			}
			++ctrlIdx;
		}

		if (e->eventType == EventType::EVENT_KEY_DOWN || e->eventType == EventType::EVENT_BUTTON_UP) {
			if (flags & WidgetStateFlags::WIDGET_FOCUSED) {
				submitGuiEvent(e->eventType == EventType::EVENT_KEY_DOWN ? GUIEventType::KEY_DOWN : GUIEventType::KEY_UP, e);
				e->isHandled = true;
			}
		}

		if (e->eventType == EventType::EVENT_KEY_PRESS) {
			if (flags & WidgetStateFlags::WIDGET_FOCUSED) {
				submitGuiEvent(GUIEventType::KEY_PRESS, e);
				e->isHandled = true;
			}
		}
		return e->isHandled;
	}

	void Widget::onRender() {}

	Widget::Widget() : maxScale({ 1,1 }), name(""), parent(NULL), preRenderFunction(NULL),
		flags(WIDGET_ACTIVE | WIDGET_ENABLED | WIDGET_VISIBLE), borderSize(0), borderColor({ 0, 0, 0, 0 }), foregroundColor({ 0,0,0,0 }),
		backgroundColor({ 0,0,0,0 }), data(NULL), enabled(true),
		clickable(false), surface(NULL), palette(NULL), position({ 0,0 }), resolvedLocation({ 0,0 }),
		size({0,0})
	{
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
			resolvedLocation.x = position.x;
			resolvedLocation.y = position.y;
		}
		else {
			if (position.x > parentControl->size.x || position.y > parentControl->size.y) {
				resolvedLocation.x = -99999;
				resolvedLocation.y = -99999;
			}
			else {
				resolvedLocation.x = position.x + parentControl->resolvedLocation.x;
				resolvedLocation.y = position.y + parentControl->resolvedLocation.y;
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
		if (!(flags & WidgetStateFlags::WIDGET_VISIBLE) || size.x == 0 || size.y == 0) {
			return;
		}

		if (surface == nullptr || surface->getSize() != Vector2<unsigned int>(size.x, size.y)) {
			setDirty();
			surface = new Surface(Vector2<unsigned int>(size.x, size.y));
		}

		if (flags & WidgetStateFlags::WIDGET_DIRTY) {
			Palette* prevPalette;
			if (palette) {
				prevPalette = renderer->getPalette();
				renderer->setPalette(palette);
			}
			RendererFrameBufferBinding b(surface);
			onRender();
			postRender();

			if (palette) {
				renderer->setPalette(prevPalette);
			}
		}

		clearFlag(WidgetStateFlags::WIDGET_DIRTY);

		bool renderDefaultSurface = false;
		if (renderer->getCurrentSurface() != NULL) {
			Framebuffer* fbo = dynamic_cast<Framebuffer*>(renderer->getCurrentSurface()->renderData);
			renderDefaultSurface = fbo->fbo_id == 0;
		}
		if (enabled) {
			renderer->draw(surface, Vector2<float>(position.x, position.y), renderDefaultSurface);
		}
		else {
			renderer->drawTinted(surface, Vector2<float>(position.x, position.y), Color{ 255, 255,255,128 }, renderDefaultSurface);
		}
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
		for (auto child : children) {
			if (child->isVisible()) {
				child->render();
			}
		}
	}

	void Widget::unloadResources(){}

	Widget::~Widget()
	{
		unloadResources();
	}

	void Widget::setParent(Widget* _parent) {
		if (_parent) {
			_parent->children.append(this);
			_parent->setDirty();
		}
		parent = _parent;
	}

	void Widget::submitGuiEvent(GUIEventType eventType, AEvent* parentEvent) {
		AEvent* event = nullptr;

		switch (eventType) {
			case GUIEventType::KEY_DOWN:
			case GUIEventType::KEY_PRESS:
			case GUIEventType::KEY_UP:
			{
				event = new AEvent();
				event->eventType = EventType::EVENT_UI_INTERACTION;
				event->guiEvent.raisedBy = this;
				event->guiEvent.eventType = eventType;
				event->guiEvent.keyInfo = parentEvent->keyboardEvent;
				inputManager->submitEvent(event);
				setDirty();
				break;
			}
			case GUIEventType::BUTTON_CLICK:
			case GUIEventType::SCROLLBAR_CHANGE:
			case GUIEventType::LISTBOX_CHANGE_HOVER:
			case GUIEventType::LISTBOX_CHANGE_SELECTED:
			{
				event = new AEvent();
				event->eventType = EventType::EVENT_UI_INTERACTION;
				if (parentEvent->eventType == EventType::EVENT_UI_INTERACTION) {
					event->guiEvent = parentEvent->guiEvent;
				}
				event->guiEvent.raisedBy = this;
				event->guiEvent.eventType = eventType;
				inputManager->submitEvent(event);
				setDirty();
				break;
			}
			default:
				break;
		}
		executeEventCallbacks(event);
	}

	void Widget::addEventCallback(GUIEventType eventType, eventCallback callback)
	{
		List<eventCallback>* eCallbacks;
		eventCallbacks.get(eventType, &eCallbacks);
		if (eCallbacks) { //if there's a list of callbacks for that event type, add
			eCallbacks->append(callback);
		}
		else { //if no list of callbacks found, create a new one, add it to the list, and set the hashtable
			eCallbacks = new List<eventCallback>();
			eCallbacks->append(callback);
			eventCallbacks.set(eventType, *eCallbacks);
		}
	}

	void Widget::executeEventCallbacks(AEvent* e)
	{
		List<eventCallback>* _callbacks;
		eventCallbacks.get(e->guiEvent.eventType, &_callbacks);
		if (_callbacks) {
			for (auto& cb : *_callbacks) {
				cb(e, this);
			}
		}
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
		position.x = this->align(_halign, parent->size.x, size.x);
	}

	void Widget::align(VerticalAlignment _valign) {
		position.y = this->align(_valign, parent->size.y, size.y);
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