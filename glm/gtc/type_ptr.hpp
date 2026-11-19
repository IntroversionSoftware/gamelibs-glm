/// @ref gtc_type_ptr
/// @file glm/gtc/type_ptr.hpp
///
/// @see core (dependence)
/// @see gtc_quaternion (dependence)
///
/// @defgroup gtc_type_ptr GLM_GTC_type_ptr
/// @ingroup gtc
///
/// Include <glm/gtc/type_ptr.hpp> to use the features of this extension.
///
/// Handles the interaction between pointers and vector, matrix types.
///
/// This extension defines an overloaded function, glm::value_ptr. It returns
/// a pointer to the memory layout of the object. Matrix types store their values
/// in column-major order.
///
/// This is useful for uploading data to matrices or copying data to buffer objects.
///
/// Example:
/// @code
/// #include <glm/glm.hpp>
/// #include <glm/gtc/type_ptr.hpp>
///
/// glm::vec3 aVector(3);
/// glm::mat4 someMatrix(1.0);
///
/// glUniform3fv(uniformLoc, 1, glm::value_ptr(aVector));
/// glUniformMatrix4fv(uniformMatrixLoc, 1, GL_FALSE, glm::value_ptr(someMatrix));
/// @endcode
///
/// value_ptr points at the type's own storage. Aligned types (including the default types under
/// GLM_FORCE_DEFAULT_ALIGNED_GENTYPES) pad 3-component vectors to 4 components, so a matrix with
/// 3 rows has a padding component after each column. That matches std140/std430 buffer layout, but
/// not APIs that expect tightly packed components; convert with glm::to_packed first:
/// @code
/// glm::mat3 normalMatrix = ...;
/// glUniformMatrix3fv(uniformMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::to_packed(normalMatrix)));
/// @endcode
/// The temporary returned by to_packed lives until the end of the full expression.
///
/// <glm/gtc/type_ptr.hpp> need to be included to use the features of this extension.

#pragma once

// Dependency:
#include "../gtc/quaternion.hpp"
#include "../gtc/vec1.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../mat2x2.hpp"
#include "../mat2x3.hpp"
#include "../mat2x4.hpp"
#include "../mat3x2.hpp"
#include "../mat3x3.hpp"
#include "../mat3x4.hpp"
#include "../mat4x2.hpp"
#include "../mat4x3.hpp"
#include "../mat4x4.hpp"
#include <cstring>
#include <utility>

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_type_ptr extension included")
#endif

namespace glm
{
	/// @addtogroup gtc_type_ptr
	/// @{

	/// Return the constant address to the data of the input parameter.
	/// @see gtc_type_ptr
	template<typename genType>
	GLM_FUNC_DECL typename genType::value_type const * value_ptr(genType const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<1, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<2, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<3, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<1, T, Q> make_vec1(vec<4, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<1, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<2, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<3, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<2, T, Q> make_vec2(vec<4, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<1, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<2, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<3, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> make_vec3(vec<4, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<1, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<2, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<3, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template <typename T, qualifier Q>
	GLM_FUNC_DECL vec<4, T, Q> make_vec4(vec<4, T, Q> const& v);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL vec<2, T, defaultp> make_vec2(T const * const ptr);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL vec<3, T, defaultp> make_vec3(T const * const ptr);

	/// Build a vector from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL vec<4, T, defaultp> make_vec4(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<2, 2, T, defaultp> make_mat2x2(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<2, 3, T, defaultp> make_mat2x3(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<2, 4, T, defaultp> make_mat2x4(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<3, 2, T, defaultp> make_mat3x2(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<3, 3, T, defaultp> make_mat3x3(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<3, 4, T, defaultp> make_mat3x4(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<4, 2, T, defaultp> make_mat4x2(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<4, 3, T, defaultp> make_mat4x3(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> make_mat4x4(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<2, 2, T, defaultp> make_mat2(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<3, 3, T, defaultp> make_mat3(T const * const ptr);

	/// Build a matrix from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL mat<4, 4, T, defaultp> make_mat4(T const * const ptr);

	/// Build a quaternion from a pointer.
	/// @see gtc_type_ptr
	template<typename T>
	GLM_FUNC_DECL qua<T, defaultp> make_quat(T const * const ptr);

	/// Convert to the packed qualifier of the same precision. Packed types have no padding, so
	/// value_ptr of the result points to exactly L (or C * R) contiguous components.
	/// @see gtc_type_ptr
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, detail::packed_qualifier<Q>::value> to_packed(vec<L, T, Q> const& v);

	/// Convert to the packed qualifier of the same precision. Packed types have no padding, so
	/// value_ptr of the result points to exactly L (or C * R) contiguous components.
	/// @see gtc_type_ptr
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR mat<C, R, T, detail::packed_qualifier<Q>::value> to_packed(mat<C, R, T, Q> const& m);

	/// Convert to the packed qualifier of the same precision.
	/// @see gtc_type_ptr
	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR qua<T, detail::packed_qualifier<Q>::value> to_packed(qua<T, Q> const& q);

	/// Convert to the aligned qualifier of the same precision (packed when aligned types are disabled).
	/// @see gtc_type_ptr
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR vec<L, T, detail::aligned_qualifier<Q>::value> to_aligned(vec<L, T, Q> const& v);

	/// Convert to the aligned qualifier of the same precision (packed when aligned types are disabled).
	/// @see gtc_type_ptr
	template<length_t C, length_t R, typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR mat<C, R, T, detail::aligned_qualifier<Q>::value> to_aligned(mat<C, R, T, Q> const& m);

	/// Convert to the aligned qualifier of the same precision (packed when aligned types are disabled).
	/// @see gtc_type_ptr
	template<typename T, qualifier Q>
	GLM_FUNC_DECL GLM_CONSTEXPR qua<T, detail::aligned_qualifier<Q>::value> to_aligned(qua<T, Q> const& q);

	/// The packed counterpart of a vector, matrix or quaternion type: packed_t<mat3> is mat<3, 3, float, packed_highp>.
	/// @see gtc_type_ptr
	template<typename genType>
	using packed_t = decltype(to_packed(std::declval<genType const&>()));

	/// The aligned counterpart of a vector, matrix or quaternion type.
	/// @see gtc_type_ptr
	template<typename genType>
	using aligned_t = decltype(to_aligned(std::declval<genType const&>()));

	/// @}
}//namespace glm

#include "type_ptr.inl"
