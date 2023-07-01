#include "ResourceManager.h"
#include "../renderer/Texture.h"
#include "../definitions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../renderer/stb_image.h"
#include <physfs.h>

namespace Arboria {

	Texture* loadTextureFromFile(String& filename, GLuint _format) {
		Texture* image = new Texture(filename);
		int width, height, nChannels;
		String fullfilepath = PHYSFS_getRealDir(filename.c_str()) + filename;
		unsigned char* data = stbi_load(PHYSFS_getRealDir(fullfilepath.c_str()), &width, &height, &nChannels, 0);
		image->intializePixelData(width, height, _format);
		if (data != NULL && image->getHeight() != 0 && image->getWidth() != 0) {
			memcpy(image->getPixelData(), data, width * height);
			stbi_image_free(data);
		}
		return image;
	}

	Texture* loadTextureFromFile(const char* filename, GLuint _format) {
		Texture* image = new Texture(filename);
		int width, height, nChannels;
		String fullfilepath = String::join({ PHYSFS_getRealDir(filename), filename }, "");
		unsigned char* data = stbi_load(fullfilepath.c_str(), &width, &height, &nChannels, 0);
		image->intializePixelData(width, height, _format);
		if (data != NULL && image->getHeight() != 0 && image->getWidth() != 0) {
			memcpy(image->getPixelData(), data, width * height);
			stbi_image_free(data);
		}
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