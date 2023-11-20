/// @ref core

#if GLM_ARCH & GLM_ARCH_CLANG_BIT
namespace glm
{
	template <>
	GLM_FUNC_QUALIFIER typename mat<4, 4, float, defaultp>::col_type operator*(
		mat<4, 4, float, defaultp> const &m,
		typename mat<4, 4, float, defaultp>::row_type const &v)
	{
		typename mat<4, 4, float, defaultp>::col_type result;
		result.data = v.data.xxxx * m[0].data +
		              v.data.yyyy * m[1].data +
		              v.data.zzzz * m[2].data +
		              v.data.wwww * m[3].data;
		return result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, defaultp> operator*(
		mat<4, 4, float, defaultp> const &m2,
		mat<4, 4, float, defaultp> const &m1)
	{
		glm_f32vec4 x_axis, y_axis, z_axis, w_axis;

		x_axis =          m1[0].data.xxxx * m2[0].data;
		x_axis = x_axis + m1[0].data.yyyy * m2[1].data;
		x_axis = x_axis + m1[0].data.zzzz * m2[2].data;

		y_axis =          m1[1].data.xxxx * m2[0].data;
		y_axis = y_axis + m1[1].data.yyyy * m2[1].data;
		y_axis = y_axis + m1[1].data.zzzz * m2[2].data;

		z_axis =          m1[2].data.xxxx * m2[0].data;
		z_axis = z_axis + m1[2].data.yyyy * m2[1].data;
		z_axis = z_axis + m1[2].data.zzzz * m2[2].data;

		w_axis =          m1[3].data.xxxx * m2[0].data;
		w_axis = w_axis + m1[3].data.yyyy * m2[1].data;
		w_axis = w_axis + m1[3].data.zzzz * m2[2].data;
		w_axis = w_axis + m2[3].data;

		mat<4, 4, float, defaultp> Result;
		Result[0].data = x_axis;
		Result[1].data = y_axis;
		Result[2].data = z_axis;
		Result[3].data = w_axis;
		return Result;
	}
}//namespace glm
#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
