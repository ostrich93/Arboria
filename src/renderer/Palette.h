#ifndef PALETTE_H
#define PALETTE_H

#include "../utils/Color.h"
#include "../containers/List.h"

namespace Arboria {
	class RenderData;
	class Palette {
	public:
		List<Color> colors;
		RenderData* renderData = nullptr;

		Palette(unsigned int size = 256, Color initialColor = { 0, 0, 0, 0 });
		~Palette();

		const Color& getColor(unsigned int idx) const {
			return colors[idx];
		}

		void setColor(unsigned int idx, Color c) {
			colors[idx] = c;
		}

		Palette(const Palette&);
	};
}

#endif