#include "ResourceManager.h"
#include "../renderer/Texture.h"
#include "../definitions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../renderer/stb_image.h"
#include <physfs.h>

namespace Arboria {
	Texture* ResourceManager::loadTextureFromFile(String& filename) {
		Texture* image = new Texture(filename);
		int width, height, nChannels;
		String fullfilepath = PHYSFS_getRealDir(filename.c_str()) + filename;
		unsigned char* data = stbi_load(PHYSFS_getRealDir(fullfilepath.c_str()), &width, &height, &nChannels, 0);
		image->generateTexture(width, height, data);
		stbi_image_free(data);
		return image;
	}

	Texture* ResourceManager::loadTextureFromFile(const char* filename) {
		Texture* image = new Texture(filename);
		int width, height, nChannels;
		String fullfilepath = String::join({ PHYSFS_getRealDir(filename), filename }, "");
		unsigned char* data = stbi_load(fullfilepath, &width, &height, &nChannels, 0);
		image->generateTexture(width, height, data);
		stbi_image_free(data);
		return image;
	}

	HashTable<String, Texture*>& ResourceManager::getImageCache()
	{
		return imageCache;
	}

	const HashTable<String, Texture*>& ResourceManager::getImageCache() const
	{
		return imageCache;
	}

	Texture* ResourceManager::loadTexture(String& filename) {
		Texture* tex = imageCache.get(filename);
		if (tex) {
			return tex;
		}

		tex = loadTextureFromFile(filename);
		if (tex) {
			imageCache.set(filename, tex);
		}
		return tex;
	}

	Texture* ResourceManager::loadTexture(const char* filename) {
		Texture* tex = imageCache.get(filename);
		if (tex) {
			return tex;
		}

		tex = loadTextureFromFile(filename);
		if (tex) {
			imageCache.set(filename, tex);
		}
		return tex;
	}
}