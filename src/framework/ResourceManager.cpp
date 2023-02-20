#include "ResourceManager.h"
#include "../renderer/Texture.h"
#include "../renderer/stb_image.h"

namespace Arboria {
	Texture* ResourceManager::loadTextureFromFile(const char* filename) {
		Texture* image = new Texture(filename);
		int width, height, nChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nChannels, 0);
		image->generateTexture(width, height, data);
		stbi_image_free(data);
		return image;
	}

	Texture* ResourceManager::loadTexture(const char* filename) {
		Texture* tex = &imageCache.get(filename);
		if (tex) {
			return tex;
		}

		tex = loadTextureFromFile(filename);
		if (tex) {
			imageCache.set(filename, *tex);
		}
		return tex;
	}
}