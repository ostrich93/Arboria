#ifndef RESOURCEOBJECT_H
#define RESOURCEOBJECT_H

#include "String.h"

namespace Arboria {
	class ResourceObject {
	public:
		uint32_t resId;
		String filePath;
		bool operator==(const ResourceObject& other) const { return (filePath == other.filePath) && (resId == other.resId); }
		bool operator!=(const ResourceObject& other) const { return !(*this == other); }
	};
}

#endif