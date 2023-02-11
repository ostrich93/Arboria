#include "Heap.h"
#include <malloc.h>
#include <memory.h>

namespace Arboria {
	void* Mem_Alloc16(const int size)
	{
		if (!size) return NULL;

		const int paddedSize = (size + 15) & ~15;
		return _aligned_malloc(paddedSize, 16);
	}
	void Mem_Free16(void* ptr)
	{
		if (ptr == NULL) return;
		_aligned_free(ptr);
	}

	void* Mem_ClearedAlloc(const int size)
	{
		void* mem = Mem_Alloc(size);
		memset(mem, 0, size);
		return mem;
	}
}