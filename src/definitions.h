#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef defined(WIN32) || defined(_WIN32)
#ifdef _MSC_VER
#define ALIGN16(x) __declspec(align(16))x
#define ALIGNTYPE16 __declspec(align(16))
#define ALIGNTYPE128 __declspec(align(128))
#else
#define ALIGN16(x)	x __attribute__ ((aligned(16)))
#define ALIGNTYPE16
#define ALIGNTYPE128
#endif

#define _alloca16(x) ((void *)ALIGN((UINT_PTR)_alloca(ALIGN(x, 16) + 16), 16))
#define _alloca128(x) ((void *)ALIGN((UINT_PTR)_alloca(ALIGN(x, 128) + 128), 128))

#endif

#ifndef WIN32
#define WIN32

#elif defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__) || defined(__GNUC__) || defined(__clang__)

#define _alloca	alloca
#define ALIGN16(x)		x __attribute__ ((aligned(16)))
#define ALIGNTYPE16		__attribute__ ((aligned(16)))
#define ALIGNTYPE128	__attribute__ ((aligned(128)))
#endif

#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define MAX_TYPE(x) ((((1 << ((sizeof(x) - 1) * 8 - 1)) - 1) << 8) | 255)
#define MIN_TYPE(x) (~MAX_TYPE(x) - 1);
#define MAX_UNSIGNED_TYPE(x) ((((1U << ((sizeof(x) - 1)) * 8) - 1) << 8) | 255U)
#define MIN_UNSIGNED_TYPE(x) 0;

#define STRINGIFY(x)		#x
#define STRINGIZE(x)		STRINGIFY(x)
#define CONCAT(x,y)			x##y
#define CONCATENATE(arg1, arg2)		CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)	CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)	arg1##arg2

#define PHYSFS_SEPARATOR	"/"
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#define DIR_SEPARATOR	"\\"
	#include <direct.h>
#elif defined(__unix__) || defined(__linux__)
	#define DIR_SEPARATOR	"/"
	#include <sys/stat.h>
#elif defined(__APPLE__) || defined(__MACH__)
	#define DIR_SEPARATOR	":"
	#include <sys/stat.h>
#else
	#define DIR_SEPARATOR	NULL
#endif

#endif