#include "FileSystem.h"
#include "Heap.h"
#include <physfs.h>
#include "definitions.h"
#include "SDL2/SDL.h"

namespace Arboria {

	int initializeFileSystem()
	{
		char* buildDir = (char*)Mem_Alloc(128);
		char* srcDir = (char*)Mem_Alloc(128);
		char* temp_base = SDL_GetBasePath();
		strncpy(buildDir, temp_base, 128);
		strncpy(srcDir, temp_base, 128);
		
		char output[128] = "";
		int mkdirResult;
		char temp[128] = "";
		SDL_assert(PHYSFS_init(0) > 0);
		PHYSFS_permitSymbolicLinks(1);
		PHYSFS_setSaneConfig("Vultan", "Arboria", "7z", 0, 0);
		if (strlen(srcDir) > 0) {
			strcpy(output, srcDir);
			if (output[strlen(output) - 1] != DIR_SEPARATOR_CHAR) {
				strcat(output, DIR_SEPARATOR_STR);
			}
		}
		if (!PHYSFS_mount(output, NULL, 1)) {
			//print error message, throw?
			exit(PHYSFS_getLastErrorCode());
			//return 0;
		}
		PHYSFS_setWriteDir(output);
		/*strcpy(temp, output);
		strcat(temp, "saves" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}*/

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR_STR "background" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR_STR "fonts" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR_STR "gui" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR_STR "shader" DIR_SEPARATOR_STR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			exit(PHYSFS_getLastErrorCode());
		}

		Mem_Free(buildDir);
		Mem_Free(srcDir);
		return 1;
	}

	void freeFileSystem() {
		PHYSFS_deinit();
	}
	char* readBytesFromPhysFS(const char* filename, unsigned int* bytesRead)
	{
		if (!PHYSFS_exists(filename)) {
			*bytesRead = 0;
			return nullptr;
		}
		PHYSFS_File* file = PHYSFS_openRead(filename);
		int size = PHYSFS_fileLength(file);
		char* buffer;
		if (bytesRead != NULL) {
			*bytesRead = size;
			buffer = (char*)Mem_ClearedAlloc(size+1);
			buffer[size] = '\0';
		}
		else {
			buffer = (char*)Mem_ClearedAlloc(size);
		}

		PHYSFS_readBytes(file, buffer, size);
		PHYSFS_close(file);
		return buffer;
	}

	int readFileFromPhysFS(const char* filename, void** buffer)
	{
		PHYSFS_File* file;
		char* buf;
		int length;

		if (buffer)
			*buffer = NULL;

		file = PHYSFS_openRead(filename);
		if (file == NULL) {
			printf("Error on PHYSFS when opening %s: %d", filename, PHYSFS_getLastErrorCode());
			if (buffer) {
				*buffer = NULL;
			}
			return -1;
		}
		length = PHYSFS_fileLength(file);
		if (!buffer) {
			PHYSFS_close(file);
			return length;
		}

		*buffer = buf = (char*)Mem_ClearedAlloc(length + 1);
		PHYSFS_readBytes(file, *buffer, length);
		buf[length] = 0;
		PHYSFS_close(file);
		return length;
	}
}