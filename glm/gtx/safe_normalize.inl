/// @ref gtx_safe_normalize

#include <limits>

namespace glm
{
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> safeNormalize(vec<L, T, Q> const& x, vec<L, T, Q> const& fallback)
	{
		static_assert(std::numeric_limits<T>::is_iec559 || GLM_CONFIG_UNRESTRICTED_FLOAT, "'safeNormalize' accepts only floating-point inputs");

		return dot(x, x) > std::numeric_limits<T>::epsilon() ? normalize(x) : fallback;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> safeNormalize(vec<L, T, Q> const& x)
	{
		return safeNormalize(x, vec<L, T, Q>(static_cast<T>(0)));
	}
}//namespace glm
