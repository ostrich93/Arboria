#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

namespace Arboria {
	int initializeFileSystem();
	void freeFileSystem();

	//various functions to read or write game specific files like tiles, etc.
}

#endif