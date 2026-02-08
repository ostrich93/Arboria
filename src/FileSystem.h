#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "framework/String.h"

namespace Arboria {
	int initializeFileSystem();
	void freeFileSystem();

	unsigned char* readBytesFromPhysFS(const char* filename, unsigned int* bytesRead);
	int readFileFromPhysFS(const char* filename, void** buffer);
	//various functions to read or write game specific files like tiles, etc.
	const String getFontPath(const char* filename);
}

#endif