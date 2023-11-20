/// @ref core

#if GLM_ARCH & GLM_ARCH_CLANG_BIT
namespace glm
{
	template <>
	GLM_FUNC_QUALIFIER typename mat<3, 3, float, defaultp>::col_type operator*(
		mat<3, 3, float, defaultp> const &m,
		typename mat<3, 3, float, defaultp>::row_type const &v)
	{
		typename mat<3, 3, float, defaultp>::col_type result;
		result.data = v.data.xxx * m[0].data +
		              v.data.yyy * m[1].data +
		              v.data.zzz * m[2].data;
		return result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<3, 3, float, defaultp> operator*(
		mat<3, 3, float, defaultp> const &m2,
		mat<3, 3, float, defaultp> const &m1)
	{
		glm_f32vec3 x_axis, y_axis, z_axis;

		x_axis =          m1[0].data.xxx * m2[0].data;
		x_axis = x_axis + m1[0].data.yyy * m2[1].data;
		x_axis = x_axis + m1[0].data.zzz * m2[2].data;

		y_axis =          m1[1].data.xxx * m2[0].data;
		y_axis = y_axis + m1[1].data.yyy * m2[1].data;
		y_axis = y_axis + m1[1].data.zzz * m2[2].data;

		z_axis =          m1[2].data.xxx * m2[0].data;
		z_axis = z_axis + m1[2].data.yyy * m2[1].data;
		z_axis = z_axis + m1[2].data.zzz * m2[2].data;

		mat<3, 3, float, defaultp> Result;
		Result[0].data = x_axis;
		Result[1].data = y_axis;
		Result[2].data = z_axis;
		return Result;
	}
}//namespace glm
#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
