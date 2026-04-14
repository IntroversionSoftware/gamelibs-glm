#pragma once

#include "setup.hpp"

// When a native IEEE 754 binary16 type is available (Clang/GCC _Float16 on
// x86_64/aarch64, etc.), toFloat32/toFloat16 reduce to a cast and can be
// constexpr. The bit-twiddling fallback uses a non-literal union type and
// cannot be constexpr before C++23.
#if defined(__FLT16_MANT_DIG__) && __FLT16_MANT_DIG__ == 11
#	define GLM_HAS_NATIVE_FLOAT16 1
#	define GLM_HALF_CONSTEXPR constexpr
#else
#	define GLM_HAS_NATIVE_FLOAT16 0
#	define GLM_HALF_CONSTEXPR
#endif

namespace glm{
namespace detail
{
	typedef short hdata;

	GLM_FUNC_DECL GLM_HALF_CONSTEXPR float toFloat32(hdata value);
	GLM_FUNC_DECL GLM_HALF_CONSTEXPR hdata toFloat16(float const& value);

}//namespace detail
}//namespace glm

#include "type_half.inl"
