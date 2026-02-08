#ifndef NUMERICS_H
#define NUMERICS_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace Arboria::Numerics {
	template<typename UIntType>
	static constexpr UIntType rol(UIntType x, size_t shift) {
		static_assert(std::is_unsigned<UIntType>::value, "result type must be an unsigned integer type");
		using limits = std::numeric_limits<UIntType>;
		return ((static_cast<UIntType>(x) << shift) | (static_cast<UIntType>(x) >> (limits::digits - shift)));
	}

	template<typename UIntType>
	static constexpr UIntType ror(UIntType x, size_t shift) {
		static_assert(std::is_unsigned<UIntType>::value, "result type must be an unsigned integer type");
		using limits = std::numeric_limits<UIntType>;
		return ((static_cast<UIntType>(x) >> shift) | (static_cast<UIntType>(x) << (limits::digits - shift)));
	}

	const constexpr auto rol8 = rol<uint8_t>;
	const constexpr auto ror8 = ror<uint8_t>;
	const constexpr auto rol16 = rol<uint16_t>;
	const constexpr auto ror16 = ror<uint16_t>;
	const constexpr auto rol32 = rol<uint32_t>;
	const constexpr auto ror32 = ror<uint32_t>;
	const constexpr auto rol64 = rol<uint64_t>;
	const constexpr auto ror64 = ror<uint64_t>;
}

#endif