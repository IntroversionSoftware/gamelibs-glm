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
		result.data = v.data.xxxx * m[0].data + v.data.yyyy * m[1].data + v.data.zzzz * m[2].data + v.data.wwww * m[3].data;
		return result;
	}

}//namespace glm
#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
