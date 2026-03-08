#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <memory>
#include <assert.h>
#include <initializer_list>
#include "../utils/math.h"
#include "../framework/String.h"

namespace Arboria {
	template<typename K, typename V>
	class HashNode {
		public:
			K key;
			V value;
			HashNode<K, V>* next;

			HashNode() : next(NULL) {}
			HashNode(const K& _key, const V& _value) : key(_key), value(_value), next(NULL) {};
			HashNode(const K& _key, const V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {};
			HashNode(K& _key, V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {};

			static int getHash(const K& key, const int maskValue) {
				return key & maskValue;
			}

			static int compare(const K& keyA, const K& keyB) {
				if (keyA < keyB) {
					return -1;
				}
				else if (keyA > keyB) {
					return 1;
				}

				return 0;
			}
	};

	//The types used as the keys in a hashtable are strings, numbers (char, long, int, etc.), enums, or objects with a == operator.
	template<typename K, typename V>
	class HashTable {
		public:
			HashTable(const int tableSize = 256);
			HashTable(const HashTable<K, V>& other);
			HashTable(const std::initializer_list<HashNode<K,V>>& other);
			HashTable<K, V>& operator=(const HashTable<K, V>& other);
			~HashTable();

			size_t size() const;
			size_t memoryAllocated() const;

			void clear();
			void deleteContents();
			bool contains(const K& key);
			bool isEmpty() const;
			bool get(const K& key, V** _value_ = nullptr);
			bool get(const K& key, const V** _value_ = nullptr) const;
			bool remove(const K& key); //remove value at key
			V& set(const K& key, const V& value);

		protected:
			void copy(const HashTable<K, V>& other);
		private:

			HashNode<K, V>** buckets;
			int tableSize;
			int numEntries;
			int tableMask;
	};

	template<typename K, typename V>
	inline HashTable<K, V>::HashTable(const int tableSize) {
		assert(Math::isPowerOfTwo(tableSize));
		this->tableSize = tableSize;
		buckets = new HashNode<K,V>* [tableSize];
		memset(buckets, 0, sizeof(HashNode<K,V>*) * tableSize);
		numEntries = 0;
		tableMask = tableSize - 1;
	}

	template<typename K, typename V>
	inline HashTable<K, V>::HashTable(const HashTable<K, V>& other) {
		copy(other);
	}

	template<typename K, typename V>
	inline HashTable<K, V>::HashTable(const std::initializer_list<HashNode<K,V>>& other) : HashTable()
	{
		if (other.size() > tableSize) {
			//set new tablesize to the nearest power of 2
			this->tableSize = other.size();
			buckets = new HashNode<K, V>* [tableSize];
			memset(buckets, 0, sizeof(HashNode<K, V>*) * tableSize);
			tableMask = tableSize - 1;
		}
		
		for (const auto& n : other) {
			set(n.key, n.value);
		}
	}

	template<typename K, typename V>
	inline HashTable<K, V>::~HashTable() {
		clear();
		delete[] buckets;
	}

	template<typename K, typename V>
	inline HashTable<K, V>& HashTable<K, V>::operator=(const HashTable<K, V>& other) {
		copy(other);
		return *this;
	}

	template<typename K, typename V>
	inline size_t HashTable<K, V>::size() const {
		return sizeof(HashTable<K, V>) + memoryAllocated();
	}

	template<typename K, typename V>
	inline size_t HashTable<K, V>::memoryAllocated() const {
		return tableSize * sizeof(buckets) + sizeof(*buckets) * numEntries;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::clear() {
		HashNode<K, V>* cursor;
		HashNode<K, V>* next;
		for (int i = 0; i < tableSize; i++) {
			next = buckets[i];
			while (next != NULL) {
				cursor = next;
				next = next->next;
				delete cursor;
			}
			buckets[i] = NULL;
		}

		numEntries = 0;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::deleteContents() {
		HashNode<K, V>* cursor;
		HashNode<K, V>* next;
		for (int i = 0; i < tableSize; i++) {
			next = buckets[i];
			while (next != NULL) {
				cursor = next;
				next = next->next;
				delete cursor->value;
				delete cursor;
			}
			buckets[i] = NULL;
		}

		numEntries = 0;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::contains(const K& key) {
		int hash = HashNode<K, V>::getHash(key, tableMask);
		HashNode<K, V>* cursor = buckets[hash];
		for (; cursor != NULL; cursor = cursor->next) {
			if (cursor->key == key) {
				return true;
			}
		}
		return false;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::get(const K& key, V** _value_) {
		int hash = HashNode<K, V>::getHash(key, tableMask);
		HashNode<K, V>* cursor = buckets[hash];
		for (; cursor != NULL; cursor = cursor->next) {
			int s = cursor->compare(cursor->key, key);
			if (s == 0) {
				if (_value_) {
					*_value_ = &cursor->value;
				}
				return true;
			}
			if (s > 0) {
				break;
			}
		}
		if (_value_) {
			*_value_ = NULL;
		}

		return false;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::get(const K& key, const V** _value_) const {
		int hash = HashNode<K, V>::getHash(key, tableMask);
		HashNode<K, V>* cursor = buckets[hash];
		for (; cursor != NULL; cursor = cursor->next) {
			int s = cursor->compare(cursor->key, key);
			if (s == 0) {
				if (_value_) {
					*_value_ = &cursor->value;
				}
				return true;
			}
			if (s > 0) {
				break;
			}
		}
		if (_value_) {
			*_value_ = NULL;
		}

		return false;

	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::isEmpty() const {
		return numEntries == 0;
	}

	template<typename K, typename V>
	inline bool HashTable<K, V>::remove(const K& key) {
		int hash = HashNode<K, V>::getHash(key, tableMask);
		HashNode<K, V>** head = &buckets[hash];
		if (*head) {
			HashNode<K, V>* prev = NULL;
			HashNode<K, V>* node = *head;
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
	inline V& HashTable<K, V>::set(const K& key, const V& value) {
		int hash = HashNode<K,V>::getHash(key, tableMask);
		HashNode<K,V>** nextPtr = &(buckets[hash]);
		HashNode<K,V>* cursor = *nextPtr;
		for (; cursor != NULL; nextPtr = &(cursor->next), cursor = *nextPtr) {
			if (cursor->key == key) {
				cursor->value = value;
				return cursor->value;
			}
		}
		numEntries++;
		*(nextPtr) = new HashNode<K,V>(key, value, buckets[hash]);
		(*nextPtr)->next = cursor;
		return (*nextPtr)->value;
	}

	template<typename K, typename V>
	inline void HashTable<K, V>::copy(const HashTable<K, V>& other) {
		if (&other == this) {
			return;
		}

		tableSize = other.tableSize;
		buckets = new HashNode<K, V>* [tableSize];
		numEntries = other.numEntries;
		tableMask = other.tableMask;

		for (int i = 0; i < tableSize; i++) {
			if (!other.buckets[i]) {
				buckets[i] = NULL;
				continue;
			}
			//lastHead holds the memory of the address of the head by pointing to the address of the pointer of the bucket. Then go through the linked list of the buckets
			HashNode<K, V>** lastHead = &buckets[i];
			for (HashNode<K, V>* node = other.buckets[i]; node != NULL; node = node->next) {
				//dereference lastHead and have it refer to a pointer to a new HashNode with data copied from node, with the next node set to null. Then have lastHead point to the address of the new node's next node (NULL) and repeat the process
				*lastHead = new HashNode<K,V>(node->key, node->value, NULL);
				lastHead = &(*lastHead)->next;
			}
		}
	}

	template<typename V>
	class HashNode<String, V> {
		public:
			String key;
			V value;
			HashNode<String, V>* next;

			HashNode(const String& _key, const V& _value) : key(_key), value(_value), next(NULL) {};
			HashNode(const String& _key, const V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {};
			HashNode(const String& _key, V _value, HashNode* _next) : key(_key), value(_value), next(_next) {};
			HashNode(const char* _key, V _value, HashNode* _next) : key(_key), value(_value), next(_next) {};
			HashNode(String& _key, V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {};

			static int getHash(const String& key, const int maskValue) {
				return String::getHash(key) & maskValue;
			}

			static int compare(const String& keyA, const String& keyB) {
				return String::iCompare(keyA, keyB);
			}
		};

	template <typename V>
	class HashNode<const char*, V> {
		public:
			String key;
			V value;
			HashNode<const char*, V>* next;

			HashNode(const char* const& _key, const V& _value) : key(_key), value(_value), next(NULL) {};
			HashNode(const char* const& _key, const V& _value, HashNode* _next) : key(_key), value(_value), next(_next) {};
		
			static int getHash(const char* const& key, const int maskValue) {
				return String::getHash(key) & maskValue;
			}

			static int compare(const char* const& keyA, const char* const& keyB) {
				return String::iCompare(keyA, keyB);
			}
	};

	template<typename V>
	class HashTable<String, V> {
	public:
		HashTable(int tableSize = 256);
		HashTable(const HashTable<String, V>& other);
		HashTable<String, V>& operator=(const HashTable<String, V>& other);
		~HashTable();

		size_t size() const;
		size_t memoryAllocated() const;

		void clear();
		void deleteContents();
		bool contains(const char* key);
		bool isEmpty() const;
		bool get(const char* key, V** _value_ = nullptr) const;
		bool remove(const char* key); //remove value at key
		void set(const char* key, V& value);

	protected:
		void copy(const HashTable<String, V>& other);
	private:
		struct hashNode_string {
			String key;
			V value;
			hashNode_string* next;

			hashNode_string(const String& k, V v, hashNode_string* n) : key(k), value(v), next(n) {};
			hashNode_string(const char* k, V v, hashNode_string* n) : key(k), value(v), next(n) {};
		};
		hashNode_string** buckets;
		int tableSize;
		int numEntries;
		int tableMask;

		int getHash(const char* key) const {
			return (String::getHash(key) & tableMask);
		}
	};

	template<typename V>
	inline HashTable<String, V>::HashTable(int tableSize) {
		assert(Math::isPowerOfTwo(tableSize));
		this->tableSize = tableSize;
		buckets = new hashNode_string*[tableSize];
		memset(buckets, 0, sizeof(buckets) * tableSize);
		numEntries = 0;
		tableMask = tableSize - 1;
	}

	template<typename V>
	inline HashTable<String, V>::HashTable(const HashTable<String, V>& other) {
		copy(other);
	}

	template<typename V>
	inline HashTable<String, V>& HashTable<String, V>::operator=(const HashTable<String, V>& other) {
		copy(other);
		return *this;
	}

	template<typename V>
	inline HashTable<String, V>::~HashTable() {
		clear();
		delete[] buckets;
	}

	template<typename V>
	inline size_t HashTable<String, V>::size() const {
		return sizeof(HashTable<String, V>) + memoryAllocated();
	}

	template<typename V>
	inline size_t HashTable<String, V>::memoryAllocated() const {
		return (buckets) * tableSize + sizeof(buckets) * numEntries;
	}

	template<typename V>
	inline void HashTable<String, V>::clear() {
		hashNode_string* cursor;
		hashNode_string* next;
		for (int i = 0; i < tableSize; i++) {
			next = buckets[i];
			while (next != NULL) {
				cursor = next;
				next = next->next;
				delete cursor;
			}
			buckets[i] = NULL;
		}

		numEntries = 0;
	}

	template<typename V>
	inline void HashTable<String, V>::deleteContents() {
		hashNode_string* cursor;
		hashNode_string* next;
		for (int i = 0; i < tableSize; i++) {
			next = buckets[i];
			while (next != NULL) {
				cursor = next;
				next = next->next;
				delete cursor->value;
				delete cursor;
			}
			buckets[i] = NULL;
		}

		numEntries = 0;
	}

	template<typename V>
	inline bool HashTable<String, V>::contains(const char* key) {
		int hash = getHash(key);
		int cmpValue;
		hashNode_string* cursor;
		for (cursor = buckets[hash]; cursor != NULL; cursor = cursor->next) {
			cmpValue = cursor->key.compare(key);
			if (cmpValue == 0) {
				return true;
			}
		}
		return false;
	}

	template<typename V>
	inline bool HashTable<String, V>::get(const char* key, V** _value_) const {
		int hash = getHash(key);
		int cmpValue;
		hashNode_string* cursor;
		for (cursor = buckets[hash]; cursor != NULL; cursor = cursor->next) {
			cmpValue = cursor->key.compare(key);
			if (cmpValue == 0) {
				if (_value_)
					*_value_ = &cursor->value;
				return true;
			}
			if (cmpValue > 0)
				break;
		}

		if (_value_) {
			*_value_ = NULL;
		}

		return false;
	}

	template< typename V>
	inline bool HashTable<String, V>::isEmpty() const {
		return numEntries == 0;
	}

	template<typename V>
	inline bool HashTable<String, V>::remove(const char* key) {
		int hash = getHash(key);
		hashNode_string** head = &buckets[hash];
		if (*head) {
			hashNode_string* prev;
			hashNode_string* node;
			for (prev = NULL, node = *head; node != NULL; prev = node, node = node->next) {
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

	template<typename V>
	inline void HashTable<String, V>::set(const char* key, V& value) {
		int hash = getHash(key);
		hashNode_string** nextPtr;
		hashNode_string* cursor;
		int cmpValue;
		for (nextPtr = &(buckets[hash]), cursor = *nextPtr; cursor != NULL; nextPtr = &(cursor->next), cursor = *nextPtr) {
			cmpValue = cursor->key.compare(key);
			if (cmpValue == 0) {
				cursor->value = value;
				return;
			}
			if (cmpValue > 0) {
				break;
			}
		}
		numEntries++;
		*(nextPtr) = new hashNode_string(key, value, buckets[hash]);
		(*nextPtr)->next = cursor;
	}

	template<typename V>
	inline void HashTable<String, V>::copy(const HashTable<String, V>& other) {
		if (&other == this) {
			return;
		}

		int i;
		hashNode_string* node;
		hashNode_string** prev;
		assert(other.tableSize > 0);

		tableSize = other.tableSize;
		buckets = new hashNode_string *[tableSize];
		numEntries = other.numEntries;
		tableMask = other.tableMask;

		for (i = 0; i < tableSize; i++) {
			if (!other.buckets[i]) {
				buckets[i] = NULL;
				continue;
			}
			prev = &buckets[i];
			for (node = other.buckets[i]; node != NULL; node = node->next) {
				*prev = new hashNode_string(node->key, node->value, NULL);
				prev = &(*prev)->next;
			}
		}
	}
}

#endif