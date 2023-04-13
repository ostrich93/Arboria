#include <malloc.h>
#include <memory.h>
#include "Heap.h"
#include "definitions.h"

#undef new

void* Mem_Alloc16(const size_t size)
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

void* Mem_ClearedAlloc(const size_t size)
{
	void* mem = Mem_Alloc(size);
	memset(mem, 0, size);
	return mem;
}
