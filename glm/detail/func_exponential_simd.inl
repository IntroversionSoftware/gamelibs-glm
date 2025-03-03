/// @ref core
/// @file glm/detail/func_exponential_simd.inl

#include "../simd/exponential.h"

#if defined(_M_ARM64) || defined(__aarch64__)
#include <arm_neon.h>
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
