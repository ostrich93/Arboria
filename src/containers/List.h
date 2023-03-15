#ifndef __LIST_H__
#define __LIST_H__

#include "../Heap.h"
#include <new>
#include <stdlib.h>
#include <assert.h>
#include <initializer_list>

namespace Arboria {
	template <typename T>
	class List {
	public:
		List(int newGranularity = 16);
		List(const List& other);
		List(const std::initializer_list<T>& other);
		~List();

		void append(const T& elem);
		void append(const List& other);
		void insert(const T& elem, int index = 0);
		bool remove(const T& elem);
		bool removeAt(int index);
		bool removeLast();
		bool contains(const T& elem);
		int findIndex(const T& elem);
		T* find(T const& elem);
		int findNull();
		void clear();
		int getLength() const;
		int getCapacity();
		int getGranularity();
		void setGranularity(int newGranularity);
		size_t size() const;
		size_t memoryAllocated() const;
		size_t memoryUsed() const;
		void resize(int newSize);
		void resize(int newSize, int newGranularity);
		void setNum(int newNum);
		T* getPtr();
		T& allocateNewElement();

		//operators
		List<T>& operator=(const List<T>& other);
		T& operator[](int index);
		const T& operator[](int index) const;
	private:
		int num;
		int capacity;
		int granularity;
		T* list;
	};

	template<typename T>
	inline List<T>::List<T>(int newGranularity) {
		num = 0;
		capacity = newGranularity;
		granularity = newGranularity;
		list = NULL;
	}

	template<typename T>
	inline List<T>::List<T>(const List<T>& other) { //copy constructor
		list = NULL;
		*this = other;
	}

	template<typename T>
	inline List<T>::List<T>(const std::initializer_list<T>& other) {
		resize(other.size);
		auto x = other.begin();
		while (x != other.end()) {
			append(*x++);
		}
	}

	template<typename T>
	inline List<T>::~List<T>() {
		clear();
	}

	template <typename T>
	inline void* generateNewList(int num, bool zeroBuffer) { //this is used by the second constructor
		T* ptr = NULL;
		if (zeroBuffer) {
			ptr = (T*)Mem_ClearedAlloc(sizeof(T) * num);
		}
		else {
			ptr = (T*)Mem_Alloc(sizeof(T) * num);
		}
		for (int i = 0; i < size; ++i) {
			//the new call does NOT allocate new memory, what it does is construct a T object at &ptr[i], which is equivalent to new(sizeof(T), &ptr[i])
			new (&ptr[i]) T;
		}
		return ptr;
	}

	template<typename T>
	inline void deleteListContent(void* ptr, int _num) {
		for (int i = 0; i < _num; ++i) {
			((T*)list)[i].~T();
		}
		Mem_Free(list);
	}

	template <typename T>
	inline void* resizeList(void* ptr, int oldSize, int newSize, bool zeroBuffer) {
		T* oldPtr = (T*)ptr;
		T* newPtr = NULL;
		if (newSize > 0) {
			newPtr = (T*)generateNewList(newSize, zeroBuffer);
			//get the min between the oldSize and newSize, then copy the first min bytes from the oldptr to the newptr
			int carryOver = min(oldSize, newSize);
			for (int i = 0; i < carryOver; ++i) {
				newPtr[i] = oldPtr[i];
			}
		}
		deleteListContent<T>(oldPtr, oldSize);
		return newPtr;
	}

	template<typename T>
	inline void List<T>::append(const T& elem) {
		if (!list) {
			resize(granularity);
		}
		if (num == size) {
			int newSize = capacity + granularity;
			newSize -= newSize % granularity;
			resize(newSize);
		}
		list[num] = elem;
		num++;
	}

	template<typename T>
	inline void List<T>::append(const List<T>& other) {
		if (!list) {
			resize(granularity);
		}
		if (num == size) {
			int newSize = capacity + granularity;
			newSize -= newSize % granularity;
			resize(newSize);
		}
		int otherSize = other.size();
		for (int i = 0; i < otherSize; i++) {
			append(other[i]);
		}
	}

	template<typename T>
	inline void List<T>::insert(const T& elem, int index) {
		if (!list) {
			resize(granularity);
		}
		if (num == capacity) {
			int newSize = capacity + granularity;
			newSize -= newSize % granularity;
			resize(newSize);
		}
		if (index < 0) {
			index = 0;
		}
		else if (index > num) {
			index = num;
		}
		for (int i = num; i > index; --i) {
			list[i] = list[i - 1];
		}
		list[index] = elem;
		num++;
	}

	template<typename T>
	inline bool List<T>::remove(const T& elem) {
		int index = findIndex(elem);
		if (index >= 0) {
			return removeAt(index);
		}

		return false;
	}

	template<typename T>
	inline bool List<T>::removeAt(int index) {
		if (i < 0 || i >= num) {
			return false;
		}

		num--;
		for (int i = index; i < num; i++) {
			list[i] = list[i + 1];
		}

		return true;
	}

	template<typename T>
	inline bool List<T>::removeLast() {
		if (num == 0) {
			return false;
		}

		num--;
		list[num] = list[num + 1];

		return true;
	}

	template<typename T>
	inline bool List<T>::contains(const T& elem) {
		return findIndex(elem) > -1;
	}

	template<typename T>
	inline int List<T>::findIndex(const T& elem) {
		//Since the list is unsorted, it's best to do a linear search
		for (int i = 0; i < num; i++) {
			if (list[i] == elem) {
				return i;
			}
		}

		return -1;
	}

	template<typename T>
	inline T* List<T>::find(const T& elem) {
		//return a pointer to the element
		int index = findIndex(elem);
		if (index >= 0) {
			return &list[i];
		}
		return NULL;
	}

	template<typename T>
	inline int List<T>::findNull() {
		//since we can't get a pointer to null for obvious reasons, findIndex isn't used here
		for (int i = 0; i < num; i++) {
			if (list[i] == NULL)
				return i;
		}

		return -1;
	}

	template<typename T>
	inline void List<T>::clear()
	{
		deleteListContent(list, capacity);
		list = NULL;
		num = 0;
		capacity = 0;
	}

	template<typename T>
	inline int List<T>::getLength() const {
		return num;
	}

	template<typename T>
	inline int List<T>::getCapacity() {
		return capacity;
	}

	template<typename T>
	inline int List<T>::getGranularity() {
		return granularity;
	}

	template<typename T>
	inline void List<T>::setGranularity(int newGranularity) {
		int newSize;

		assert(newGranularity > 0);
		granularity = newGranularity;

		if (list) {
			newSize = num + granularity - 1;
			newSize -= newSize % granularity; //sets the size to the exact granularity value
			if (capacity != newSize) { //if the capacity is different from the newSize, it becomes necessary to resize it.
				resize(newSize);
			}
		}
	}

	//return the total size of the list in BYTES without taking into account the memory allocation for the T objects inside
	template<typename T>
	inline size_t List<T>::size() const {
		return sizeof(List<T>) + memoryAllocated();
	}

	//The total number of BYTES allocated for the list without taking into account the memory allocated by the T objects inside
	template<typename T>
	inline size_t List<T>::memoryAllocated() const {
		return capacity * sizeof(T);
	}

	//The total number of BYTES currently being used by the actual list content.
	template<typename T>
	inline size_t List<T>::memoryUsed() const {
		return num * sizeof(*list);
	}

	template<typename T>
	inline void List<T>::resize(int newSize) {
		if (newSize <= 0) {
			clear();
			return;
		}

		if (newSize == size)
			return;

		list = (T*)resizeList(list, capacity, newSize, false);
		capacity = newSize;
		if (capacity < num) {
			num = capacity;
		}
	}
	template<typename T>
	inline void List<T>::resize(int newSize, int newGranularity)
	{
		assert(newGranularity > 0);
		granularity = newGranularity;

		if (newSize <= 0) {
			clear();
			return;
		}

		if (newSize == size)
			return;

		list = (T*)resizeList(list, capacity, newSize, false);
		capacity = newSize;
		if (capacity < num) {
			num = capacity;
		}
	}

	template<typename T>
	inline void List<T>::setNum(int newNum) {
		if (newNum >= 0) {
			resize(newNum);
		}
		num = newNum;
	}

	template<typename T>
	inline T* List<T>::getPtr() {
		return list;
	}

	template<typename T>
	inline T& List<T>::allocateNewElement()
	{
		if (!list) { //if empty list
			resize(granularity);
		}
		if (num == capacity) { //if list is full
			resize(capacity + granularity);
		}

		return list[num++];
	}

	template<typename T>
	inline List<T>& List<T>::operator=(const List& other) { //copy operator
		clear();
		num = other.num;
		capacity = other.capacity;
		granularity = other.granularity;

		if (capacity) {
			list = (T*)generateNewList(capacity, false);
			for (int i = 0; i < capacity; i++) {
				list[i] = other.list[i];
			}
		}

		return *this;
	}

	template<typename T>
	inline T& List<T>::operator[](int index)
	{
		return list[index];
	}

	template<typename T>
	inline const T& List<T>::operator[](int index) const
	{
		return list[index];
	}
}

#endif