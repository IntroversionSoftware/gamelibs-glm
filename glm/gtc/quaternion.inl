#include "../trigonometric.hpp"
#include "../geometric.hpp"
#include "../exponential.hpp"
#include "epsilon.hpp"
#include <limits>

namespace glm
{
	namespace detail
	{
		template<typename MatrixType, typename T, qualifier Q>
		GLM_FUNC_QUALIFIER qua<T, Q> quat_cast_impl(MatrixType const& m)
		{
			// Compute diagonal elements once
			T m00 = m[0][0];
			T m11 = m[1][1];
			T m22 = m[2][2];

			// Compute the four squares directly
			T fourWSquaredMinus1 = m00 + m11 + m22;
			T fourXSquaredMinus1 = m00 - m11 - m22;
			T fourYSquaredMinus1 = m11 - m00 - m22;
			T fourZSquaredMinus1 = m22 - m00 - m11;

			int biggestIndex = 0;
			T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
			if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourXSquaredMinus1;
				biggestIndex = 1;
			}
			if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourYSquaredMinus1;
				biggestIndex = 2;
			}
			if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
			{
				fourBiggestSquaredMinus1 = fourZSquaredMinus1;
				biggestIndex = 3;
			}

			// Calculate these only once
			T biggestVal = sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0.5);
			T mult = T(0.25) / biggestVal;

			// Pre-compute differences and sums that appear in multiple cases
			T yz_zy = m[1][2] - m[2][1];
			T zx_xz = m[2][0] - m[0][2];
			T xy_yx = m[0][1] - m[1][0];

			// We only need these if biggestIndex != 0
			T xy_plus_yx, zx_plus_xz, yz_plus_zy;
			if (biggestIndex != 0) {
				xy_plus_yx = m[0][1] + m[1][0];
				zx_plus_xz = m[2][0] + m[0][2];
				yz_plus_zy = m[1][2] + m[2][1];
			}

			switch(biggestIndex)
			{
			case 0:
				return qua<T, Q>::wxyz(
					biggestVal,
					yz_zy * mult,
					zx_xz * mult,
					xy_yx * mult
				);
			case 1:
				return qua<T, Q>::wxyz(
					yz_zy * mult,
					biggestVal,
					xy_plus_yx * mult,
					zx_plus_xz * mult
				);
			case 2:
				return qua<T, Q>::wxyz(
					zx_xz * mult,
					xy_plus_yx * mult,
					biggestVal,
					yz_plus_zy * mult
				);
			case 3:
				return qua<T, Q>::wxyz(
					xy_yx * mult,
					zx_plus_xz * mult,
					yz_plus_zy * mult,
					biggestVal
				);
			}

			// Unreachable, but avoids compiler warnings
			// Will be optimized out completely
			return qua<T, Q>::wxyz(T(1), T(0), T(0), T(0));
		}
	}//namespace detail

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> eulerAngles(qua<T, Q> const& x)
	{
		return vec<3, T, Q>(pitch(x), yaw(x), roll(x));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T roll(qua<T, Q> const& q)
	{
		T const y = static_cast<T>(2) * (q.x * q.y + q.w * q.z);
		T const x = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;

		if(all(equal(vec<2, T, Q>(x, y), vec<2, T, Q>(0), epsilon<T>()))) //avoid atan2(0,0) - handle singularity - Matiis
			return static_cast<T>(0);

		return static_cast<T>(atan(y, x));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T pitch(qua<T, Q> const& q)
	{
		//return T(atan(T(2) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z));
		T const y = static_cast<T>(2) * (q.y * q.z + q.w * q.x);
		T const x = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;

		if(all(equal(vec<2, T, Q>(x, y), vec<2, T, Q>(0), epsilon<T>()))) //avoid atan2(0,0) - handle singularity - Matiis
			return static_cast<T>(static_cast<T>(2) * atan(q.x, q.w));

		return static_cast<T>(atan(y, x));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER T yaw(qua<T, Q> const& q)
	{
		return asin(clamp(static_cast<T>(-2) * (q.x * q.z - q.w * q.y), static_cast<T>(-1), static_cast<T>(1)));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> mat3_cast(qua<T, Q> const& q)
	{
		mat<3, 3, T, Q> Result(T(1));
		T qxx(q.x * q.x);
		T qyy(q.y * q.y);
		T qzz(q.z * q.z);
		T qxz(q.x * q.z);
		T qxy(q.x * q.y);
		T qyz(q.y * q.z);
		T qwx(q.w * q.x);
		T qwy(q.w * q.y);
		T qwz(q.w * q.z);

		Result[0][0] = T(1) - T(2) * (qyy +  qzz);
		Result[0][1] = T(2) * (qxy + qwz);
		Result[0][2] = T(2) * (qxz - qwy);

		Result[1][0] = T(2) * (qxy - qwz);
		Result[1][1] = T(1) - T(2) * (qxx +  qzz);
		Result[1][2] = T(2) * (qyz + qwx);

		Result[2][0] = T(2) * (qxz + qwy);
		Result[2][1] = T(2) * (qyz - qwx);
		Result[2][2] = T(1) - T(2) * (qxx +  qyy);
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> mat4_cast(qua<T, Q> const& q)
	{
		return mat<4, 4, T, Q>(mat3_cast(q));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> quat_cast(mat<3, 3, T, Q> const& m)
	{
		return detail::quat_cast_impl<mat<3, 3, T, Q>, T, Q>(m);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> quat_cast(mat<4, 4, T, Q> const& m)
	{
		return detail::quat_cast_impl<mat<4, 4, T, Q>, T, Q>(m);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> lessThan(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result(false, false, false, false);
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] < y[i];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> lessThanEqual(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result(false, false, false, false);
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] <= y[i];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> greaterThan(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result(false, false, false, false);
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] > y[i];
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<4, bool, Q> greaterThanEqual(qua<T, Q> const& x, qua<T, Q> const& y)
	{
		vec<4, bool, Q> Result(false, false, false, false);
		for(length_t i = 0; i < x.length(); ++i)
			Result[i] = x[i] >= y[i];
		return Result;
	}


	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> quatLookAt(vec<3, T, Q> const& direction, vec<3, T, Q> const& up)
	{
#		if GLM_CONFIG_CLIP_CONTROL & GLM_CLIP_CONTROL_LH_BIT
			return quatLookAtLH(direction, up);
#		else
			return quatLookAtRH(direction, up);
# 		endif
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> quatLookAtRH(vec<3, T, Q> const& direction, vec<3, T, Q> const& up)
	{
		mat<3, 3, T, Q> Result;

		Result[2] = -direction;
		vec<3, T, Q> const& Right = cross(up, Result[2]);
		Result[0] = Right * inversesqrt(max(static_cast<T>(0.00001), dot(Right, Right)));
		Result[1] = cross(Result[2], Result[0]);

		return quat_cast(Result);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER qua<T, Q> quatLookAtLH(vec<3, T, Q> const& direction, vec<3, T, Q> const& up)
	{
		mat<3, 3, T, Q> Result;

		Result[2] = direction;
		vec<3, T, Q> const& Right = cross(up, Result[2]);
		Result[0] = Right * inversesqrt(max(static_cast<T>(0.00001), dot(Right, Right)));
		Result[1] = cross(Result[2], Result[0]);

		return quat_cast(Result);
	}
}//namespace glm

#if GLM_CONFIG_SIMD == GLM_ENABLE
#	include "quaternion_simd.inl"
#endif

