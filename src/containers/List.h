#ifndef LIST_H
#define LIST_H

#include "../Heap.h"
#include "../utils/math.h"
#include <new>
#include <stdlib.h>
#include <assert.h>
#include <initializer_list>
#include <type_traits>

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
		bool remove(T const& elem);
		bool removeAt(int index);
		bool removeLast();
		bool contains(T const& elem);
		int findIndex(T const& elem) const;
		T* find(T const& elem) const;
		int findNull() const;
		void clear();
		void clearFree();
		void deleteContents(bool clear);
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
		granularity = newGranularity;
		list = NULL;
	}

	template<typename T>
	inline List<T>::List<T>(const List<T>& other) { //copy constructor
		list = NULL;
		*this = other;
	}

	template<typename T>
	inline List<T>::List<T>(const std::initializer_list<T>& other) : List() {
		resize(other.size());
		auto x = other.begin();
		while (x != other.end()) {
			append(*x++);
		}
	}

	template<typename T>
	inline List<T>::~List<T>() {
		clearFree();
	}

	template <typename T>
	inline void* generateNewList(size_t num, bool zeroBuffer) { //this is used by the second constructor
		T* ptr = NULL;
		if (zeroBuffer) {
			ptr = (T*)Mem_ClearedAlloc(sizeof(T) * num);
		}
		else {
			ptr = (T*)Mem_Alloc(sizeof(T) * num);
		}
		for (int i = 0; i < num; ++i) {
			//the new call does NOT allocate new memory, what it does is construct a T object at &ptr[i], which is equivalent to new(sizeof(T), &ptr[i])
			new (&ptr[i]) T;
		}
		return ptr;
	}

	template<typename T>
	inline void deleteListContent(void* ptr, int _num) {
		for (int i = 0; i < _num; ++i) {
			((T*)ptr)[i].~T();
		}
		Mem_Free(ptr);
	}

	template <typename T>
	inline void* resizeList(void* ptr, size_t oldSize, size_t newSize, bool zeroBuffer) {
		T* oldPtr = (T*)ptr;
		T* newPtr = NULL;
		if (newSize > 0) {
			newPtr = (T*)generateNewList<T>(newSize, zeroBuffer);
			//get the min between the oldSize and newSize, then copy the first min bytes from the oldptr to the newptr
			int carryOver = Math::iMin(oldSize, newSize);
			for (int i = 0; i < carryOver; ++i) {
				newPtr[i] = oldPtr[i];
			}
		}
		if (oldPtr)
			deleteListContent<T>(oldPtr, oldSize);
		return newPtr;
	}

	template<typename T>
	inline void List<T>::append(const T& elem) {
		if (!list) {
			resize(granularity);
		}
		if (num == capacity) {
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
		if (num == capacity) {
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
	inline bool List<T>::remove(T const& elem) {
		int index = findIndex(elem);
		if (index >= 0) {
			return removeAt(index);
		}

		return false;
	}

	template<typename T>
	inline bool List<T>::removeAt(int index) {
		if (index < 0 || index >= num) {
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
	inline bool List<T>::contains(T const& elem) {
		return findIndex(elem) > -1;
	}

	template<typename T>
	inline int List<T>::findIndex(T const& elem) const {
		//Since the list is unsorted, it's best to do a linear search
		for (int i = 0; i < num; i++) {
			if (list[i] == elem) {
				return i;
			}
		}

		return -1;
	}

	template<typename T>
	inline T* List<T>::find(T const& elem) const {
		//return a pointer to the element
		int index = findIndex(elem);
		if (index >= 0) {
			return &list[index];
		}
		return NULL;
	}

	template<typename T>
	inline int List<T>::findNull() const {
		if (std::is_pointer_v<T>) {
			//since we can't get a pointer to null for obvious reasons, findIndex isn't used here
			for (int i = 0; i < num; i++) {
				if (list[i] == NULL)
					return i;
			}
		}

		return -1;
	}

	template<typename T>
	inline void List<T>::clear()
	{
		num = 0;
	}

	template<typename T>
	inline void List<T>::clearFree()
	{
		if (list)
			delete[] list;

		list = NULL;
		num = 0;
		capacity = 0;
	}

	template<typename T>
	inline void List<T>::deleteContents(bool clear) {
		int i;
		for (i = 0; i < num; i++) {
			delete list[i];
			list[i] = NULL;
		}

		if (clear)
			clearFree();
		else
			memset(list, 0, capacity * sizeof(T));
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
		T* temp;

		assert(newSize >= 0);

		if (newSize <= 0) {
			clearFree();
			return;
		}

		if (newSize == capacity)
			return;

		temp = list;
		capacity = newSize;
		if (capacity < num)
			num = capacity;

		list = new T[capacity];
		
		for (int i = 0; i < num; ++i) {
			list[i] = temp[i];
		}

		if (temp)
			delete[] temp;

	}
	template<typename T>
	inline void List<T>::resize(int newSize, int newGranularity)
	{
		T* temp;
		assert(newSize >= 0);
		assert(newGranularity > 0);
		granularity = newGranularity;

		if (newSize <= 0) {
			clearFree();
			return;
		}

		if (newSize == capacity)
			return;

		temp = list;
		capacity = newSize;
		if (capacity < num)
			num = capacity;

		list = new T[capacity];

		for (int i = 0; i < num; ++i) {
			list[i] = temp[i];
		}

		if (temp)
			delete[] temp;

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
		clearFree();
		num = other.num;
		capacity = other.capacity;
		granularity = other.granularity;

		if (capacity) {
			list = new T[capacity];
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
		assert(unsigned(index) < unsigned(num));
		return list[index];
	}
}

#endif