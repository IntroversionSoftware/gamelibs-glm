#include "../simd/integer.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm
{
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

#endif // GLM_ARCH & GLM_ARCH_SSE2_BIT

#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm
{
	template<>
	GLM_FUNC_QUALIFIER uint bitfieldReverse(uint x)
	{
		return __builtin_bitreverse32(x);
	}

	template<>
	GLM_FUNC_QUALIFIER detail::uint64 bitfieldReverse(detail::uint64 x)
	{
		return __builtin_bitreverse64(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(uint x)
	{
		return __builtin_popcount(x);
	}

	template<>
	GLM_FUNC_QUALIFIER int bitCount(detail::uint64 x)
	{
		return static_cast<int>(__builtin_popcountll(x));
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, uint, Q> bitfieldReverse(vec<L, uint, Q> const &v)
	{
		vec<L, uint, Q> x;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			x[i] = bitfieldReverse(v[i]);
		return x;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, glm::uint64, Q> bitfieldReverse(vec<L, glm::uint64, Q> const &v)
	{
		vec<L, glm::uint64, Q> x;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			x[i] = bitfieldReverse(v[i]);
		return x;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitfieldReverse(vec<L, int, Q> const &v)
	{
		vec<L, int, Q> x;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			x[i] = bitfieldReverse(__builtin_bit_cast(uint, v[i]));
		return x;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, glm::int64, Q> bitfieldReverse(vec<L, glm::int64, Q> const &v)
	{
		vec<L, glm::int64, Q> x;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			x[i] = bitfieldReverse(__builtin_bit_cast(glm::uint64, v[i]));
		return x;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitCount(vec<L, uint, Q> const &v)
	{
		vec<L, int, Q> rv;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			rv[i] = bitCount(v[i]);
		return rv;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitCount(vec<L, detail::uint64, Q> const &v)
	{
		vec<L, int, Q> rv;
		#pragma clang loop unroll(enable)
		for (int i = 0; i < L; i++)
			rv[i] = bitCount(v[i]);
		return rv;
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitCount(vec<L, int, Q> const &v)
	{
		vec<L, uint, Q> x(*reinterpret_cast<vec<L, uint, Q> const *>(&v));
		return bitCount(x);
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitCount(vec<L, detail::int64, Q> const &v)
	{
		vec<L, glm::uint64, Q> x(*reinterpret_cast<vec<L, glm::uint64, Q> const *>(&v));
		return bitCount(x);
	}
}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
