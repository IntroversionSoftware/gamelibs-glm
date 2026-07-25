/// @ref core
/// @file glm/detail/simd_transcendental.h
///
/// Shared 4-wide SIMD exp/log primitives used by both func_exponential_simd.inl
/// (exp/log/exp2/log2) and func_trigonometric_simd.inl (hyperbolics). Kept in one
/// header so the definitions exist regardless of the exponential-vs-trigonometric
/// include order. Accuracy ~1-2e-7 relative; used only for aligned mediump/lowp.

#pragma once

#include "../simd/platform.h"

// Requires FMA (used throughout the reductions/polynomials). Without it, callers
// fall back to their libm/scalar primaries (e.g. x86-64-v2 builds).
#if ((GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)) && GLM_HAS_FMA

#if defined(_M_ARM64) || defined(__aarch64__)
#	include <arm_neon.h>
#elif defined(_M_X64)
#	include <intrin.h>
#elif defined(__x86_64__)
#	include <x86intrin.h>
#endif

namespace glm{
namespace detail
{
	GLM_FUNC_QUALIFIER __m128 glm_explog_fnmadd(__m128 a, __m128 b, __m128 c)
	{
#	if GLM_HAS_FMA
		return _mm_fnmadd_ps(a, b, c);
#	else
		return _mm_sub_ps(c, _mm_mul_ps(a, b));
#	endif
	}

	// e^x = 2^k * e^r, k = round(x/ln2), r in [-ln2/2, ln2/2]
	GLM_FUNC_QUALIFIER __m128 glm_exp_ps(__m128 x)
	{
		x = _mm_min_ps(_mm_set1_ps(88.3762626f), x);
		x = _mm_max_ps(_mm_set1_ps(-87.3365478f), x);
		__m128 k = _mm_round_ps(_mm_mul_ps(x, _mm_set1_ps(1.44269504088896341f)),
			_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
		__m128 r = glm_explog_fnmadd(k, _mm_set1_ps(0.693359375f), x);
		r = glm_explog_fnmadd(k, _mm_set1_ps(-2.12194440e-4f), r);
		__m128 p = _mm_set1_ps(1.f/720.f);
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/120.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/24.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/6.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/2.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f));
		__m128i pow2 = _mm_slli_epi32(_mm_add_epi32(_mm_cvtps_epi32(k), _mm_set1_epi32(127)), 23);
		return _mm_mul_ps(p, _mm_castsi128_ps(pow2));
	}

	// ln(x) = e*ln2 + ln(m), m in [sqrt(2)/2, sqrt(2)); Cephes minimax (~1 ULP). The special cases
	// follow logf: ln(0) = -inf, ln(+inf) = +inf, and a negative or NaN input gives NaN. Denormals
	// are scaled into the normal range first, since the exponent bits don't describe them.
	GLM_FUNC_QUALIFIER __m128 glm_log_ps(__m128 x)
	{
		__m128 const in = x;
		__m128 const denormal = _mm_cmplt_ps(x, _mm_set1_ps(1.17549435e-38f));
		x = _mm_blendv_ps(x, _mm_mul_ps(x, _mm_set1_ps(33554432.0f)), denormal); // 2^25
		__m128i xi = _mm_castps_si128(x);
		__m128 e = _mm_cvtepi32_ps(_mm_sub_epi32(_mm_srli_epi32(xi, 23), _mm_set1_epi32(127)));
		e = _mm_sub_ps(e, _mm_and_ps(denormal, _mm_set1_ps(25.f)));
		__m128 m = _mm_castsi128_ps(_mm_or_si128(_mm_and_si128(xi, _mm_set1_epi32(0x007FFFFF)), _mm_set1_epi32(0x3F800000)));
		__m128 gt = _mm_cmpgt_ps(m, _mm_set1_ps(1.41421356237f));
		m = _mm_blendv_ps(m, _mm_mul_ps(m, _mm_set1_ps(0.5f)), gt);
		e = _mm_add_ps(e, _mm_and_ps(gt, _mm_set1_ps(1.f)));
		__m128 f = _mm_sub_ps(m, _mm_set1_ps(1.f));
		__m128 z = _mm_mul_ps(f, f);
		__m128 p = _mm_set1_ps(7.0376836292E-2f);
		p = _mm_fmadd_ps(p, f, _mm_set1_ps(-1.1514610310E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps( 1.1676998740E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps(-1.2420140846E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps( 1.4249322787E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps(-1.6668057665E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps( 2.0000714765E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps(-2.4999993993E-1f));
		p = _mm_fmadd_ps(p, f, _mm_set1_ps( 3.3333331174E-1f));
		p = _mm_mul_ps(_mm_mul_ps(p, f), z);
		p = _mm_fmadd_ps(e, _mm_set1_ps(-2.12194440e-4f), p);
		p = _mm_fnmadd_ps(_mm_set1_ps(0.5f), z, p);
		__m128 res = _mm_add_ps(f, p);
		res = _mm_fmadd_ps(e, _mm_set1_ps(0.693359375f), res);
		__m128 const inf = _mm_castsi128_ps(_mm_set1_epi32(0x7F800000));
		res = _mm_blendv_ps(res, inf, _mm_cmpeq_ps(in, inf));
		res = _mm_blendv_ps(res, _mm_sub_ps(_mm_setzero_ps(), inf), _mm_cmpeq_ps(in, _mm_setzero_ps()));
		return _mm_or_ps(res, _mm_cmpnge_ps(in, _mm_setzero_ps())); // all-ones (a NaN) where !(x >= 0)
	}

	// e^x - 1, accurate near 0: (2^k - 1) + 2^k*(e^r - 1)
	GLM_FUNC_QUALIFIER __m128 glm_expm1_ps(__m128 x)
	{
		x = _mm_min_ps(_mm_set1_ps(88.3762626f), x);
		x = _mm_max_ps(_mm_set1_ps(-87.3365478f), x);
		__m128 k = _mm_round_ps(_mm_mul_ps(x, _mm_set1_ps(1.44269504088896341f)),
			_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
		__m128 r = glm_explog_fnmadd(k, _mm_set1_ps(0.693359375f), x);
		r = glm_explog_fnmadd(k, _mm_set1_ps(-2.12194440e-4f), r);
		__m128 p = _mm_set1_ps(1.f/720.f);
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/120.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/24.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/6.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f/2.f));
		p = _mm_fmadd_ps(p, r, _mm_set1_ps(1.f));
		__m128 er_m1 = _mm_mul_ps(r, p);   // e^r - 1
		__m128 twok = _mm_castsi128_ps(_mm_slli_epi32(_mm_add_epi32(_mm_cvtps_epi32(k), _mm_set1_epi32(127)), 23));
		return _mm_add_ps(_mm_sub_ps(twok, _mm_set1_ps(1.f)), _mm_mul_ps(twok, er_m1));
	}

	// ln(1+x), Kahan-corrected: ln(u) * x/(u-1), u = 1+x (falls back to x when u==1)
	GLM_FUNC_QUALIFIER __m128 glm_log1p_ps(__m128 x)
	{
		__m128 u = _mm_add_ps(_mm_set1_ps(1.f), x);
		__m128 um1 = _mm_sub_ps(u, _mm_set1_ps(1.f));
		__m128 r = _mm_mul_ps(glm_log_ps(u), _mm_div_ps(x, um1));
		return _mm_blendv_ps(r, x, _mm_cmpeq_ps(u, _mm_set1_ps(1.f)));
	}

}//namespace detail
}//namespace glm

#endif//(GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)
