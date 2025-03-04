#include "../simd/matrix.h"

#if GLM_ARCH & GLM_ARCH_SSE2_BIT

#include "type_mat4x4.hpp"
#include "../geometric.hpp"
#include <cstring>

namespace glm{
namespace detail
{
#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<qualifier Q>
	struct compute_matrixCompMult<4, 4, float, Q, true>
	{
		GLM_STATIC_ASSERT(detail::is_aligned<Q>::value, "Specialization requires aligned");

		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& x, mat<4, 4, float, Q> const& y)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_matrixCompMult(
			        &x[0].data,
			        &y[0].data,
			        &Result[0].data);
			return Result;
		}
	};
#	endif

	template<qualifier Q>
	struct compute_transpose<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_transpose(&m[0].data, &Result[0].data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_transpose<3, 3, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<3, 3, float, Q> call(mat<3, 3, float, Q> const& m)
		{
			mat<3, 3, float, Q> Result;
			glm_mat3_transpose(&m[0].data, &Result[0].data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_determinant<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(mat<4, 4, float, Q> const& m)
		{
			return _mm_cvtss_f32(glm_mat4_determinant(&m[0].data));
		}
	};

	template<qualifier Q>
	struct compute_inverse<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_inverse(&m[0].data, &Result[0].data);
			return Result;
		}
	};
}//namespace detail

#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_lowp> outerProduct<4, 4, float, aligned_lowp>(vec<4, float, aligned_lowp> const& c, vec<4, float, aligned_lowp> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_lowp> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_mediump> outerProduct<4, 4, float, aligned_mediump>(vec<4, float, aligned_mediump> const& c, vec<4, float, aligned_mediump> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_mediump> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_highp> outerProduct<4, 4, float, aligned_highp>(vec<4, float, aligned_highp> const& c, vec<4, float, aligned_highp> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_highp> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}
#	endif
}//namespace glm

#elif GLM_ARCH & GLM_ARCH_NEON_BIT

namespace glm {
#if GLM_LANG & GLM_LANG_CXX11_FLAG
	template <qualifier Q>
	GLM_FUNC_QUALIFIER
	typename std::enable_if<detail::is_aligned<Q>::value, mat<4, 4, float, Q>>::type
	operator*(mat<4, 4, float, Q> const & m1, mat<4, 4, float, Q> const & m2)
	{
		auto MulRow = [&](int l) {
			float32x4_t const SrcA = m2[l].data;

			float32x4_t r = neon::mul_lane(m1[0].data, SrcA, 0);
			r = neon::madd_lane(r, m1[1].data, SrcA, 1);
			r = neon::madd_lane(r, m1[2].data, SrcA, 2);
			r = neon::madd_lane(r, m1[3].data, SrcA, 3);

			return r;
		};

		mat<4, 4, float, aligned_highp> Result;
		Result[0].data = MulRow(0);
		Result[1].data = MulRow(1);
		Result[2].data = MulRow(2);
		Result[3].data = MulRow(3);

		return Result;
	}
#endif // CXX11

namespace detail
{
	template<qualifier Q>
	struct compute_inverse<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			float32x4_t const& m0 = m[0].data;
			float32x4_t const& m1 = m[1].data;
			float32x4_t const& m2 = m[2].data;
			float32x4_t const& m3 = m[3].data;

			// m[2][2] * m[3][3] - m[3][2] * m[2][3];
			// m[2][2] * m[3][3] - m[3][2] * m[2][3];
			// m[1][2] * m[3][3] - m[3][2] * m[1][3];
			// m[1][2] * m[2][3] - m[2][2] * m[1][3];

			float32x4_t Fac0;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac0 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			// m[2][1] * m[3][3] - m[3][1] * m[2][3];
			// m[2][1] * m[3][3] - m[3][1] * m[2][3];
			// m[1][1] * m[3][3] - m[3][1] * m[1][3];
			// m[1][1] * m[2][3] - m[2][1] * m[1][3];

			float32x4_t Fac1;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 1), neon::dup_lane(m1, 1));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 1), 3, m2, 1);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac1 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			// m[2][1] * m[3][2] - m[3][1] * m[2][2];
			// m[2][1] * m[3][2] - m[3][1] * m[2][2];
			// m[1][1] * m[3][2] - m[3][1] * m[1][2];
			// m[1][1] * m[2][2] - m[2][1] * m[1][2];

			float32x4_t Fac2;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 1), neon::dup_lane(m1, 1));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 1), 3, m2, 1);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				Fac2 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			// m[2][0] * m[3][3] - m[3][0] * m[2][3];
			// m[2][0] * m[3][3] - m[3][0] * m[2][3];
			// m[1][0] * m[3][3] - m[3][0] * m[1][3];
			// m[1][0] * m[2][3] - m[2][0] * m[1][3];

			float32x4_t Fac3;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac3 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			// m[2][0] * m[3][2] - m[3][0] * m[2][2];
			// m[2][0] * m[3][2] - m[3][0] * m[2][2];
			// m[1][0] * m[3][2] - m[3][0] * m[1][2];
			// m[1][0] * m[2][2] - m[2][0] * m[1][2];

			float32x4_t Fac4;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				Fac4 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			// m[2][0] * m[3][1] - m[3][0] * m[2][1];
			// m[2][0] * m[3][1] - m[3][0] * m[2][1];
			// m[1][0] * m[3][1] - m[3][0] * m[1][1];
			// m[1][0] * m[2][1] - m[2][0] * m[1][1];

			float32x4_t Fac5;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 1), 3, m2, 1);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 1), neon::dup_lane(m1, 1));
				Fac5 = vsubq_f32(vmulq_f32(w0, w1), vmulq_f32(w2, w3));
			}

			float32x4_t Vec0 = neon::copy_lane(neon::dupq_lane(m0, 0), 0, m1, 0); // (m[1][0], m[0][0], m[0][0], m[0][0]);
			float32x4_t Vec1 = neon::copy_lane(neon::dupq_lane(m0, 1), 0, m1, 1); // (m[1][1], m[0][1], m[0][1], m[0][1]);
			float32x4_t Vec2 = neon::copy_lane(neon::dupq_lane(m0, 2), 0, m1, 2); // (m[1][2], m[0][2], m[0][2], m[0][2]);
			float32x4_t Vec3 = neon::copy_lane(neon::dupq_lane(m0, 3), 0, m1, 3); // (m[1][3], m[0][3], m[0][3], m[0][3]);

			float32x4_t Inv0 = vaddq_f32(vsubq_f32(vmulq_f32(Vec1, Fac0), vmulq_f32(Vec2, Fac1)), vmulq_f32(Vec3, Fac2));
			float32x4_t Inv1 = vaddq_f32(vsubq_f32(vmulq_f32(Vec0, Fac0), vmulq_f32(Vec2, Fac3)), vmulq_f32(Vec3, Fac4));
			float32x4_t Inv2 = vaddq_f32(vsubq_f32(vmulq_f32(Vec0, Fac1), vmulq_f32(Vec1, Fac3)), vmulq_f32(Vec3, Fac5));
			float32x4_t Inv3 = vaddq_f32(vsubq_f32(vmulq_f32(Vec0, Fac2), vmulq_f32(Vec1, Fac4)), vmulq_f32(Vec2, Fac5));

			constexpr float mr0[4] = {-1.0f, +1.0f, -1.0f, +1.0f};
			constexpr float mr1[4] = {+1.0f, -1.0f, +1.0f, -1.0f};

			float32x4_t r0 = vmulq_f32(vld1q_f32(mr0), Inv0);
			float32x4_t r1 = vmulq_f32(vld1q_f32(mr1), Inv1);
			float32x4_t r2 = vmulq_f32(vld1q_f32(mr0), Inv2);
			float32x4_t r3 = vmulq_f32(vld1q_f32(mr1), Inv3);

			float32x4_t det = neon::mul_lane(r0, m0, 0);
			det = neon::madd_lane(det, r1, m0, 1);
			det = neon::madd_lane(det, r2, m0, 2);
			det = neon::madd_lane(det, r3, m0, 3);

			float32x4_t rdet = vdupq_n_f32(1 / vgetq_lane_f32(det, 0));

			mat<4, 4, float, Q> r;
			r[0].data = vmulq_f32(r0, rdet);
			r[1].data = vmulq_f32(r1, rdet);
			r[2].data = vmulq_f32(r2, rdet);
			r[3].data = vmulq_f32(r3, rdet);
			return r;
		}
	};
}//namespace detail
}//namespace glm

#elif GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm{
namespace detail
{
	template<qualifier Q, typename = typename std::enable_if<is_aligned<Q>::value>::type>
	GLM_FUNC_QUALIFIER mat<4, 4, float, Q> mat4_scale(vec<3, float, Q> const &scale)
	{
		mat<4, 4, float, Q> Result;
		Result[0].data = {scale.data.x, 0.0f, 0.0f, 0.0f};
		Result[1].data = {0.0f, scale.data.y, 0.0f, 0.0f};
		Result[2].data = {0.0f, 0.0f, scale.data.z, 0.0f};
		Result[3].data = {0.0f, 0.0f, 0.0f, 1.0f};
		return Result;
	}

	template<qualifier Q, typename = typename std::enable_if<is_aligned<Q>::value>::type>
	GLM_FUNC_QUALIFIER mat<4, 4, float, Q> mat4_translate(vec<3, float, Q> const &pos)
	{
		mat<4, 4, float, Q> Result;
		Result[0].data = {1.0f, 0.0f, 0.0f, 0.0f};
		Result[1].data = {0.0f, 1.0f, 0.0f, 0.0f};
		Result[2].data = {0.0f, 0.0f, 1.0f, 0.0f};
		Result[3].data = {pos.data.x, pos.data.y, pos.data.z, 1.0f};
		return Result;
	}

	template<qualifier Q>
	struct compute_inverse<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_inverse(&m[0].data, &Result[0].data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_transpose<3, 3, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<3, 3, float, Q> call(mat<3, 3, float, Q> const &m)
		{
			mat<3, 3, float, Q> Result;
			Result[0].data = { m[0].data.x, m[1].data.x, m[2].data.x };
			Result[1].data = { m[0].data.y, m[1].data.y, m[2].data.y };
			Result[2].data = { m[0].data.z, m[1].data.z, m[2].data.z };
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_transpose<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const &m)
		{
			mat<4, 4, float, Q> Result;
			Result[0].data = { m[0].data.x, m[1].data.x, m[2].data.x, m[3].data.x };
			Result[1].data = { m[0].data.y, m[1].data.y, m[2].data.y, m[3].data.y };
			Result[2].data = { m[0].data.z, m[1].data.z, m[2].data.z, m[3].data.z };
			Result[3].data = { m[0].data.w, m[1].data.w, m[2].data.w, m[3].data.w };
			return Result;
		}
	};

}//namespace detail
}//namespace glm

#endif
