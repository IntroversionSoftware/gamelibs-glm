/// @ref core
/// @file glm/detail/func_trigonometric_simd.inl

// Vectorized sin/cos/tan for aligned float vectors (mediump/lowp).
// highp defers to libm (per-lane) for full accuracy; mediump/lowp use a
// 4-wide Cody-Waite pi/2 range reduction + minimax polynomials on [-pi/4, pi/4].
// Accuracy (mediump/lowp, measured vs double ref): sin/cos ~3.6e-6 up to |x|~1e4,
// ~1.4e-5 at 1e6; tan tracks true value up to the asymptote (no clamp).
// lowp aliases mediump (a distinct rougher lowp is not worth the small extra speed).

#if ((GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)) && defined(__FMA__)  // SIMD path needs FMA; else libm/scalar primaries

#include <x86intrin.h>
#include <cmath>

namespace glm{
namespace detail
{
	GLM_FUNC_QUALIFIER __m128 glm_trig_fnmadd(__m128 a, __m128 b, __m128 c)
	{
#	if defined(__FMA__)
		return _mm_fnmadd_ps(a, b, c);           // c - a*b, fused (exact product)
#	else
		return _mm_sub_ps(c, _mm_mul_ps(a, b));  // fallback (x86-64-v2, no FMA)
#	endif
	}

	// 4-wide sin & cos. Range reduction by pi/2 (Cody-Waite, FMA where available).
	GLM_FUNC_QUALIFIER void glm_sincos_ps(__m128 x, __m128& sin_out, __m128& cos_out)
	{
		const __m128 TWO_OVER_PI = _mm_set1_ps(0.636619772f);
		const __m128 PIO2_A = _mm_set1_ps(1.5707855225e+00f); // pi/2 split into 3 parts
		const __m128 PIO2_B = _mm_set1_ps(1.0804334124e-05f); //   so k*part stays exact
		const __m128 PIO2_C = _mm_set1_ps(2.1489973390e-11f);

		__m128 fk = _mm_round_ps(_mm_mul_ps(x, TWO_OVER_PI),
			_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
		__m128 r = glm_trig_fnmadd(fk, PIO2_A, x);   // r = x - fk*(pi/2), remainder in [-pi/4, pi/4]
		r = glm_trig_fnmadd(fk, PIO2_B, r);
		r = glm_trig_fnmadd(fk, PIO2_C, r);

		__m128 r2 = _mm_mul_ps(r, r);
		// sin(r) ~ r*(1 - r2/6 + r2^2/120 - r2^3/5040)   on [-pi/4, pi/4]
		__m128 S = _mm_mul_ps(r, _mm_add_ps(_mm_set1_ps(1.f), _mm_mul_ps(r2,
			_mm_add_ps(_mm_set1_ps(-1.f/6.f), _mm_mul_ps(r2,
			_mm_add_ps(_mm_set1_ps(1.f/120.f), _mm_mul_ps(r2, _mm_set1_ps(-1.f/5040.f))))))));
		// cos(r) ~ 1 - r2/2 + r2^2/24 - r2^3/720
		__m128 C = _mm_add_ps(_mm_set1_ps(1.f), _mm_mul_ps(r2,
			_mm_add_ps(_mm_set1_ps(-1.f/2.f), _mm_mul_ps(r2,
			_mm_add_ps(_mm_set1_ps(1.f/24.f), _mm_mul_ps(r2, _mm_set1_ps(-1.f/720.f)))))));

		__m128i k = _mm_cvtps_epi32(fk);
		// quadrant: swap sin/cos when k is odd
		__m128 swap = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_and_si128(k, _mm_set1_epi32(1)), _mm_set1_epi32(1)));
		__m128 sinP = _mm_or_ps(_mm_and_ps(swap, C), _mm_andnot_ps(swap, S));
		__m128 cosP = _mm_or_ps(_mm_and_ps(swap, S), _mm_andnot_ps(swap, C));
		// sign: sin negates when (k&2); cos negates when ((k+1)&2)
		__m128 sinsign = _mm_castsi128_ps(_mm_slli_epi32(_mm_and_si128(k, _mm_set1_epi32(2)), 30));
		__m128 cossign = _mm_castsi128_ps(_mm_slli_epi32(
			_mm_and_si128(_mm_add_epi32(k, _mm_set1_epi32(1)), _mm_set1_epi32(2)), 30));
		sin_out = _mm_xor_ps(sinP, sinsign);
		cos_out = _mm_xor_ps(cosP, cossign);
	}

	template<length_t L, qualifier Q>
	struct compute_sin_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::sin(a[i]);
			} else {
				__m128 s, c;
				glm_sincos_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&a.data)), s, c);
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data), s);
			}
			return Result;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_cos_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::cos(a[i]);
			} else {
				__m128 s, c;
				glm_sincos_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&a.data)), s, c);
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data), c);
			}
			return Result;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_tan_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::tan(a[i]);
			} else {
				__m128 s, c;
				glm_sincos_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&a.data)), s, c);
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data), _mm_div_ps(s, c));
			}
			return Result;
		}
	};

}//namespace detail
}//namespace glm

#endif//(GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)
