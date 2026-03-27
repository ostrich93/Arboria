#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <exception>
#include "../Heap.h"

namespace Arboria {

	template<typename T>
	class Deque {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;

		struct DequeNode {
			T value;
			DequeNode* next = nullptr;
			DequeNode* prev = nullptr;

			DequeNode() {}
			DequeNode(const T& _value) : value(_value) {}
			DequeNode(const T& _value, DequeNode* _next) : value(_value), next(_next) {
				next->prev = this;
			}

		};
		Deque() {};
		Deque(const Deque& other);
		//Deque(Deque&&);

		~Deque();
		//Deque& operator=(const Deque& other);
		//Deque& operator=(Deque&& other);

		//capacity
		bool isEmpty() const noexcept { return top == nullptr; }
		int count() const noexcept { return currentLength; }

		//element access
		reference front() { return top; }
		const_reference front() const { return top; }
		reference back() { return rear; }
		const_reference back() const { return rear; }

		void push_front(const T& v);
		//void push_front(T&& v);
		void push_back(const T& v);
		//void push_back(T&& v);
		void pop_front();
		void pop_back();
		void clear() noexcept;

	private:
		DequeNode* top = nullptr;
		DequeNode* rear = nullptr;
		int currentLength = 0;
	};

	
	template<typename T>
	inline Deque<T>::Deque(const Deque& other)
	{
		if (&other == this)
			return;
		*this = other;
	}

	template<typename T>
	inline Deque<T>::~Deque()
	{
		clear();
	}

	template<typename T>
	void Deque<T>::clear() noexcept {
		while (!isEmpty()) {
			pop_front();
		}
	}

	template<typename T>
	void Deque<T>::pop_front() {
		if (isEmpty()) {
			return;
		}

		DequeNode* temp = top;
		top = temp->next;

		if (top) top->prev = nullptr; //if the next element isn't null, set the prev pointer to null, otherwise the deque had only one element, so also point rear to null
		else rear = nullptr;

		delete temp;

		currentLength--;
	}

	template <typename T>
	void Deque<T>::pop_back() {
		if (isEmpty()) return;

		DequeNode* temp = rear;
		rear = rear->prev;

		if (rear)//if rear is only node in deque
			rear->next = nullptr;
		else front = nullptr;

		delete temp;

		currentLength--;
	}

	template<typename T>
	inline void Deque<T>::push_front(const T& v)
	{
		if (isEmpty()) {
			front = rear = new DequeNode(v);
		}
		else {
			DequeNode* temp = new DequeNode(v, front);
			front = temp;
		}

		currentLength++;
	}

	template<typename T>
	inline void Deque<T>::push_back(const T& v)
	{
		if (isEmpty()) {
			front = rear = new DequeNode(v);
		}
		else {
			DequeNode* temp = new DequeNode(v);
			temp->prev = rear;
			rear->next = temp;
			rear = temp;
		}

		currentLength++;
	}
}

#endif