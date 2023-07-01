#ifndef HASH_INDEX_H
#define HASH_INDEX_H

#include <cassert>
#include <memory>

namespace Arboria {

#define DEFAULT_BUCKET_SIZE			1024
#define DEFAULT_HASH_GRANULARITY	1024

	class HashIndex {
		public:
			HashIndex();
			HashIndex(const int initialHashSize, const int initialIndexSize);
			~HashIndex();

			size_t size() const;
			size_t memoryAllocated() const;	

			HashIndex& operator=(const HashIndex& other);

			void add(const int key, const int index);
			void remove(const int key, const int index);

			int first(const int key) const;
			int next(const int index) const;

			void insertIndex(const int key, const int index);
			void removeIndex(const int key, const int index);

			void clear();
			void clearResize(const int newHashSize, const int newIndexSize);
			void clearFree();
			int getHashSize() const;
			int getIndexSize() const;
			void setGranularity(const int newGranularity);
			void resizeIndex(const int newIndexSize);
			int getSpread() const;
			int generateKey(const char* s, bool caseSensitive = false) const;
			int generateKey(const int n1, const int n2) const;
		private:
			int hashSize;
			int* hashBuckets;
			int indexSize;
			int* indexChain;
			int granularity;
			int hashMask;
			int lookupMask;

			static int invalidIndexSentinelNode[1];

			void init(const int initialHashSize, const int initialIndexSize);
			void allocate(const int newHashSize, const int newIndexSize);
	};

	inline HashIndex::HashIndex() {
		init(DEFAULT_BUCKET_SIZE, DEFAULT_BUCKET_SIZE);
	}

	inline HashIndex::HashIndex(const int initialHashSize, const int initialIndexSize) {
		init(initialHashSize, initialIndexSize);
	}

	inline HashIndex::~HashIndex() {
		clearFree();
	}

	inline size_t HashIndex::memoryAllocated() const {
		return hashSize * sizeof(int) + indexSize * sizeof(int);
	}

	inline size_t HashIndex::size() const {
		return sizeof(*this) + memoryAllocated();
	}

	inline HashIndex& HashIndex::operator=(const HashIndex& other) {
		if (&other == this)
			return *this;

		granularity = other.granularity;
		hashMask = other.hashMask;
		lookupMask = other.lookupMask;

		if (other.lookupMask == 0) {
			hashSize = other.hashSize;
			indexSize = other.indexSize;
			clearFree();
		}
		else {
			if (other.hashSize != hashSize || hashBuckets == invalidIndexSentinelNode) {
				if (hashBuckets == invalidIndexSentinelNode)
					delete[] hashBuckets;

				hashSize = other.hashSize;
				hashBuckets = new int[hashSize];
			}
			if (other.indexSize != indexSize || indexChain == invalidIndexSentinelNode) {
				if (indexChain == invalidIndexSentinelNode)
					delete[] indexChain;

				indexSize = other.indexSize;
				indexChain = new int[indexSize];
			}
			memcpy(hashBuckets, other.hashBuckets, hashSize * sizeof(hashBuckets[0]));
			memcpy(indexChain, other.indexChain, indexSize * sizeof(indexChain[0]));
		}
		
		return *this;
	}

	inline void HashIndex::add(const int key, const int index) {
		assert(index >= 0);

		if (hashBuckets == invalidIndexSentinelNode) {
			allocate(hashSize, index >= indexSize ? indexSize + 1 : indexSize);
		}
		else if (index > indexSize) {
			resizeIndex(index + 1);
		}

		int hash = key & hashMask;
		indexChain[index] = hashBuckets[hash];
		hashBuckets[hash] = index;
	}

	inline void HashIndex::remove(const int key, const int index) {
		if (hashBuckets == invalidIndexSentinelNode)
			return;

		int k = key & hashMask;

		if (hashBuckets[k] == index) {
			hashBuckets[k] = indexChain[index];
		}
		else {
			for (int i = hashBuckets[k]; i != invalidIndexSentinelNode[0]; i = indexChain[i]) {
				if (indexChain[i] == index) {
					indexChain[i] == indexChain[index];
					break;
				}
			}
		}
		indexChain[index] = -1;
	}

	inline void HashIndex::insertIndex(const int key, const int index) {
		int i, max;
		if (hashBuckets != invalidIndexSentinelNode) {
			max = index;
			for (i = 0; i < hashSize; i++) {
				if (hashBuckets[i] >= index) {
					hashBuckets[i]++;
					if (hashBuckets[i] > max)
						max = hashBuckets[i];
				}
			}
			for (i = 0; i < indexSize; i++) {
				if (indexChain[i] >= index) {
					indexChain[i]++;
					if (indexChain[i] > max) {
						max = indexChain[i];
					}
				}
			}
			if (max >= indexSize)
				resizeIndex(max + 1);
			for (i = max; i > index; i--)
				indexChain[i] = indexChain[i - 1];
			indexChain[index] = -1;
		}
		add(key, index);
	}

	inline void HashIndex::removeIndex(const int key, const int index) {
		int i, max;
		if (hashBuckets != invalidIndexSentinelNode) {
			max = index;
			for (i = 0; i < hashSize; i++) {
				if (hashBuckets[i] >= index) {
					if (hashBuckets[i] > max) {
						max = hashBuckets[i];
					}
					hashBuckets[i]--;
				}
			}
			for (i = 0; i < indexSize; i++) {
				if (indexChain[i] >= index) {
					if (indexChain[i] > max) {
						max = indexChain[i];
					}
					indexChain[i]--;
				}
			}
			for (i = 0; i < max; i++) {
				indexChain[i] = indexChain[i + 1];
			}
			indexChain[max] = -1;
		}
	}
	
	inline int HashIndex::first(const int key) const {
		return hashBuckets[key & hashMask & lookupMask];
	}

	inline int HashIndex::next(const int index) const {
		assert(index >= 0 && index < indexSize);
		return indexChain[index & lookupMask];
	}

	inline void HashIndex::clear() {
		if (hashBuckets != invalidIndexSentinelNode)
			memset(hashBuckets, invalidIndexSentinelNode[0], sizeof(invalidIndexSentinelNode[0]));
	}

	inline void HashIndex::clearResize(const int newHashSize, const int newIndexSize) {
		clearFree();
		hashSize = newHashSize;
		indexSize = newIndexSize;
	}

	inline int HashIndex::getHashSize() const {
		return hashSize;
	}

	inline int HashIndex::getIndexSize() const {
		return indexSize;
	}

	inline void HashIndex::setGranularity(const int newGranularity) {
		assert(newGranularity > 0);
		granularity = newGranularity;
	}

	inline int HashIndex::generateKey(const int n1, const int n2) const {
		return ((n1 + n2) & hashMask);
	}
}

#endif