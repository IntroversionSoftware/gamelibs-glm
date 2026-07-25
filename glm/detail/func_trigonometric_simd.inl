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
#include "simd_transcendental.h"   // glm_exp_ps / glm_log_ps for hyperbolics

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

	// --- Inverse trig: asin/acos/atan (polynomial, aligned mediump/lowp) ---
	// highp/L<3 defer to libm.

	// 4-wide atan: reduce |x|>1 via pi/2 - atan(1/x); degree-13 minimax on [-1,1].
	GLM_FUNC_QUALIFIER __m128 glm_atan_ps(__m128 x)
	{
		__m128 sign = _mm_and_ps(x, _mm_set1_ps(-0.0f));
		__m128 ax = _mm_andnot_ps(_mm_set1_ps(-0.0f), x);
		__m128 gt1 = _mm_cmpgt_ps(ax, _mm_set1_ps(1.0f));
		__m128 z = _mm_blendv_ps(ax, _mm_div_ps(_mm_set1_ps(1.0f), ax), gt1); // reduce to [0,1]
		__m128 z2 = _mm_mul_ps(z, z);
		__m128 p = _mm_set1_ps(-0.01172120f);
		p = _mm_fmadd_ps(p, z2, _mm_set1_ps(0.05265332f));
		p = _mm_fmadd_ps(p, z2, _mm_set1_ps(-0.11643287f));
		p = _mm_fmadd_ps(p, z2, _mm_set1_ps(0.19354346f));
		p = _mm_fmadd_ps(p, z2, _mm_set1_ps(-0.33262347f));
		p = _mm_fmadd_ps(p, z2, _mm_set1_ps(0.99997726f));
		p = _mm_mul_ps(p, z);                          // atan(z)
		__m128 r = _mm_blendv_ps(p, _mm_sub_ps(_mm_set1_ps(1.57079632679f), p), gt1);
		return _mm_or_ps(r, sign);
	}

	template<length_t L, qualifier Q>
	struct compute_atan_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::atan(a[i]);
			} else {
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data),
					glm_atan_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&a.data))));
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_atan2_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& y, vec<L, float, Q> const& x)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::atan2(y[i], x[i]);
			} else {
				__m128 vy = _mm_loadu_ps(reinterpret_cast<const float*>(&y.data));
				__m128 vx = _mm_loadu_ps(reinterpret_cast<const float*>(&x.data));
				__m128 a = glm_atan_ps(_mm_div_ps(vy, vx));     // atan(y/x); x==0 -> +-inf -> +-pi/2
				// when x < 0, shift by copysign(pi, y) to land in the correct quadrant
				__m128 xlt0 = _mm_cmplt_ps(vx, _mm_setzero_ps());
				__m128 pi_signed = _mm_or_ps(_mm_set1_ps(3.14159265359f), _mm_and_ps(vy, _mm_set1_ps(-0.0f)));
				a = _mm_add_ps(a, _mm_and_ps(xlt0, pi_signed));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), a);
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_asin_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::asin(a[i]);
			} else {
				// Cephes asinf: sqrt reduction for |x|>0.5 keeps accuracy near +-1
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				__m128 sign = _mm_and_ps(x, _mm_set1_ps(-0.0f));
				__m128 av = _mm_andnot_ps(_mm_set1_ps(-0.0f), x);
				__m128 big = _mm_cmpgt_ps(av, _mm_set1_ps(0.5f));
				__m128 zbig = _mm_mul_ps(_mm_set1_ps(0.5f), _mm_sub_ps(_mm_set1_ps(1.0f), av));
				__m128 z = _mm_blendv_ps(_mm_mul_ps(av, av), zbig, big);
				__m128 xx = _mm_blendv_ps(av, _mm_sqrt_ps(zbig), big);
				__m128 p = _mm_set1_ps(4.2163199048E-2f);
				p = _mm_fmadd_ps(p, z, _mm_set1_ps(2.4181311049E-2f));
				p = _mm_fmadd_ps(p, z, _mm_set1_ps(4.5470025998E-2f));
				p = _mm_fmadd_ps(p, z, _mm_set1_ps(7.4953002686E-2f));
				p = _mm_fmadd_ps(p, z, _mm_set1_ps(1.6666752422E-1f));
				__m128 r = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(p, z), xx), _mm_set1_ps(1.0f)), xx);
				r = _mm_blendv_ps(r, _mm_sub_ps(_mm_set1_ps(1.57079632679f), _mm_add_ps(r, r)), big);
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), _mm_or_ps(r, sign));
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_acos_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::acos(a[i]);
			} else {
				vec<L, float, Q> asin_v = compute_asin_vec<L, float, Q, true>::call(a);
				__m128 as = _mm_loadu_ps(reinterpret_cast<const float*>(&asin_v.data));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), _mm_sub_ps(_mm_set1_ps(1.57079632679f), as));
			}
			return R;
		}
	};

	// --- Hyperbolics via glm_exp_ps / glm_log_ps (aligned mediump/lowp) ---
	// highp/L<3 defer to libm.
	template<length_t L, qualifier Q>
	struct compute_sinh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::sinh(a[i]);
			} else {
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				// sinh via expm1 avoids e^x - e^-x cancellation near 0
				__m128 r = _mm_mul_ps(_mm_sub_ps(glm_expm1_ps(x), glm_expm1_ps(_mm_sub_ps(_mm_setzero_ps(), x))), _mm_set1_ps(0.5f));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), r);
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_cosh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::cosh(a[i]);
			} else {
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				__m128 r = _mm_mul_ps(_mm_add_ps(glm_exp_ps(x), glm_exp_ps(_mm_sub_ps(_mm_setzero_ps(), x))), _mm_set1_ps(0.5f));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), r);
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_tanh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::tanh(a[i]);
			} else {
				// Direct minimax rational (Eigen/Cephes) - accurate near 0 and skips exp
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				x = _mm_max_ps(_mm_set1_ps(-9.f), _mm_min_ps(_mm_set1_ps(9.f), x));
				__m128 x2 = _mm_mul_ps(x, x);
				__m128 p = _mm_set1_ps(-2.76076847742355e-16f);
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps( 2.00018790482477e-13f));
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps(-8.60467152213735e-11f));
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps( 5.12229709037114e-08f));
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps( 1.48572235717979e-05f));
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps( 6.37261928875436e-04f));
				p = _mm_fmadd_ps(p, x2, _mm_set1_ps( 4.89352455891786e-03f));
				__m128 num = _mm_mul_ps(x, p);
				__m128 q = _mm_set1_ps(1.19825839466702e-06f);
				q = _mm_fmadd_ps(q, x2, _mm_set1_ps(1.18534705686654e-04f));
				q = _mm_fmadd_ps(q, x2, _mm_set1_ps(2.26843463243900e-03f));
				q = _mm_fmadd_ps(q, x2, _mm_set1_ps(4.89352518554385e-03f));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), _mm_div_ps(num, q));
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_asinh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::asinh(a[i]);
			} else {
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				__m128 sign = _mm_and_ps(x, _mm_set1_ps(-0.0f));
				__m128 ax = _mm_andnot_ps(_mm_set1_ps(-0.0f), x);            // |x|
				__m128 root = _mm_sqrt_ps(_mm_fmadd_ps(ax, ax, _mm_set1_ps(1.f)));
				// asinh = log1p(|x| + |x|^2/(sqrt(x^2+1)+1)); log1p avoids ln(~1) cancellation
				__m128 t = _mm_add_ps(ax, _mm_div_ps(_mm_mul_ps(ax, ax), _mm_add_ps(root, _mm_set1_ps(1.f))));
				__m128 r = _mm_xor_ps(glm_log1p_ps(t), sign);               // asinh is odd
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), r);
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_acosh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::acosh(a[i]);
			} else {
				// acosh = log1p((x-1) + sqrt((x-1)(x+1))); factored sqrt + log1p avoid
				// the x^2-1 / ln(~1) cancellation as x -> 1.
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				__m128 xm1 = _mm_sub_ps(x, _mm_set1_ps(1.f));
				__m128 s = _mm_sqrt_ps(_mm_mul_ps(xm1, _mm_add_ps(x, _mm_set1_ps(1.f))));
				__m128 r = glm_log1p_ps(_mm_add_ps(xm1, s));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), r);
			}
			return R;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_atanh_vec<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& a)
		{
			vec<L, float, Q> R;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) R[i] = std::atanh(a[i]);
			} else {
				__m128 x = _mm_loadu_ps(reinterpret_cast<const float*>(&a.data));
				// atanh = 0.5*(log1p(x) - log1p(-x)); avoids ln(~1) cancellation near 0
				__m128 r = _mm_mul_ps(_mm_set1_ps(0.5f),
					_mm_sub_ps(glm_log1p_ps(x), glm_log1p_ps(_mm_sub_ps(_mm_setzero_ps(), x))));
				_mm_storeu_ps(reinterpret_cast<float*>(&R.data), r);
			}
			return R;
		}
	};

}//namespace detail
}//namespace glm

#endif//(GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)
