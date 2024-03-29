#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#if defined(_WIN32)
#define MICROSESCOND_CLOCK
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef __GNUC__
#define ar_attribute(x) __attribute__(x)
#else
#define ar_attribute(x)
#endif

namespace Arboria {
	inline void NoP(void*) {
		return;
	}

	inline uint64_t getNanoseconds() {
		static uint64_t is_init = 0;
		#if defined(_WIN32)
		static LARGE_INTEGER window_freq;
		if (0 == is_init)
			QueryPerformanceFrequency(&window_freq);
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (uint64_t)((1e9 * now.QuadPart) / window_freq.QuadPart);
		#endif
	}

	inline double getMicroseconds() {
		return (double)(getNanoseconds() / 1e6);
	}
}

#endif