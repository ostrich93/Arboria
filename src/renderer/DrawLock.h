#ifndef DRAW_LOCK_H
#define DRAW_LOCK_H

namespace Arboria {
	template<typename T>
	class DrawUniqueLock {
		T& _mutex;
		const bool _enabled;

	public:
		DrawUniqueLock(T& mutex, bool isMultithreaded = false) : _mutex(mutex), _enabled(isMultithreaded) {
			if (_enabled)
				_mutex.lock();
		}
		~DrawUniqueLock() {
			if (_enabled) {
				_mutex.unlock();
			}
		}
	};
}

#endif