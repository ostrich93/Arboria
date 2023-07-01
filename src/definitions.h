#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#ifdef _MSC_VER
#define ALIGN16(x) __declspec(align(16))x
#define ALIGNTYPE16 __declspec(align(16))
#define ALIGNTYPE128 __declspec(align(128))
#else
#define ALIGN16(x)	x __attribute__ ((aligned(16)))
#define ALIGNTYPE16
#define ALIGNTYPE128
#endif

#define ARBORIA_INLINE				inline
#ifdef _MSC_VER
#define ARBORIA_FORCE_INLINE		__forceinline
#else
#define ARBORIA_FORCE_INLINE		inline
#endif

#ifndef WIN32
#define WIN32

#endif


#elif defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__) || defined(__GNUC__) || defined(__clang__)

#define _alloca	alloca
#define ALIGN16(x)		x __attribute__ ((aligned(16)))
#define ALIGNTYPE16		__attribute__ ((aligned(16)))
#define ALIGNTYPE128	__attribute__ ((aligned(128)))

#define __cdecl
#endif

#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define _alloca16(x) ((void *)ALIGN((UINT_PTR)_alloca(ALIGN(x, 16) + 16), 16))
#define _alloca128(x) ((void *)ALIGN((UINT_PTR)_alloca(ALIGN(x, 128) + 128), 128))
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
#define ARRAY_ELEMENT_SIZE(a)		(sizeof(a)/sizeof(a[0]))

#define PHYSFS_SEPARATOR	"/"
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#define DIR_SEPARATOR_STR	"\\"
	#define DIR_SEPARATOR_CHAR	'\\'
	#define NEWLINE				"\r\n"
#elif defined(__unix__) || defined(__linux__)
	#define DIR_SEPARATOR_STR	"/"
	#define DIR_SEPARATOR_CHAR	'/'
	#define NEWLINE				"\n"
	#include <sys/stat.h>
#elif defined(__APPLE__) || defined(__MACH__)
	#define DIR_SEPARATOR_STR	":"
	#define DIR_SEPARATOR_CHAR	':'
	#define NEWLINE				"\n"
	#include <sys/stat.h>
#else
	#define DIR_SEPARATOR_STR	NULL
	#define DIR_SEPARATOR_CHAR	NULL
	#define NEWLINE				"\n"
#endif

// Warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is uninitialized. Always initialize a member variable (type.6).
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"          // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wclass-memaccess"  // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

#if defined(_MSC_VER)
#define NO_RETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define NO_RETURN __attribute__((noreturn))
#else
#define NO_RETURN
#endif

#endif