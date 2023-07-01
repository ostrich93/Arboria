#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>

namespace Arboria {

	class Rectangle {
		public:
			float x, y, w, h;
			Rectangle();
			Rectangle(float _x, float _y, float _w, float _h);
			explicit Rectangle(const SDL_Rect& _r);
			operator SDL_Rect() const;
	};
}

#endif