#include "../simd/integer.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm{
#	if GLM_ARCH & GLM_ARCH_AVX_BIT
	template<>
	GLM_FUNC_QUALIFIER int bitCount(uint x)
	{
		return _mm_popcnt_u32(x);
	}

#	if(GLM_MODEL == GLM_MODEL_64)
	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint64 x)
	{
		return static_cast<int>(_mm_popcnt_u64(x));
	}
#	endif//GLM_MODEL
#	endif//GLM_ARCH

}//namespace glm

#endif//GLM_ARCH & GLM_ARCH_SSE2_BIT

#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm
{
	template<>
	GLM_FUNC_QUALIFIER std::uint8_t bitfieldReverse(std::uint8_t x)
	{
		return __builtin_bitreverse8(x);
	}

	template<>
	GLM_FUNC_QUALIFIER std::uint16_t bitfieldReverse(std::uint16_t x)
	{
		return __builtin_bitreverse16(x);
	}

	template<>
	GLM_FUNC_QUALIFIER std::uint32_t bitfieldReverse(std::uint32_t x)
	{
		return __builtin_bitreverse32(x);
	}

	template<>
	GLM_FUNC_QUALIFIER std::uint64_t bitfieldReverse(std::uint64_t x)
	{
		return __builtin_bitreverse64(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(std::uint8_t x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(std::uint16_t x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(std::uint32_t x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(std::uint64_t x)
	{
		return static_cast<int>(__builtin_popcountll(x));
	}

}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
