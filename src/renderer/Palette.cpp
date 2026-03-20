#include "Palette.h"

namespace Arboria {
	Palette::Palette(unsigned int size, Color initialColor) : colors(size)
	{
		for (unsigned int i = 0; i < size; i++) {
			colors.append(initialColor);
		}
	}
	Palette::~Palette() = default;

	Palette::Palette(const Palette& source)
	{
		colors = source.colors;
	}
}