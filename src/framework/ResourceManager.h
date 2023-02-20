#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "../containers/HashTable.h"

namespace Arboria {
	class Texture;
	class ResourceManager {
		private:
			HashTable<const char*, Texture> imageCache;
			Texture* loadTextureFromFile(const char* filename);
		public:
			HashTable<const char*, Texture>& getImageCache() { return imageCache; }
			const HashTable<const char*, Texture>& getImageCache() const { return imageCache; }
			Texture* loadTexture(const char* filename);

	};
}

#endif