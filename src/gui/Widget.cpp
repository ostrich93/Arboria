#include "Window.h"
#include "../framework/InputActionType.h"
#include "../framework/InputManager.h"
#include "../framework/ResourceManager.h"
#include "../framework/Lexer.h"
#include "../renderer/Renderer.h"
#include "../renderer/RenderData.h"
#include "ListBoxWidget.h"
#include "TextButton.h"
#include "Graphic.h"
#include "GraphicButton.h"
#include "Spinner.h"
#include "Label.h"

namespace Arboria {
	bool Widget::onEvent(AEvent* e)
	{
		static bool actionDown;

		if (flags & WIDGET_ROOT) {
			actionDown = false;
		}

		if (visible) {
			if (e->eventType >= EventType::EVENT_KEY && e->eventType <= EventType::EVENT_CONTROLLER_BUTTON) {
				int c = children.getLength();
				while (--c >= 0) {
					if (children[c]->visible && children[c]->containsPos(gui->getCursorX(), gui->getCursorY())) {
						Widget* child = children[c];
						if (e->eventValue2) {
							setFocus(child);
							e->isHandled = child->onEvent(e);
						}
					}
				}
				if (e->eventValue2 && !actionDown) {
					int action = inputManager->getBinding(e->eventValue);
					if (action > -1 && callbacks[action]) {
						actionDown = callbacks[action](this, e);
						e->isHandled = actionDown;
					}
				}
			}
			if (e->eventType == EventType::EVENT_MOUSE_MOVE || e->eventType == EventType::EVENT_MOUSE_MOVE_ABS) {
				e->isHandled = routeCursorCoordinates();
				return e->isHandled;
			}
		}

		return e->isHandled;
	}

	void Widget::onRender() {}

	Widget::Widget() : maxScale({ 1,1 }), name(""), parent(NULL), preRenderFunction(NULL),
		flags(0), borderSize(0), borderColor({ 0, 0, 0, 0 }), foregroundColor({ 0,0,0,0 }),
		backgroundColor({ 0,0,0,0 }), data(NULL), visible(true),
		clickable(false), surface(NULL), palette(NULL), position({ 0,0 }),
		size({ 0,0 }), gui(NULL) 
	{
	}

	Widget::Widget(Window* gui) : maxScale({ 1,1 }), name(""), parent(NULL), preRenderFunction(NULL),
		flags(0), borderSize(0), borderColor({ 0, 0, 0, 0 }), foregroundColor({ 0,0,0,0 }),
		backgroundColor({ 0,0,0,0 }), data(NULL), visible(true),
		clickable(false), surface(NULL), palette(NULL), position({ 0,0 }),
		size({0,0}), gui(gui)
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

	Widget* Widget::getChild(int index)
	{
		if (index < 0 || index > children.getLength()) {
			Engine::printError("Widget::getChild: index out of bounds");
			return nullptr;
		}
		return children[index];
	}

	Widget* Widget::getFocusedChild()
	{
		if (flags & WIDGET_ROOT)
			return gui->getRoot()->focusedChild;
		return nullptr;
	}

	Widget* Widget::setFocus(Widget* widget) {
		Widget* lastFocused = NULL;
		if (flags & WIDGET_FOCUSABLE) {
			lastFocused = gui->getRoot()->focusedChild;
			if (lastFocused) {
				lastFocused->clearFlag(WIDGET_FOCUSED);

			}
			widget->setFlag(WIDGET_FOCUSED);
			gui->getRoot()->focusedChild = widget;
		}
		return lastFocused;
	}

	void Widget::render()
	{
		if (!visible || size.x == 0 || size.y == 0) {
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
			renderer->draw(surface, Vector2<float>(position.x, position.y), renderDefaultSurface);
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

	bool Widget::parse(Lexer* src, bool rebuild)
	{
		Token token, token2, token3;

		src->expectTokenType(TOKENTYPE_NAME, 0, &token);
		name = token.c_str();

		src->expectTokenString("{");
		src->expectAnyToken(&token);

		bool ret = true;

		while (token != "}") {
			if (token == "widget") {
				src->expectTokenType(TOKENTYPE_NAME, 0, &token);
				token2 = token;
				src->unreadToken(&token);

				Widget* wid = new Widget(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "listbox") {
				ListBoxWidget* wid = new ListBoxWidget(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "graphic") {
				Graphic* wid = new Graphic(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "graphicButton") {
				GraphicButton* wid = new GraphicButton(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "label") {
				Label* wid = new Label(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "textButton") {
				TextButton* wid = new TextButton(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "spinner") {
				Spinner* wid = new Spinner(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (token == "scrollbar") {
				ScrollbarWidget* wid = new ScrollbarWidget(gui);
				wid->parse(src, rebuild);
				wid->setParent(this);
			}
			else if (parseInternalValue(token, src)) {
			}

			//parsing alignment, separated by commas
			else if (String::iCompare(token, "alignment") == 0) {
				HorizontalAlignment hal = HorizontalAlignment::HOR_LEFT;
				VerticalAlignment val = VerticalAlignment::VERT_TOP;
				parseAlignment(src, val, hal);
				align(hal, val);
			}
			if (!src->readToken(&token)) {
				Engine::printError("Widget::parse: Unexpected end of file");
				ret = false;
				break;
			}
		}

		return ret;
	}

	bool Widget::parseInternalValue(const char* _name, Lexer* src)
	{
		if (String::iCompare(_name, "size") == 0) {
			parseVect2i(src, size);
			return true;
		}
		if (String::iCompare(_name, "position") == 0) {
			parseVect2i(src, position);
			return true;
		}
		if (String::iCompare(_name, "backgroundColor") == 0) {
			parseColor(src, selectColor);
			return true;
		}
		if (String::iCompare(_name, "foregroundColor") == 0) {
			parseColor(src, foregroundColor);
			return true;
		}
		if (String::iCompare(_name, "selectColor") == 0) {
			parseColor(src, selectColor);
			return true;
		}
		if (String::iCompare(_name, "borderColor") == 0) {
			parseColor(src, borderColor);
			return true;
		}
		if (String::iCompare(_name, "borderSize") == 0) {
			borderSize = src->parseInt();
			return true;
		}
		if (String::iCompare(_name, "name") == 0) {
			parseString(src, name);
			return true;
		}
		if (String::iCompare(_name, "maxScale") == 0) {
			parseVect2f(src, maxScale);
			return true;
		}
		if (String::iCompare(_name, "clickable") == 0) {
			clickable = src->parseBool();
			return true;
		}
		if (String::iCompare(_name, "visible") == 0) {
			visible = src->parseBool();
			return true;
		}
		if (String::iCompare(_name, "palette") == 0) {
			parsePalette(src, palette);
			return true;
		}
		return false;
	}

	void Widget::parseVect2i(Lexer* src, Vector2<int>& out)
	{
		Token tok;
		src->readToken(&tok);
		out.x = atoi(tok);
		src->expectTokenString(",");
		src->readToken(&tok);
		out.y = atoi(tok);
	}

	void Widget::parseVect2f(Lexer* src, Vector2<float>& out) {
		Token tok;
		src->readToken(&tok);
		out.x = atof(tok);
		src->expectTokenString(",");
		src->readToken(&tok);
		out.y = atof(tok);
	}

	void Widget::parseColor(Lexer* src, Color& out) {
		Token tok;
		src->readToken(&tok);
		out.r = (uint8_t)atoi(tok);
		src->expectTokenString(",");
		src->readToken(&tok);
		out.g = (uint8_t)atoi(tok);
		src->expectTokenString(",");
		src->readToken(&tok);
		out.b = (uint8_t)atoi(tok);
		src->expectTokenString(",");
		src->readToken(&tok);
		out.a = (uint8_t)atoi(tok);
	}

	void Widget::parseString(Lexer* src, String& out) {
		Token tok;
		if (src->readToken(&tok))
			out = tok;
	}

	void Widget::parsePalette(Lexer* src, Palette* out)
	{
		Token tok;
		if (!src->expectTokenType(TOKENTYPE_NUMBER, 0, &tok)) {
			Engine::printError("Could not read the expected palette id value");
			return;
		}
		out = resourceManager->loadPalette(tok.getUnsignedIntegerValue());
	}

	void Widget::parseAlignment(Lexer* src, VerticalAlignment& out1, HorizontalAlignment& out2)
	{
		Token tok;
		src->readToken(&tok);
		out1 = (VerticalAlignment)atoi(tok);
		if (!src->expectTokenString(",")) { //if horizontal alignment is NOT specified, go with default, otherwise, read another token and get the result.
			out2 = HorizontalAlignment::HOR_LEFT;
			return;
		}
		src->readToken(&tok);
		out2 = (HorizontalAlignment)atoi(tok);
	}

	void Widget::parseImage(Lexer* src, Image* img) {}

	void Widget::setParent(Widget* _parent) {
		if (_parent) {
			_parent->children.append(this);
			_parent->setDirty();
		}
		parent = _parent;
	}

	void Widget::setParent(Widget* _parent, int pos)
	{
		if (_parent) {
			_parent->children.insert(this, pos);
			_parent->setDirty();
		}
		parent = _parent;
	}

	bool Widget::containsPos(int x, int y)
	{
		if (x < 0 || y < 0) return false;
		Vector2<int> actualPos = getActualPosition();
		return (x >= actualPos.x && x <= actualPos.x + size.x) &&
			(y >= actualPos.y && y <= actualPos.y + size.y);
	}

	Vector2<int> Widget::getActualPosition()
	{
		if (parent)
			return parent->getActualPosition() + position;
		return position;
	}

	bool Widget::routeCursorCoordinates()
	{

		int c = children.getLength();
		while (c > 0) {
			Widget* child = children[c--];
			if (child->isVisible() && child->containsPos(gui->getCursorX(), gui->getCursorY())) {
				child->hovered = true;
				child->routeCursorCoordinates();
			}
		}
		return false;
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
		if (!parent) {
			position.x = this->align(_halign, 0, size.x);
			return;
		}
		position.x = this->align(_halign, parent->size.x, size.x);
	}

	void Widget::align(VerticalAlignment _valign) {
		if (!parent) {
			position.y = this->align(_valign, 0, size.y);
			return;
		}
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
			callbacks[actionType] = callback;
		}
	}

	void Widget::executeCallback(int actionType, AEvent* ev)
	{
		if (actionType >= ACTION_CONFIRM && actionType <= ACTION_SELECT) {
			callbacks[actionType](this, ev);
		}
	}

	void Widget::run() {
		for (int i = 0; i < children.getLength(); i++) {
			Widget child = *children[i];
			child.run();
		}
	}
}