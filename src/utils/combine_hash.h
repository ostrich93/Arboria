#ifndef __COMBINE_HASH_H
#define __COMBINE_HASH_H

namespace Arboria {
	inline size_t combineHash(size_t hash1, size_t hash2) {
		return hash1 ^ (hash2 + 0x9e3779b9 + ((hash1 << 6) + (hash1 >> 2)));
	}
}

#endif