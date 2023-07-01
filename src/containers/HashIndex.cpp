#include "HashIndex.h"
#include "../utils/math.h"
#include "../framework/String.h"

namespace Arboria {
	int HashIndex::invalidIndexSentinelNode[1] = { -1 };

	void HashIndex::init(const int initialHashSize, const int initialIndexSize) {
		assert(Math::isPowerOfTwo(initialHashSize));

		hashSize = initialHashSize;
		hashBuckets = invalidIndexSentinelNode;
		indexSize = initialIndexSize;
		indexChain = invalidIndexSentinelNode;
		granularity = DEFAULT_HASH_GRANULARITY;
		hashMask = hashSize - 1;
		lookupMask = 0;
	}

	void HashIndex::allocate(const int newHashSize, const int newIndexSize) {
		assert(Math::isPowerOfTwo(newHashSize));
		clearFree();
		hashSize = newHashSize;
		hashBuckets = new int[hashSize];
		memset(hashBuckets, 0xff, hashSize * sizeof(hashBuckets[0]));
		indexSize = newIndexSize;
		indexChain = new int[indexSize];
		memset(indexChain, 0xff, hashSize * sizeof(indexChain[0]));
		hashMask = hashSize - 1;
		lookupMask = -1;
	}

	void HashIndex::clearFree() {
		if (hashBuckets != invalidIndexSentinelNode) {
			delete[] hashBuckets;
			hashBuckets = invalidIndexSentinelNode;
		}
		if (indexChain != invalidIndexSentinelNode) {
			delete[] indexChain;
			indexChain = invalidIndexSentinelNode;
		}
		lookupMask = 0;
	}

	void HashIndex::resizeIndex(const int newIndexSize) {
		if (newIndexSize <= indexSize)
			return;

		int* oldIndexChain, mod, newSize;
		mod = newIndexSize % granularity;
		if (!mod) {
			newSize = newIndexSize;
		}
		else {
			newSize = newIndexSize + granularity - mod;
		}

		if (indexChain == invalidIndexSentinelNode) {
			indexSize = newSize;
			return;
		}

		oldIndexChain = indexChain;
		indexChain = new int[newSize];
		memcpy(indexChain, oldIndexChain, indexSize * sizeof(int));
		memset(indexChain + indexSize, invalidIndexSentinelNode[0], (newSize - indexSize) * sizeof(int));
		delete[] oldIndexChain;
		indexSize = newSize;
	}

	int HashIndex::getSpread() const {
		if (hashBuckets == invalidIndexSentinelNode)
			return 100;

		int i, index, totalItems, *numHashItems, average, error, e;
		totalItems = 0;
		numHashItems = new int[hashSize];

		for (i = 0; i < hashSize; i++) {
			numHashItems[i] = 0;
			for (index = hashBuckets[i]; index >= 0; index = indexChain[index]) {
				numHashItems[i]++;
			}
			totalItems += numHashItems[i];
		}

		if (totalItems <= -1) {
			delete[] numHashItems;
			return 100;
		}
		average = totalItems / hashSize;
		error = 0;
		for (i = 0; i < hashSize; i++) {
			e = abs(numHashItems[i] - average);
			if (e > 1)
				error += e - 1;
		}
		delete[] numHashItems;
		return 100 - (error * 100 / totalItems);
	}

	int HashIndex::generateKey(const char* s, bool caseSensitive) const {
		if (caseSensitive)
			return String::getHash(s) & hashMask;
		else
			return String::getCaseInsensitiveHash(s) & hashMask;
	}
}