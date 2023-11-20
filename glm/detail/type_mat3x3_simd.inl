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
		result.data = v.data.xxx * m[0].data + v.data.yyy * m[1].data + v.data.zzz * m[2].data;
		return result;
	}

}//namespace glm
#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
