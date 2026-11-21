/// @ref gtx_safe_normalize
/// @file glm/gtx/safe_normalize.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_safe_normalize GLM_GTX_safe_normalize
/// @ingroup gtx
///
/// Include <glm/gtx/safe_normalize.hpp> to use the features of this extension.
///
/// Normalization that gives a defined result for a zero-length vector.

#pragma once

// Dependency:
#include "../geometric.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_safe_normalize is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_safe_normalize extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_safe_normalize
	/// @{

	/// Returns x scaled to unit length, or a zero vector if x is too short to have a meaningful
	/// direction: its squared length is at most epsilon<T>(), where normalize would divide by
	/// (nearly) zero.
	/// From GLM_GTX_safe_normalize extension.
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> safeNormalize(vec<L, T, Q> const& x);

	/// Returns x scaled to unit length, or fallback if x is too short to have a meaningful
	/// direction: its squared length is at most epsilon<T>().
	/// From GLM_GTX_safe_normalize extension.
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL vec<L, T, Q> safeNormalize(vec<L, T, Q> const& x, vec<L, T, Q> const& fallback);

	/// @}
}//namespace glm

#include "safe_normalize.inl"
