/// @ref core
/// @file glm/detail/func_exponential_simd.inl

#include "../simd/exponential.h"

#if defined(_M_ARM64) || defined(__aarch64__)
#include <arm_neon.h>
#elif defined(_M_X64)
#include <intrin.h>
#elif defined(__x86_64__)
#include <x86intrin.h>
#endif

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm{
namespace detail
{
	template<qualifier Q>
	struct compute_sqrt<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& v)
		{
			vec<4, float, Q> Result;
			Result.data = _mm_sqrt_ps(v.data);
			return Result;
		}
	};

#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<>
	struct compute_sqrt<4, float, aligned_lowp, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, aligned_lowp> call(vec<4, float, aligned_lowp> const& v)
		{
			vec<4, float, aligned_lowp> Result;
			Result.data = glm_vec4_sqrt_lowp(v.data);
			return Result;
		}
	};
#	endif
}//namespace detail
}//namespace glm

#endif//GLM_ARCH & GLM_ARCH_SSE2_BIT

#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm{
namespace detail
{
	template<qualifier Q>
	struct compute_inversesqrt_scalar<float, Q>
	{
		GLM_FUNC_QUALIFIER static float call(float _v)
		{
			float result;

#if defined(_M_X64) || defined(__x86_64__)
			// Load the scalar value into an SSE register
			const __m128 v = _mm_set_ss(_v);

			// Get initial estimate of 1/sqrt(x) using the RSQRTSS instruction
			__m128 current = _mm_rsqrt_ss(v);

			if constexpr (Q != lowp) {
				// Constants for Newton-Raphson
				const __m128 half = _mm_set_ss(0.5f);
				const __m128 three_halves = _mm_set_ss(1.5f);

				// First Newton-Raphson iteration for refinement
				// y₁ = y₀ * (1.5 - 0.5 * x * y₀²)
				__m128 current_squared = _mm_mul_ss(current, current);
				__m128 v_times_est_squared = _mm_mul_ss(v, current_squared);
				__m128 half_v_est_squared = _mm_mul_ss(half, v_times_est_squared);
				__m128 step = _mm_sub_ss(three_halves, half_v_est_squared);
				current = _mm_mul_ss(current, step);

				if constexpr (Q == highp) {
					// Second Newton-Raphson iteration for further refinement
					// y₂ = y₁ * (1.5 - 0.5 * x * y₁²)
					current_squared = _mm_mul_ss(current, current);
					v_times_est_squared = _mm_mul_ss(v, current_squared);
					half_v_est_squared = _mm_mul_ss(half, v_times_est_squared);
					step = _mm_sub_ss(three_halves, half_v_est_squared);
					current = _mm_mul_ss(current, step);
				}
			}

			// Extract the result
			_mm_store_ss(&result, current);

#elif defined(_M_ARM64) || defined(__aarch64__)
			// Load the scalar value into a NEON register
			const float32x2_t v = vdup_n_f32(_v);

			// Get initial estimate of 1/sqrt(x)
			float32x2_t current = vrsqrte_f32(v);

			if constexpr (Q != lowp) {
				// First Newton-Raphson iteration using vrsqrts_f32
				// This intrinsic computes (2 - v * estimate^2) / 2, which is the step factor
				float32x2_t step = vrsqrts_f32(v, vmul_f32(current, current));
				current = vmul_f32(current, step);

				if constexpr (Q == highp) {
					// Second Newton-Raphson iteration for further refinement
					step = vrsqrts_f32(v, vmul_f32(current, current));
					current = vmul_f32(current, step);
				}
			}

			// Extract the result (first lane)
			result = vget_lane_f32(current, 0);

#else
			// Standard library fallback
			result = 1.0f / std::sqrt(_v);
#endif

			return result;
		}
	};

#if GLM_HAS_BUILTIN(__builtin_elementwise_sqrt)
	template<qualifier Q>
	struct compute_sqrt<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& v)
		{
			vec<4, float, Q> Result;
			Result.data = __builtin_elementwise_sqrt(v.data);
			return Result;
		}
	};
#endif
}//namespace detail
}//namespace glm

#endif//GLM_ARCH & GLM_ARCH_CLANG_BIT

// --- Vectorized exp/log/exp2/log2 for aligned float vectors (mediump/lowp) ---
// highp defers to libm; mediump/lowp use 4-wide SIMD (rel err ~1-2e-7).
#if ((GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)) && defined(__FMA__)  // SIMD path needs FMA; else libm/scalar primaries

#include <cmath>

namespace glm{
namespace detail
{
	GLM_FUNC_QUALIFIER __m128 glm_explog_fnmadd(__m128 a, __m128 b, __m128 c)
	{
#	if defined(__FMA__)
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

	// ln(x) = e*ln2 + ln(m), m in [sqrt(2)/2, sqrt(2)); Cephes minimax (~1 ULP)
	GLM_FUNC_QUALIFIER __m128 glm_log_ps(__m128 x)
	{
		__m128i xi = _mm_castps_si128(x);
		__m128 e = _mm_cvtepi32_ps(_mm_sub_epi32(_mm_srli_epi32(xi, 23), _mm_set1_epi32(127)));
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
		return _mm_fmadd_ps(e, _mm_set1_ps(0.693359375f), res);
	}

	template<length_t L, qualifier Q>
	struct compute_exp<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& x)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::exp(x[i]);
			} else {
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data),
					glm_exp_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&x.data))));
			}
			return Result;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_log<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& x)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::log(x[i]);
			} else {
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data),
					glm_log_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&x.data))));
			}
			return Result;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_exp2<L, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& x)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::exp2(x[i]);
			} else {
				__m128 in = _mm_mul_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&x.data)),
					_mm_set1_ps(0.6931471805599453f)); // exp2(x) = exp(x*ln2)
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data), glm_exp_ps(in));
			}
			return Result;
		}
	};

	template<length_t L, qualifier Q>
	struct compute_log2<L, float, Q, true, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, float, Q> call(vec<L, float, Q> const& x)
		{
			vec<L, float, Q> Result;
			if constexpr (is_highp<Q>::value || L < 3) {
				for (length_t i = 0; i < L; ++i) Result[i] = std::log2(x[i]);
			} else {
				__m128 l = glm_log_ps(_mm_loadu_ps(reinterpret_cast<const float*>(&x.data)));
				_mm_storeu_ps(reinterpret_cast<float*>(&Result.data),
					_mm_mul_ps(l, _mm_set1_ps(1.44269504088896341f))); // log2(x) = ln(x)*log2(e)
			}
			return Result;
		}
	};

}//namespace detail
}//namespace glm

#endif//(GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT)
