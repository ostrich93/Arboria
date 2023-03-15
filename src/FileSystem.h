#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <physfs.h>
#include "definitions.h"
#include "SDL2/SDL.h"

namespace Arboria {
	int initializeFileSystem();
	void freeFileSystem();

	//various functions to read or write game specific files like tiles, etc.
}

#endif