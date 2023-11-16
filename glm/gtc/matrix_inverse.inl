/// @ref gtc_matrix_inverse

namespace glm
{
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> affineInverse(mat<3, 3, T, Q> const& m)
	{
		mat<2, 2, T, Q> const Inv(inverse(mat<2, 2, T, Q>(m)));

		return mat<3, 3, T, Q>(
			vec<3, T, Q>(Inv[0], static_cast<T>(0)),
			vec<3, T, Q>(Inv[1], static_cast<T>(0)),
			vec<3, T, Q>(-Inv * vec<2, T, Q>(m[2]), static_cast<T>(1)));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> affineInverse(mat<4, 4, T, Q> const& m)
	{
		mat<3, 3, T, Q> const Inv(inverse(mat<3, 3, T, Q>(m)));

		return mat<4, 4, T, Q>(
			vec<4, T, Q>(Inv[0], static_cast<T>(0)),
			vec<4, T, Q>(Inv[1], static_cast<T>(0)),
			vec<4, T, Q>(Inv[2], static_cast<T>(0)),
			vec<4, T, Q>(-Inv * vec<3, T, Q>(m[3]), static_cast<T>(1)));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<2, 2, T, Q> inverseTranspose(mat<2, 2, T, Q> const& m)
	{
		T Determinant = m[0][0] * m[1][1] - m[1][0] * m[0][1];

		mat<2, 2, T, Q> Inverse(
			+ m[1][1] / Determinant,
			- m[0][1] / Determinant,
			- m[1][0] / Determinant,
			+ m[0][0] / Determinant);

		return Inverse;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<3, 3, T, Q> inverseTranspose(mat<3, 3, T, Q> const& m)
	{
		return transpose(inverse(m));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER mat<4, 4, T, Q> inverseTranspose(mat<4, 4, T, Q> const& m)
	{
		return transpose(inverse(m));
	}
}//namespace glm
