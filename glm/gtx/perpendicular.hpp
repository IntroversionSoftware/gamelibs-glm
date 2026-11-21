/// @ref gtx_perpendicular
/// @file glm/gtx/perpendicular.hpp
///
/// @see core (dependence)
/// @see gtx_projection (dependence)
///
/// @defgroup gtx_perpendicular GLM_GTX_perpendicular
/// @ingroup gtx
///
/// Include <glm/gtx/perpendicular.hpp> to use the features of this extension.
///
/// Perpendicular of a vector from other one

#pragma once

// Dependency:
#include "../glm.hpp"
#include "../gtx/projection.hpp"
#include "../gtx/safe_normalize.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_perpendicular is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_perpendicular extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_perpendicular
	/// @{

	//! Projects x a perpendicular axis of Normal.
	//! From GLM_GTX_perpendicular extension.
	template<typename genType>
	GLM_FUNC_DECL genType perp(genType const& x, genType const& Normal);

	//! Returns a unit vector perpendicular to v, any one: for example to complete an orthonormal
	//! basis, or to stand in for normalize(cross(v, w)) when w is parallel to v. It follows v's
	//! direction continuously except where v crosses the z = 0 plane (Duff et al., "Building an
	//! Orthonormal Basis, Revisited", 2017). A zero v gives (1, 0, 0).
	//! From GLM_GTX_perpendicular extension.
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> anyPerpendicular(vec<3, T, Q> const& v);

	/// @}
}//namespace glm

#include "perpendicular.inl"
