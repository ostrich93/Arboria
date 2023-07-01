#include "Rectangle.h"

namespace Arboria {

	Rectangle::Rectangle(): x(0), y(0), w(0), h(0){}
	Rectangle::Rectangle(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {}
	Rectangle::Rectangle(const SDL_Rect& _r) : x(_r.x), y(_r.y), w(_r.w), h(_r.h) {}
	Rectangle::operator SDL_Rect() const {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
	}
}