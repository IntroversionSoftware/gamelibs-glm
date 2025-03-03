/// @ref core
/// @file glm/detail/func_exponential_simd.inl

#include "../simd/exponential.h"

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
#if defined(_M_X64) || defined(__x86_64__)
		// Load the scalar value into an SSE register
		const __m128 v = _mm_set_ss(_v);

		// Get initial estimate of 1/sqrt(x) using the RSQRTSS instruction
		// This provides a 12-bit approximation
		const __m128 estimate = _mm_rsqrt_ss(v);

		// Depending on our precision requirements, either do a single
		// Newton-Raphson iteration, or just return the estimate as-is.
		if constexpr (Q != lowp) {
			// One Newton-Raphson iteration for refinement
			// Formula: estimate = estimate * (1.5 - 0.5 * v * estimate * estimate)
			const __m128 half = _mm_set_ss(0.5f);
			const __m128 three_halves = _mm_set_ss(1.5f);

			const __m128 estimate_squared = _mm_mul_ss(estimate, estimate); // estimate²
			const __m128 v_times_est_squared = _mm_mul_ss(v, estimate_squared); // v * estimate²
			const __m128 half_v_est_squared = _mm_mul_ss(half, v_times_est_squared); // 0.5 * v * estimate²
			const __m128 step = _mm_sub_ss(three_halves, half_v_est_squared); // 1.5 - 0.5 * v * estimate²
			const __m128 refined = _mm_mul_ss(estimate, step); // estimate * (1.5 - 0.5 * v * estimate²)

			// Extract the result
			float result;
			_mm_store_ss(&result, refined);
			return result;
		} else {
			// Extract the result
			float result;
			_mm_store_ss(&result, estimate);
			return result;
		}
#elif defined(_M_ARM64) || defined(__aarch64__)
		// Load the scalar value into a NEON register
		const float32x2_t v = vdup_n_f32(_v);

		// Get initial estimate of 1/sqrt(x)
		const float32x2_t estimate = vrsqrte_f32(v);

		if constexpr (Q != lowp) {
			// One Newton-Raphson iteration for refinement using vrsqrts_f32
			// This intrinsic computes (2 - v * estimate^2) / 2, which is the step factor
			const float32x2_t step = vrsqrts_f32(v, vmul_f32(estimate, estimate));

			// Apply the step: estimate = estimate * step
			const float32x2_t refined = vmul_f32(estimate, step);

			// Extract the result (first lane)
			return vget_lane_f32(refined, 0);
		} else {
			// Extract the result (first lane)
			return vget_lane_f32(estimate, 0);
		}
#else
		return 1.0f / std::sqrt(_v);
#endif
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
