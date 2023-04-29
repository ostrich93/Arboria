#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../containers/HashTable.h"
#include "Engine.h"
#include "String.h"

namespace Arboria {
	class Texture;
	class ResourceManager {
		private:
			HashTable<String, Texture*> imageCache;
			Texture* loadTextureFromFile(String& filename);
			Texture* loadTextureFromFile(const char* filename);
		public:
			HashTable<String, Texture*>& getImageCache();
			const HashTable<String, Texture*>& getImageCache() const;
			Texture* loadTexture(String& filename);
			Texture* loadTexture(const char* filename);
	};
}

#endif