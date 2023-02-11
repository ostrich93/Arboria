#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <memory>
#include <assert.h>
#include "../utils/math.h"

namespace Arboria {
	template<typename K>
	inline static int getHash(const K& key, const int maskValue) {
		return key & maskValue;
	}

	//The types used as the keys in a hashtable are strings, numbers (char, long, int, etc.), enums, or objects with a == operator.
	template<typename K, typename V>
	class HashTable {
	public:
		HashTable(const int tableSize = 256);
		HashTable(const HashTable<K, V>& other);
		HashTable<K, V>& operator=(const HashTable<K, V>& other);
		~HashTable();

		size_t size() const;
		size_t memoryAllocated() const;
		size_t memoryUsed() const;

		void clear();
		bool contains(const K& key);
		bool isEmpty() const;
		V get(const K& key) const;
		V getOrDefault(const K& key, const V& defaultValue) const;
		bool remove(const K& key); //remove value at key
		void set(const K& key, const V& value);

		V& operator[](const K& key);
		const V operator[](const K& key) const;

	protected:
		void copy(const HashTable<K, V>& other);
	private:
		struct HashNode {
			K key;
			V value;
			HashNode* next;

			HashNode(const K& key, const V& value) : key(key), value(value), next(nullptr) {};
			HashNode(K& key, V& value, HashNode* next) : key(key), value(value), next(next) {};
		};
		HashNode** buckets;
		int tableSize;
		int numEntries;
		int tableMask;
	};

	template<typename K, typename V>
	inline HashTable<K, V>::HashTable(const int tableSize) {
		assert(isPowerOfTwo(tableSize));
		this->tableSize = tableSize;
		buckets = new HashNode * [tableSize];
		memset(buckets, 0, sizeof(HashNode*) * tableSize);
		numEntries = 0;
		tableMask = tableSize - 1;
	}

	template<typename K, typename V>
	inline HashTable<K, V>::HashTable(const HashTable<K, V>& other) {
		copy(other);
	}

	template<typename K, typename V>
	inline HashTable<K, V>::~HashTable() {
		clear();
		delete buckets[];
	}

	template<typename K, typename V>
	inline HashTable<K, V>& HashTable<K, V>::operator=(const HashTable<K, V>& other) {
		copy(other);
		return *this;
	}

	template<typename K, typename V>
	inline size_t HashTable<K, V>::size() const {
		return sizeof(HashTable<K, V>) + memoryAllocated() + memoryUsed();
	}

	template<typename K, typename V>
	inline size_t HashTable<K, V>::memoryAllocated() const {
		return tableSize * sizeof(buckets);
	}

	template<typename K, typename V>
	inline size_t HashTable<K, V>::memoryUsed() const {
		return sizeof(HashNode*) * numEntries;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::clear() {
		for (int i = 0; i < tableSize; i++) {
			HashNode* next = buckets[i];
			while (next != NULL) {
				HashNode* cursor = next;
				next = next->next;
				delete cursor;
			}
			buckets[i] = NULL;
		}

		numEntries = 0;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::contains(const K& key) {
		int hash = getHash(key, tableMask);
		HashNode* cursor = buckets[hash];
		for (; node != NULL; cursor = cursor->next) {
			if (cursor->key == key) {
				return true;
			}
		}
		return false;
	}

	template<typename K, typename V>
	inline V HashTable<K, V>::get(const K& key) const {
		int hash = getHash(key, tableMask);
		HashNode* cursor = buckets[hash];
		for (; cursor != NULL; cursor = cursor->next) {
			if (cursor->key == key) {
				return cursor->value;
			}
		}

		return NULL;
	}

	template<typename K, typename V>
	inline V HashTable<K, V>::getOrDefault(const K& key, const V& defaultValue) const {
		if (V val = get(key)) {
			return val;
		}

		return *defaultValue;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::isEmpty() const {
		return numEntries == 0;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::remove(const K& key) {
		int hash = getHash(key, tableMask);
		HashNode** head = &buckets[hash];
		if (*head) {
			HashNode* prev = NULL;
			HashNode* node = *head;
			for (; node != NULL; prev = node, node = node->next) {
				if (node->key == key) {
					if (prev) { //if previous node isn't null, have its next node be set as the pointer of the current node's nextNode.
						prev->next = node->next;
					}
					else { //if the node is the first bucket entry, have the head refer to the pointer of the next node. 
						*head = node->next;
					}

					delete node;
					numEntries--;
					return true;
				}
			}
		}
		return false;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::set(const K& key, const V& value) {
		int hash = getHash(key, tableMask);
		HashNode** nextPtr = &(buckets[hash]);
		HashNode* cursor = *nextPtr;
		for (; cursor != NULL; nextPtr = &(cursor->next), cursor = *nextPtr) {
			if (cursor->key == key) {
				cursor->value = value;
				return;
			}
		}
		numEntries++;
		*(nextPtr) = new HashNode(key, value, buckets[hash]);
		*(nextPtr)->next = cursor;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::copy(const HashTable<K, V>& other) {
		if (&other == this) {
			return;
		}

		tableSize = other.tableSize;
		buckets = new HashNode * [tableSize];
		numEntries = other.numEntries;
		tableMask = other.tableMask;

		for (int i = 0; i < tableSize; i++) {
			if (!other.buckets[i]) {
				buckets[i] = NULL;
				continue;
			}
			//lastHead holds the memory of the address of the head by pointing to the address of the pointer of the bucket. Then go through the linked list of the buckets
			HashNode** lastHead = &buckets[i];
			for (HashNode* node = other.buckets[i]; node != NULL; node = node->next) {
				//dereference lastHead and have it refer to a pointer to a new HashNode with data copied from node, with the next node set to null. Then have lastHead point to the address of the new node's next node (NULL) and repeat the process
				*lastHead = new HashNode(node->key, node->value, NULL);
				lastHead = &(*lastHead)->next;
			}
		}
	}

	template<typename K, typename V>
	inline V& HashTable<K, V>::operator[](const K& key) {
		return *(get(key));
	}

	template<typename K, typename V>
	inline const V HashTable<K, V>::operator[](const K& key) const {
		return get(key);
	}
}

#endif