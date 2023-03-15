#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include <float.h>
#include <stdint.h>

namespace Arboria {

	#ifdef INFINITE
	#undef INFINITE
	#endif

	#ifdef FLT_EPSILON
	#undef FLT_EPSILON
	#endif

	#define DEG2RAD(a)		( (a) * DEG_2_RAD);
	#define RAD2DEG(a)		( (a) * RAD_2_DEG);

	#define FLOATSIGNBITSET(f)			((*(const unsigned int *)&(f)) >> 31)
	#define FLOATSIGNBITNOTSET(f)		((~(*(const unsigned int *)&(f))) >> 31)
	#define FLOATNOTZERO(f)				((*(const unsigned int *)&(f)) & ~(1 << 31))
	#define INTSIGNBITSET(i)			(((const unsigned int)(i)) >> 31)
	#define INTSIGNBITNOTSET(i)			((~((const unsigned int)(i))) >> 31)

	#define IEEE_FLT_MANTISSA_BITS		23

	static const float PI = 3.14159265358979323846f;
	static const float TAU = 6.28318530717958647692f;
	static const float HALF_PI = 0.5f * PI;
	static const float QUARTER_PI = 0.25f * PI;
	static const float THREE_HALVES_PI = 1.5f * PI;
	static const float INV_PI = 0.31830988618379067154;
	static const float INV_TWO_PI = 0.63661977236758134308;
	static const float E = 2.71828182845904523536f;
	static const float LOG2E = 1.4426950408889634074f;
	static const float LOG10E = 0.43429448190325182765f;
	static const float LN2 = 0.69314718055994530942f;
	static const float LN10 = 2.30258509299404568402f;
	static const float SQRT_TWO = 1.41421356237309504880f;
	static const float SQRT_THREE = 1.73205080756887729352f;
	static const float INV_SQRT_TWO = 0.70710678118654752440f;
	static const float INV_SQRT_THREE = 0.57735026918962576450f;
	static const float DEG_2_RAD = PI / 180.0f;
	static const float RAD_2_DEG = 180.0f / PI;
	static const float SEC2MS = 1000.0f;
	static const float MS2SEC = 0.001f;
	static const float INFINITE = 1e30f;
	static const float FLT_EPSILON = 1.192092896e-07f;
	static const int NAN_VALUE = 0x7f800000;
	static const int SMALLEST_NON_DENORMAL = 1 << IEEE_FLT_MANTISSA_BITS;
	static const float FLT_SMALLEST_NON_DENORMAL = *reinterpret_cast<const float*>(&SMALLEST_NON_DENORMAL);


	class Math {
	public:
		static int iMin(int a, int b);
		static int iMax(int a, int b);
		static signed char clampChar(int value);
		static signed short clampShort(int value);
		static int clampInt(int min, int max, int value);
		static float clampFloat(float min, float max, float value);
		static bool isPowerOfTwo(int x);
		static float floor(float f);
		static int ftoi(float f);
		static unsigned int ftol(float f);
		static float ceil(float f);
		static float invSqrt(float x);
		static float sqrt(float x);
	};

	inline int Math::iMin(int a, int b) {
		return (a < b ? a : b);
	}

	inline int Math::iMax(int a, int b) {
		return (a > b ? a : b);
	}

	inline signed char Math::clampChar(int value) {
		if (value < -128) {
			return -128;
		}
		if (value > 127) {
			return 127;
		}
		return value;
	}

	inline signed short Math::clampShort(int value) {
		if (value < -32768) {
			return -32768;
		}
		if (value > 32767) {
			return 32767;
		}
		return value;
	}

	inline int Math::clampInt(int min, int max, int value) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}

	inline float Math::clampFloat(float min, float max, float value) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}

	inline bool Math::isPowerOfTwo(int x) {
		return (x & (x - 1)) == 0 && x > 0;
	}

	inline float Math::floor(float f) {
		return floorf(f);
	}

	inline int Math::ftoi(float f) {
		return (int) f;
	}

	inline unsigned int Math::ftol(float f) {
		return (unsigned int)f;
	}

	inline float Math::ceil(float f) {
		return ceilf(f);
	}

	inline float Math::invSqrt(float x) {
		return (x > FLT_SMALLEST_NON_DENORMAL) ? sqrtf(1.0f / x) : INFINITE;
	}

	inline float Math::sqrt(float x) {
		return (x >= 0.0f) ? sqrtf(x) : 0.0f;
	}
}

#endif