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
	GLM_FUNC_QUALIFIER detail::uint8 bitfieldReverse(detail::uint8 x)
	{
		return __builtin_bitreverse8(x);
	}

	template<>
	GLM_FUNC_QUALIFIER detail::uint16 bitfieldReverse(detail::uint16 x)
	{
		return __builtin_bitreverse16(x);
	}

	template<>
	GLM_FUNC_QUALIFIER detail::uint32 bitfieldReverse(detail::uint32 x)
	{
		return __builtin_bitreverse32(x);
	}

	template<>
	GLM_FUNC_QUALIFIER detail::uint64 bitfieldReverse(detail::uint64 x)
	{
		return __builtin_bitreverse64(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint8 x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint16 x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint32 x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint64 x)
	{
		return static_cast<int>(__builtin_popcountll(x));
	}

}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
