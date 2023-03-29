#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "framework/Engine.h"
#include <string>

#if defined(_WIN32)
#define MICROSESCOND_CLOCK
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Arboria {
	extern Engine* engine;

	void NoP(void*) {
		return;
	}

	uint64_t getNanoseconds() {
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

	double getMicroseconds() {
		return (double)(getNanoseconds() / 1e6);
	}
}

#endif