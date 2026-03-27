#ifndef QUEUE_H
#define QUEUE_H

#include "Deque.h"

namespace Arboria {

	template<typename T, class Container = Deque<T>>
	class Queue {
	protected:
		Container c;

	public:
		using value_type = typename Container::value_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;

		Queue() : Queue(Container()) {}
		explicit Queue(const Queue& rhs) : c(rhs) {}

		void push(const value_type& v) { c.push_back(v); }
		void pop() { c.pop_front(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }

		bool isEmpty() const noexcept { return c.isEmpty(); }
		int count() const noexcept { return c.count(); }
	};
}

#endif