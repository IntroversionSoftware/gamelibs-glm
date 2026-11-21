namespace glm
{
	/*
	namespace details {
		template<typename T, glm::length_t L>
		struct known_length_iterator;
	}
	*/

	namespace detail
	{
		// Flat iteration over a matrix needs its columns to be adjacent. Aligned 3-row matrices pad each
		// column to 4 components, so a T* range over them would visit padding and miss the last column.
		template<length_t C, length_t R, typename T, qualifier Q>
		struct iteration_contiguous
		{
			static const bool value = sizeof(mat<C, R, T, Q>) == sizeof(T) * C * R;
		};
	}//namespace detail

	/// @addtogroup gtx_iteration
	/// @{
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* begin(vec<L,T,Q>& v) {
		return &v[0];
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* begin(mat<C,R,T,Q>& m) {
		static_assert(detail::iteration_contiguous<C, R, T, Q>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return &m[0][0];
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* begin(qua<T,Q>& q) {
		return &q[0];
	}
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* begin(const vec<L,T,Q>& v) {
		return &v[0];
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* begin(const mat<C,R,T,Q>& m) {
		static_assert(detail::iteration_contiguous<C, R, T, Q>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return &m[0][0];
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* begin(const qua<T,Q>& q) {
		return &q[0];
	}

	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* end(vec<L,T,Q>& v) {
		return &v[0] + L;
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* end(mat<C,R,T,Q>& m) {
		static_assert(detail::iteration_contiguous<C, R, T, Q>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return (&m[0][0]) + C*R;
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T* end(qua<T,Q>& q) {
		return (&q[0]) + 4;
	}
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* end(const vec<L,T,Q>& v) {
		return &v[0] + L;
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* end(const mat<C,R,T,Q>& m) {
		static_assert(detail::iteration_contiguous<C, R, T, Q>::value, "glm::begin/end: the columns of this matrix are padded (3 rows, aligned); iterate a packed matrix instead, see glm::to_packed");
		return (&m[0][0]) + C*R;
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR const T* end(const qua<T,Q>& q) {
		return (&q[0]) + 4;
	}

	// Reverse iteration
	// rbegin,rend
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rbegin(vec<L,T,Q>& v) {
		return std::reverse_iterator<T*>(end(v));
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rbegin(mat<C,R,T,Q>& m) {
		return std::reverse_iterator<T*>(end(m));
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rbegin(qua<T,Q>& q) {
		return std::reverse_iterator<T*>(end(q));
	}
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rbegin(const vec<L,T,Q>& v) {
		return std::reverse_iterator<const T*>(end(v));
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rbegin(const mat<C,R,T,Q>& m) {
		return std::reverse_iterator<const T*>(end(m));
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rbegin(const qua<T,Q>& q) {
		return std::reverse_iterator<const T*>(end(q));
	}

	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rend(vec<L,T,Q>& v) {
		return std::reverse_iterator<T*>(begin(v));
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rend(mat<C,R,T,Q>& m) {
		return std::reverse_iterator<T*>(begin(m));
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<T*> rend(qua<T,Q>& q) {
		return std::reverse_iterator<T*>(begin(q));
	}
	template<length_t L,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rend(const vec<L,T,Q>& v) {
		return std::reverse_iterator<const T*>(begin(v));
	}
	template<length_t C,length_t R,typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rend(const mat<C,R,T,Q>& m) {
		return std::reverse_iterator<const T*>(begin(m));
	}
	template<typename T,qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR std::reverse_iterator<const T*> rend(const qua<T,Q>& q) {
		return std::reverse_iterator<const T*>(begin(q));
	}


	/// @}
}//namespace glm
