#ifndef FILESYSTEM_H
#define FILESYSTEM_H

namespace Arboria {
	int initializeFileSystem();
	void freeFileSystem();

	char* readBytesFromPhysFS(const char* filename, unsigned int* bytesRead);
	int readFileFromPhysFS(const char* filename, void** buffer);
	//various functions to read or write game specific files like tiles, etc.
}

#endif