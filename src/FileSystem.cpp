#include "FileSystem.h"
#include "Heap.h"

namespace Arboria {

	int initializeFileSystem()
	{
		char* buildDir = (char*)Mem_Alloc(128);
		char* srcDir = (char*)Mem_Alloc(128);
		char* temp_base = SDL_GetBasePath();
		strncpy(buildDir, temp_base, 128);
		strncpy(srcDir, temp_base, 128);
		Mem_Free(temp_base);
		
		char output[128] = "";
		int mkdirResult;
		char temp[128] = "";
		SDL_assert(PHYSFS_init(0) > 0);
		PHYSFS_permitSymbolicLinks(1);
		PHYSFS_setSaneConfig("Vultan", "Arboria", "pak", 0, 0);
		if (strlen(srcDir) > 0) {
			strcpy(output, srcDir);
			if (output[strlen(output) - 1] != DIR_SEPARATOR[0]) {
				strcat(output, DIR_SEPARATOR);
			}
		}
		if (!PHYSFS_mount(output, NULL, 1)) {
			//print error message, throw?
			return 0;
		}

		strcpy(temp, output);
		strcat(temp, "saves" DIR_SEPARATOR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "src" DIR_SEPARATOR "renderer" DIR_SEPARATOR "shader" DIR_SEPARATOR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets");
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR "background" DIR_SEPARATOR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR "fonts" DIR_SEPARATOR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		temp[0] = '\0';
		strcpy(temp, output);
		strcat(temp, "assets" DIR_SEPARATOR "gui" DIR_SEPARATOR);
		if (!PHYSFS_mount(temp, NULL, 1)) {
			return 0;
		}

		Mem_Free(buildDir);
		Mem_Free(srcDir);
		return 1;
	}

	void freeFileSystem() {
		PHYSFS_deinit();
	}
}