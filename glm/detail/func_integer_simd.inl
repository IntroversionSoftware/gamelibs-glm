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
