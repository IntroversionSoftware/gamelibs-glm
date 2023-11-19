/// @ref core
/// @file glm/detail/func_geometric_simd.inl

#include "../simd/geometric.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm
{
namespace detail
{
	template<qualifier Q>
	struct compute_length<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& v)
		{
			return _mm_cvtss_f32(glm_vec4_length(v.data));
		}
	};

	template<qualifier Q>
	struct compute_distance<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& p0, vec<4, float, Q> const& p1)
		{
			return _mm_cvtss_f32(glm_vec4_distance(p0.data, p1.data));
		}
	};

	template<qualifier Q>
	struct compute_dot<vec<4, float, Q>, float, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& x, vec<4, float, Q> const& y)
		{
			return _mm_cvtss_f32(glm_vec1_dot(x.data, y.data));
		}
	};

	template<qualifier Q>
	struct compute_cross<float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<3, float, Q> call(vec<3, float, Q> const& a, vec<3, float, Q> const& b)
		{
			__m128 const set0 = _mm_set_ps(0.0f, a.z, a.y, a.x);
			__m128 const set1 = _mm_set_ps(0.0f, b.z, b.y, b.x);
			__m128 const xpd0 = glm_vec4_cross(set0, set1);

			vec<4, float, Q> Result;
			Result.data = xpd0;
			return vec<3, float, Q>(Result);
		}
	};

	template<qualifier Q>
	struct compute_normalize<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& v)
		{
			vec<4, float, Q> Result;
			Result.data = glm_vec4_normalize(v.data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_faceforward<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& N, vec<4, float, Q> const& I, vec<4, float, Q> const& Nref)
		{
			vec<4, float, Q> Result;
			Result.data = glm_vec4_faceforward(N.data, I.data, Nref.data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_reflect<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& I, vec<4, float, Q> const& N)
		{
			vec<4, float, Q> Result;
			Result.data = glm_vec4_reflect(I.data, N.data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_refract<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& I, vec<4, float, Q> const& N, float eta)
		{
			vec<4, float, Q> Result;
			Result.data = glm_vec4_refract(I.data, N.data, _mm_set1_ps(eta));
			return Result;
		}
	};
}//namespace detail
}//namespace glm

#elif GLM_ARCH & GLM_ARCH_NEON_BIT

namespace glm
{
namespace detail
{
	template<qualifier Q>
	struct compute_length<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& v)
		{
			return sqrt(compute_dot<vec<4, float, Q>, float, true>::call(v, v));
		}
	};

	template<qualifier Q>
	struct compute_distance<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& p0, vec<4, float, Q> const& p1)
		{
			return compute_length<4, float, Q, true>::call(p1 - p0);
		}
	};


	template<qualifier Q>
	struct compute_dot<vec<4, float, Q>, float, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& x, vec<4, float, Q> const& y)
		{
#if GLM_ARCH & GLM_ARCH_ARMV8_BIT
			float32x4_t v = vmulq_f32(x.data, y.data);
			return vaddvq_f32(v);
#else  // Armv7a with Neon
			float32x4_t p = vmulq_f32(x.data, y.data);
			float32x2_t v = vpadd_f32(vget_low_f32(p), vget_high_f32(p));
			v = vpadd_f32(v, v);
			return vget_lane_f32(v, 0);
#endif
		}
	};

	template<qualifier Q>
	struct compute_normalize<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& v)
		{
			float32x4_t p = vmulq_f32(v.data, v.data);
#if GLM_ARCH & GLM_ARCH_ARMV8_BIT
			p = vpaddq_f32(p, p);
			p = vpaddq_f32(p, p);
#else
			float32x2_t t = vpadd_f32(vget_low_f32(p), vget_high_f32(p));
			t = vpadd_f32(t, t);
			p = vcombine_f32(t, t);
#endif

			float32x4_t vd = vrsqrteq_f32(p);
			vec<4, float, Q> Result;
			Result.data = vmulq_f32(v.data, vd);
			return Result;
		}
	};
}//namespace detail
}//namespace glm

#elif GLM_ARCH & GLM_ARCH_CLANG_BIT

#if defined(__SSE4_1__)
#include <immintrin.h>
#endif

namespace glm
{
namespace detail
{
	template<qualifier Q>
	struct compute_length<3, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<3, float, Q> const &v)
		{
			return sqrt(compute_dot<vec<3, float, Q>, float, true>::call(v, v));
		}
	};

	template<qualifier Q>
	struct compute_length<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& v)
		{
			return sqrt(compute_dot<vec<4, float, Q>, float, true>::call(v, v));
		}
	};

	template<qualifier Q>
	struct compute_distance<3, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<3, float, Q> const &p0, vec<3, float, Q> const &p1)
		{
			return compute_length<3, float, Q, true>::call(p1 - p0);
		}
	};

	template<qualifier Q>
	struct compute_distance<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& p0, vec<4, float, Q> const& p1)
		{
			return compute_length<4, float, Q, true>::call(p1 - p0);
		}
	};

	template<qualifier Q>
	struct compute_cross<float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<3, float, Q> call(vec<3, float, Q> const &a, vec<3, float, Q> const &b)
		{
			const glm_f32vec3 lhs_yzx = a.data.yzx;
			const glm_f32vec3 rhs_yzx = b.data.yzx;
			const glm_f32vec3 rv = (a.data.xyz * rhs_yzx) - (lhs_yzx * b.data.xyz);

			vec<3, float, Q> Result;
			Result.data = rv.yzx;
			return vec<3, float, Q>(Result);
		}
	};

	template<qualifier Q>
	struct compute_dot<vec<3, float, Q>, float, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<3, float, Q> const &x, vec<3, float, Q> const &y)
		{
#if defined(__SSE4_1__)
			// Clang doesn't notice this is a dot product, so we have to use
			// SSE4.1 intrinsics here.
			const __m128 xv = _mm_loadu_ps((const float *)&x.data);
			const __m128 yv = _mm_loadu_ps((const float *)&y.data);
			const __m128 dp = _mm_dp_ps(xv, yv, 0x7F);
			return _mm_cvtss_f32(dp);
#else
			const glm_f32vec3 prod = x.data * y.data;
			return prod.x + prod.y + prod.z;
#endif
		}
	};

	template<qualifier Q>
	struct compute_dot<vec<4, float, Q>, float, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<4, float, Q> const& x, vec<4, float, Q> const& y)
		{
#if defined(__SSE4_1__)
			// Clang doesn't notice this is a dot product, so we have to use
			// SSE4.1 intrinsics here.
			const __m128 xv = _mm_loadu_ps((const float *)&x.data);
			const __m128 yv = _mm_loadu_ps((const float *)&y.data);
			const __m128 dp = _mm_dp_ps(xv, yv, 0xFF);
			return _mm_cvtss_f32(dp);
#else
			const glm_f32vec4 prod = x.data * y.data;
			const glm_f32vec2 hsum = prod.xz + prod.yw;
			return hsum.x + hsum.y;
#endif
		}
	};

	template<qualifier Q>
	struct compute_normalize<3, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<3, float, Q> call(vec<3, float, Q> const& v)
		{
			const float invLen = 1.0f / compute_length<3, float, Q, true>::call(v);
			return vec<3, float, Q>(v) *= invLen;
		}
	};

	template<qualifier Q>
	struct compute_normalize<4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const &v)
		{
			const float invLen = 1.0f / compute_length<4, float, Q, true>::call(v);
			return vec<4, float, Q>(v) *= invLen;
		}
	};
}//namespace detail
}//namespace glm

#endif
