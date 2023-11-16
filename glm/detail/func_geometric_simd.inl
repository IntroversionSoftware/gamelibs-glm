/// @ref core
/// @file glm/detail/func_geometric_simd.inl

#include "../simd/geometric.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

namespace glm{
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
	struct compute_dot<vec<3, float, Q>, float, true>
	{
		GLM_FUNC_QUALIFIER static float call(vec<3, float, Q> const& a, vec<3, float, Q> const& b)
		{
			vec<4, float, Q> aa = xyz0(a);
			vec<4, float, Q> bb = xyz0(b);
			return _mm_cvtss_f32(glm_vec1_dot(aa.data, bb.data));
		}
	};

	template<qualifier Q>
	struct compute_cross<float, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<3, float, Q> call(vec<3, float, Q> const& a, vec<3, float, Q> const& b)
		{
			vec<4, float, Q> aa = xyzz(a);
			vec<4, float, Q> bb = xyzz(b);
			__m128 const xpd0 = glm_vec4_cross(aa.data, bb.data);

			vec<3, float, Q> Result;
			Result.data = xpd0;
			return Result;
		}

		GLM_FUNC_QUALIFIER static vec<4, float, Q> call(vec<4, float, Q> const& a, vec<4, float, Q> const& b)
		{
			vec<4, float, Q> Result;
			Result.data = glm_vec4_cross(a.data, b.data);
			return Result;
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
namespace glm{
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

#endif//GLM_ARCH & GLM_ARCH_SSE2_BIT

#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm{
namespace detail
{
	template<length_t L, typename T, qualifier Q>
	struct compute_length<L, T, Q, true>
	{
		GLM_FUNC_QUALIFIER static T call(vec<L, T, Q> const &v)
		{
			return sqrt(compute_dot<vec<L, T, Q>, T, true>::call(v, v));
		}
	};

	template<length_t L, typename T, qualifier Q>
	struct compute_distance<L, T, Q, true>
	{
		GLM_FUNC_QUALIFIER static T call(vec<L, T, Q> const &p0, vec<L, T, Q> const &p1)
		{
			return compute_length<L, T, Q, true>::call(p1 - p0);
		}
	};

	template<typename T, qualifier Q>
	struct compute_cross<T, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<3, T, Q> call(vec<3, T, Q> const &a, vec<3, T, Q> const &b)
		{
			using storage_type = typename detail::storage<3, T, true>::type;
			const storage_type lhs_yzx = a.data.yzx;
			const storage_type rhs_yzx = b.data.yzx;
			const storage_type rv = (a.data.xyz * rhs_yzx) - (lhs_yzx * b.data.xyz);
			vec<3, T, Q> Result;
			Result.data = rv.yzx;
			return vec<3, T, Q>(Result);
		}

		GLM_FUNC_QUALIFIER static vec<4, T, Q> call(vec<4, T, Q> const& a, vec<4, T, Q> const& b)
		{
			using storage_type = typename detail::storage<4, T, true>::type;
			const storage_type v1 = a.data;
			const storage_type v2 = b.data;
			const storage_type swp0 = v1.yzxw;
			const storage_type swp1 = v1.zxyw;
			const storage_type swp2 = v2.yzxw;
			const storage_type swp3 = v2.zxyw;
			const storage_type mul0 = swp0 * swp3;
			const storage_type mul1 = swp1 * swp2;
			const storage_type sub0 = mul0 - mul1;
			vec<4, T, Q> Result;
			Result.data = sub0;
			return Result;
		}
	};

	template<typename T, qualifier Q>
	struct compute_dot<vec<3, T, Q>, T, true>
	{
		GLM_FUNC_QUALIFIER static T call(vec<3, T, Q> const &x, vec<3, T, Q> const &y)
		{
			using storage_type = typename detail::storage<3, T, true>::type;
			const storage_type prod = x.data * y.data;
			return prod.x + prod.y + prod.z;
		}
	};

	template<typename T, qualifier Q>
	struct compute_dot<vec<4, T, Q>, T, true>
	{
		GLM_FUNC_QUALIFIER static T call(vec<4, T, Q> const& x, vec<4, T, Q> const& y)
		{
			using storage_type = typename detail::storage<4, T, true>::type;
			const storage_type prod = x.data * y.data;
			return prod.x + prod.y + prod.z + prod.w;
		}
	};

	template<length_t L, typename T, qualifier Q>
	struct compute_normalize<L, T, Q, true>
	{
		GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& v)
		{
			const T lenSq = compute_dot<vec<L, T, Q>, T, true>::call(v, v);
			vec<L, T, Q> Result;
			Result.data = v.data * compute_inversesqrt_scalar<T, lowp>::call(lenSq);
			return Result;
		}
	};

	template<typename T, qualifier Q>
	struct compute_quat_normalize<T, Q, true>
	{
		GLM_FUNC_QUALIFIER static qua<T, Q> call(qua<T, Q> const& q, T lenSq)
		{
			qua<T, Q> Result;
			Result.data = q.data * compute_inversesqrt_scalar<T, mediump>::call(lenSq);
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

}//namespace detail
}//namespace glm
#endif//GLM_ARCH & GLM_ARCH_CLANG_BIT
