#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "../containers/HashTable.h"
#include "String.h"

namespace Arboria {
	class Texture;
	class ResourceManager {
		private:
			HashTable<String, Texture> imageCache;
			Texture* loadTextureFromFile(String& filename);
			Texture* loadTextureFromFile(const char* filename);
		public:
			HashTable<String, Texture>& getImageCache() { return imageCache; }
			const HashTable<String, Texture>& getImageCache() const { return imageCache; }
			Texture* loadTexture(String& filename);
			Texture* loadTexture(const char* filename);
	};
}

#endif