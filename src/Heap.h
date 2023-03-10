#ifndef __HEAP_H__
#define __HEAP_H__

#include "definitions.h"

namespace Arboria {
	void* Mem_Alloc16(const int size);
	void Mem_Free16(void* ptr);
	void* Mem_ClearedAlloc(const int size); //allocate memory and set the bytes to NULL values.

	inline void* Mem_Alloc(const int size) { return Mem_Alloc16(size); }
	inline void Mem_Free(void* ptr) { return Mem_Free16(ptr); }

	inline void* operator new(size_t s) { return Mem_Alloc(s); }
	inline void operator delete(void* p) { Mem_Free(p); }
	inline void* operator new[](size_t s) { return Mem_Alloc(s); }
	inline void operator delete[](void* p) { Mem_Free(p); }
}

#endif