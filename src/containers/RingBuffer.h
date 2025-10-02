#ifndef RINGBUFFER_H
#define RINGBUFFER_H

namespace Arboria {
	template<typename T, size_t N>
	class RingBuffer {
	public:
		inline RingBuffer() : head(0), tail(0) {
			memset(queue, 0, sizeof(queue));
		};

		inline void clear() {
			memset(queue, 0, sizeof(queue));
			head = 0;
			tail = 0;
		}
		inline const T& getFront() const {
			return queue[head];
		}
		inline T& getFront() {
			return queue[head];
		}
		inline void popFront() {
			if (head == tail) return;
			head = next(head);
		}
		inline void pushBack(const T& item) {
			//if head - tail >= N, yield
			queue[tail] = item;
			tail = next(tail);
		}

		[[nodiscard]] constexpr bool isEmpty() const noexcept { return head == tail; }
		[[nodiscard]] constexpr bool isFull() const noexcept { return next(head) == tail; }
		constexpr size_t capacity() const noexcept { return N; }
	private:
		T queue[N];
		unsigned int head;
		unsigned int tail;

		[[nodiscard]] constexpr size_t next(size_t idx) const noexcept { return (idx + 1) & (N - 1); } //basically a modulo

	};
}

#endif