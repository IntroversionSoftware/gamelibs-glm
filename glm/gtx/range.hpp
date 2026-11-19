/// @ref gtx_range
/// @file glm/gtx/range.hpp
/// @author Joshua Moerman
///
/// @defgroup gtx_range GLM_GTX_range
/// @ingroup gtx
///
/// Include <glm/gtx/range.hpp> to use the features of this extension.
///
/// Defines begin and end for vectors and matrices. Useful for range-based for loop.
/// The range is defined over the elements, not over columns or rows (e.g. mat4 has 16 elements).

#pragma once

// Dependencies
#include "../detail/setup.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_range is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_range extension included")
#endif

#include "../gtc/type_ptr.hpp"
#include "../gtc/vec1.hpp"

namespace glm
{
	namespace detail
	{
		// Flat iteration over a matrix needs its columns to be adjacent. Aligned 3-row matrices pad each
		// column to 4 components, so a T* range over them would visit padding and miss the last column.
		template<typename genType>
		struct range_contiguous
		{
			static const bool value = true;
		};

		template<length_t C, length_t R, typename T, qualifier Q>
		struct range_contiguous<mat<C, R, T, Q> >
		{
			static const bool value = sizeof(mat<C, R, T, Q>) == sizeof(T) * C * R;
		};
	}//namespace detail

	/// @addtogroup gtx_range
	/// @{

#	if GLM_COMPILER & GLM_COMPILER_VC
#		pragma warning(push)
#		pragma warning(disable : 4100) // unreferenced formal parameter
#	endif

	template<typename T, qualifier Q>
	inline length_t components(vec<1, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<2, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<3, T, Q> const& v)
	{
		return v.length();
	}

	template<typename T, qualifier Q>
	inline length_t components(vec<4, T, Q> const& v)
	{
		return v.length();
	}

	template<typename genType>
	inline length_t components(genType const& m)
	{
		return m.length() * m[0].length();
	}

	template<typename genType>
	inline typename genType::value_type const * begin(genType const& v)
	{
		static_assert(detail::range_contiguous<genType>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return value_ptr(v);
	}

	template<typename genType>
	inline typename genType::value_type const * end(genType const& v)
	{
		return begin(v) + components(v);
	}

	template<typename genType>
	inline typename genType::value_type * begin(genType& v)
	{
		static_assert(detail::range_contiguous<genType>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return value_ptr(v);
	}

	template<typename genType>
	inline typename genType::value_type * end(genType& v)
	{
		return begin(v) + components(v);
	}

#	if GLM_COMPILER & GLM_COMPILER_VC
#		pragma warning(pop)
#	endif

	/// @}
}//namespace glm
