#ifndef STACK_H
#define STACK_H

#include "Deque.h"

namespace Arboria {
	
	template<typename T, class Container = Deque<T>>
	class Stack {
	protected:
		Container c;

	public:
		using value_type = typename Container::value_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;
		Stack() : Stack(Container()) {}
		explicit Stack(const Container& rhs) : c(rhs) {}

		void push(const value_type& v) { c.push_back(v); }
		void pop() { c.pop_back(); }
		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }

		bool isEmpty() const noexcept { return c.isEmpty(); }
		int count() const noexcept { return c.count(); }
	};
}

#endif