#pragma once

#include <type_traits>
#include <utility>

namespace glm{
namespace detail
{
	// Internal class for implementing swizzle operators
	template<typename T, int N>
	struct _swizzle_base0
	{
	protected:
		GLM_FUNC_QUALIFIER T& elem(int i){ return (reinterpret_cast<T*>(_buffer))[i]; }
		GLM_FUNC_QUALIFIER T const& elem(int i) const{ return (reinterpret_cast<const T*>(_buffer))[i]; }

		// Use an opaque buffer to *ensure* the compiler doesn't call a constructor.
		// The size 1 buffer is assumed to aligned to the actual members so that the
		// elem()
		char    _buffer[1];
	};

	// Number of source components a swizzle reaches: one past its highest index. Only the first N
	// indices are meaningful; E2/E3 are filler (-1, -2, 3) for shorter swizzles.
	//
	// SIMD implementations use this to bound their accesses, since the swizzle types don't know how
	// wide their source vector is. Every index a swizzle uses exists in its source, so touching
	// [0, extent) is always in bounds, and an extent of 3 or more implies a vec3/vec4 source.
	constexpr int _swizzle_extent(length_t N, int E0, int E1, int E2, int E3)
	{
		int m = E0 > E1 ? E0 : E1;
		if (N > 2 && E2 > m)
			m = E2;
		if (N > 3 && E3 > m)
			m = E3;
		return m + 1;
	}

	// Portable implementation, valid for any source vector and qualifier. This is the default; SIMD
	// paths specialize _swizzle_base1 only where they have something better.
	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_scalar : public _swizzle_base0<T, N>
	{
		GLM_FUNC_QUALIFIER vec<N, T, Q> operator ()()  const
		{
			if constexpr (N == 2)
				return vec<2, T, Q>(this->elem(E0), this->elem(E1));
			else if constexpr (N == 3)
				return vec<3, T, Q>(this->elem(E0), this->elem(E1), this->elem(E2));
			else
				return vec<4, T, Q>(this->elem(E0), this->elem(E1), this->elem(E2), this->elem(E3));
		}
	};

	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3, bool UseSimd>
	struct _swizzle_base1 : public _swizzle_scalar<N, T, Q, E0, E1, E2, E3>
	{
	};
}//namespace detail
}//namespace glm

// SIMD specializations of _swizzle_base1, and helpers for the vectorized write path below. These must
// be declared before any vec type is instantiated: vec<L, T, Q> instantiates all of its swizzle members,
// and a specialization declared after that point is silently ignored for them.
#include "_swizzle_simd.inl"

namespace glm{
namespace detail
{

	// Internal class for implementing swizzle operators
	/*
		Template parameters:

		T			= type of scalar values (e.g. float, double)
		N			= number of components in the vector (e.g. 3)
		E0...3		= what index the n-th element of this swizzle refers to in the unswizzled vec

		DUPLICATE_ELEMENTS = 1 if there is a repeated element, 0 otherwise (used to specialize swizzles
			containing duplicate elements so that they cannot be used as r-values).
	*/
	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3, int DUPLICATE_ELEMENTS>
	struct _swizzle_base2 : public _swizzle_base1<N, T, Q, E0,E1,E2,E3, detail::is_aligned<Q>::value>
	{
		// Each op returns the new value of a destination component (e) given the operand (t). They are
		// templates so the vectorized write path can apply them to whole vectors at once.
		struct op_equal
		{
			template<typename V>
			GLM_FUNC_QUALIFIER V operator() (V const&, V const& t) const{ return t; }
		};

		struct op_minus
		{
			template<typename V>
			GLM_FUNC_QUALIFIER V operator() (V const& e, V const& t) const{ return e - t; }
		};

		struct op_plus
		{
			template<typename V>
			GLM_FUNC_QUALIFIER V operator() (V const& e, V const& t) const{ return e + t; }
		};

		struct op_mul
		{
			template<typename V>
			GLM_FUNC_QUALIFIER V operator() (V const& e, V const& t) const{ return e * t; }
		};

		struct op_div
		{
			template<typename V>
			GLM_FUNC_QUALIFIER V operator() (V const& e, V const& t) const{ return e / t; }
		};

	public:
		GLM_FUNC_QUALIFIER _swizzle_base2& operator= (const T& t)
		{
			_apply_op(vec<N, T, Q>(t), op_equal());
			return *this;
		}

		GLM_FUNC_QUALIFIER _swizzle_base2& operator= (vec<N, T, Q> const& that)
		{
			_apply_op(that, op_equal());
			return *this;
		}

		GLM_FUNC_QUALIFIER void operator -= (vec<N, T, Q> const& that)
		{
			_apply_op(that, op_minus());
		}

		GLM_FUNC_QUALIFIER void operator += (vec<N, T, Q> const& that)
		{
			_apply_op(that, op_plus());
		}

		GLM_FUNC_QUALIFIER void operator *= (vec<N, T, Q> const& that)
		{
			_apply_op(that, op_mul());
		}

		GLM_FUNC_QUALIFIER void operator /= (vec<N, T, Q> const& that)
		{
			_apply_op(that, op_div());
		}

		GLM_FUNC_QUALIFIER T& operator[](int i)
		{
			const int offset_dst[4] = { E0, E1, E2, E3 };
			return this->elem(offset_dst[i]);
		}
		GLM_FUNC_QUALIFIER T operator[](int i) const
		{
			const int offset_dst[4] = { E0, E1, E2, E3 };
			return this->elem(offset_dst[i]);
		}

	protected:
		template<typename U>
		GLM_FUNC_QUALIFIER void _apply_op(vec<N, T, Q> const& that, const U& op)
		{
#			if GLM_ARCH & GLM_ARCH_CLANG_BIT
			if constexpr (detail::is_aligned<Q>::value && !std::is_same<T, bool>::value)
			{
				// Read-modify-write the reachable source lanes as one vector (see _swizzle_simd.inl).
				detail::_swizzle_clang_apply<N, T, E0, E1, E2, E3>(this->_buffer, that.data, op);
				return;
			}
#			endif

			// Make a copy of the data in this == &that.
			// The copier should optimize out the copy in cases where the function is
			// properly inlined and the copy is not necessary.
			T t[N];
			for (int i = 0; i < N; ++i)
				t[i] = that[i];
			for (int i = 0; i < N; ++i)
				(*this)[i] = op( (*this)[i], t[i] );
		}
	};

	// Specialization for swizzles containing duplicate elements.  These cannot be modified.
	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base2<N, T, Q, E0,E1,E2,E3, 1> : public _swizzle_base1<N, T, Q, E0,E1,E2,E3, detail::is_aligned<Q>::value>
	{
		struct Stub {};

		GLM_FUNC_QUALIFIER _swizzle_base2& operator= (Stub const&) { return *this; }

		GLM_FUNC_QUALIFIER T operator[]  (int i) const
		{
			const int offset_dst[4] = { E0, E1, E2, E3 };
			return this->elem(offset_dst[i]);
		}
	};

	// Whether a swizzle repeats a component. Only the first N indices count: 3-component swizzles of a
	// vec3/vec4 use 3 as filler for E3, which would otherwise make e.g. vec4::xwz read-only.
	constexpr bool _swizzle_has_duplicates(length_t N, int E0, int E1, int E2, int E3)
	{
		return E0 == E1
			|| (N > 2 && (E0 == E2 || E1 == E2))
			|| (N > 3 && (E0 == E3 || E1 == E3 || E2 == E3));
	}

	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle : public _swizzle_base2<N, T, Q, E0, E1, E2, E3, _swizzle_has_duplicates(N, E0, E1, E2, E3)>
	{
		typedef _swizzle_base2<N, T, Q, E0, E1, E2, E3, _swizzle_has_duplicates(N, E0, E1, E2, E3)> base_type;

		using base_type::operator=;

		GLM_FUNC_QUALIFIER operator vec<N, T, Q> () const { return (*this)(); }
	};

//
// To prevent the C++ syntax from getting entirely overwhelming, define some alias macros
//
#define GLM_SWIZZLE_TEMPLATE1   template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3>
#define GLM_SWIZZLE_TEMPLATE2   template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3, int F0, int F1, int F2, int F3>
#define GLM_SWIZZLE_TYPE1       _swizzle<N, T, Q, E0, E1, E2, E3>
#define GLM_SWIZZLE_TYPE2       _swizzle<N, T, Q, F0, F1, F2, F3>

//
// Wrapper for a binary operator (e.g. u.yy + v.zy)
//
#define GLM_SWIZZLE_VECTOR_BINARY_OPERATOR_IMPLEMENTATION(OPERAND)                 \
	GLM_SWIZZLE_TEMPLATE2                                                          \
	GLM_FUNC_QUALIFIER vec<N, T, Q> operator OPERAND ( const GLM_SWIZZLE_TYPE1& a, const GLM_SWIZZLE_TYPE2& b)  \
	{                                                                               \
		return a() OPERAND b();                                                     \
	}                                                                               \
	GLM_SWIZZLE_TEMPLATE1                                                          \
	GLM_FUNC_QUALIFIER vec<N, T, Q> operator OPERAND ( const GLM_SWIZZLE_TYPE1& a, const vec<N, T, Q>& b)                   \
	{                                                                               \
		return a() OPERAND b;                                                       \
	}                                                                               \
	GLM_SWIZZLE_TEMPLATE1                                                          \
	GLM_FUNC_QUALIFIER vec<N, T, Q> operator OPERAND ( const vec<N, T, Q>& a, const GLM_SWIZZLE_TYPE1& b)                   \
	{                                                                               \
		return a OPERAND b();                                                       \
	}

//
// Wrapper for a operand between a swizzle and a binary (e.g. 1.0f - u.xyz)
//
#define GLM_SWIZZLE_SCALAR_BINARY_OPERATOR_IMPLEMENTATION(OPERAND)								\
	GLM_SWIZZLE_TEMPLATE1																		\
	GLM_FUNC_QUALIFIER vec<N, T, Q> operator OPERAND ( const GLM_SWIZZLE_TYPE1& a, const T& b)	\
	{																							\
		return a() OPERAND b;																	\
	}																							\
	GLM_SWIZZLE_TEMPLATE1																		\
	GLM_FUNC_QUALIFIER vec<N, T, Q> operator OPERAND ( const T& a, const GLM_SWIZZLE_TYPE1& b)	\
	{																							\
		return a OPERAND b();																	\
	}

//
// Macro for wrapping a function taking one argument (e.g. abs())
//
#define GLM_SWIZZLE_FUNCTION_1_ARGS(RETURN_TYPE,FUNCTION)												\
	GLM_SWIZZLE_TEMPLATE1																				\
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a)		\
	{																									\
		return FUNCTION(a());																			\
	}

//
// Macro for wrapping a function taking two vector arguments (e.g. dot()).
//
#define GLM_SWIZZLE_FUNCTION_2_ARGS(RETURN_TYPE,FUNCTION)                                                       \
	GLM_SWIZZLE_TEMPLATE2                                                                                       \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const GLM_SWIZZLE_TYPE2& b) \
	{                                                                                                           \
		return FUNCTION(a(), b());                                                                              \
	}                                                                                                           \
	GLM_SWIZZLE_TEMPLATE1                                                                                       \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const GLM_SWIZZLE_TYPE1& b) \
	{                                                                                                           \
		return FUNCTION(a(), b());                                                                              \
	}                                                                                                           \
	GLM_SWIZZLE_TEMPLATE1                                                                                       \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const typename V& b)         \
	{                                                                                                           \
		return FUNCTION(a(), b);                                                                                \
	}                                                                                                           \
	GLM_SWIZZLE_TEMPLATE1                                                                                       \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const V& a, const GLM_SWIZZLE_TYPE1& b)                  \
	{                                                                                                           \
		return FUNCTION(a, b());                                                                                \
	}

//
// Macro for wrapping a function take 2 vec arguments followed by a scalar (e.g. mix()).
//
#define GLM_SWIZZLE_FUNCTION_2_ARGS_SCALAR(RETURN_TYPE,FUNCTION)                                                             \
	GLM_SWIZZLE_TEMPLATE2                                                                                                    \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const GLM_SWIZZLE_TYPE2& b, const T& c)   \
	{                                                                                                                         \
		return FUNCTION(a(), b(), c);                                                                                         \
	}                                                                                                                         \
	GLM_SWIZZLE_TEMPLATE1                                                                                                    \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const GLM_SWIZZLE_TYPE1& b, const T& c)   \
	{                                                                                                                         \
		return FUNCTION(a(), b(), c);                                                                                         \
	}                                                                                                                         \
	GLM_SWIZZLE_TEMPLATE1                                                                                                    \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const GLM_SWIZZLE_TYPE1& a, const typename S0::vec_type& b, const T& c)\
	{                                                                                                                         \
		return FUNCTION(a(), b, c);                                                                                           \
	}                                                                                                                         \
	GLM_SWIZZLE_TEMPLATE1                                                                                                    \
	GLM_FUNC_QUALIFIER typename GLM_SWIZZLE_TYPE1::RETURN_TYPE FUNCTION(const typename V& a, const GLM_SWIZZLE_TYPE1& b, const T& c)           \
	{                                                                                                                         \
		return FUNCTION(a, b(), c);                                                                                           \
	}

}//namespace detail
}//namespace glm

namespace glm
{
	namespace detail
	{
		GLM_SWIZZLE_SCALAR_BINARY_OPERATOR_IMPLEMENTATION(-)
		GLM_SWIZZLE_SCALAR_BINARY_OPERATOR_IMPLEMENTATION(*)
		GLM_SWIZZLE_VECTOR_BINARY_OPERATOR_IMPLEMENTATION(+)
		GLM_SWIZZLE_VECTOR_BINARY_OPERATOR_IMPLEMENTATION(-)
		GLM_SWIZZLE_VECTOR_BINARY_OPERATOR_IMPLEMENTATION(*)
		GLM_SWIZZLE_VECTOR_BINARY_OPERATOR_IMPLEMENTATION(/)
	}

	//
	// Swizzles are distinct types from the unswizzled type.  The below macros will
	// provide template specializations for the swizzle types for the given functions
	// so that the compiler does not have any ambiguity to choosing how to handle
	// the function.
	//
	// The alternative is to use the operator()() when calling the function in order
	// to explicitly convert the swizzled type to the unswizzled type.
	//

	//GLM_SWIZZLE_FUNCTION_1_ARGS(vec_type,    abs);
	//GLM_SWIZZLE_FUNCTION_1_ARGS(vec_type,    acos);
	//GLM_SWIZZLE_FUNCTION_1_ARGS(vec_type,    acosh);
	//GLM_SWIZZLE_FUNCTION_1_ARGS(vec_type,    all);
	//GLM_SWIZZLE_FUNCTION_1_ARGS(vec_type,    any);

	//GLM_SWIZZLE_FUNCTION_2_ARGS(value_type,  dot);
	//GLM_SWIZZLE_FUNCTION_2_ARGS(vec_type,    cross);
	//GLM_SWIZZLE_FUNCTION_2_ARGS(vec_type,    step);
	//GLM_SWIZZLE_FUNCTION_2_ARGS_SCALAR(vec_type, mix);
}

#define GLM_SWIZZLE2_2_MEMBERS(T, Q, E0,E1) \
	struct { detail::_swizzle<2, T, Q, 0,0,-1,-2> E0 ## E0; }; \
	struct { detail::_swizzle<2, T, Q, 0,1,-1,-2> E0 ## E1; }; \
	struct { detail::_swizzle<2, T, Q, 1,0,-1,-2> E1 ## E0; }; \
	struct { detail::_swizzle<2, T, Q, 1,1,-1,-2> E1 ## E1; };

#define GLM_SWIZZLE2_3_MEMBERS(T, Q, E0,E1) \
	struct { detail::_swizzle<3,T, Q, 0,0,0,-1> E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<3,T, Q, 0,0,1,-1> E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<3,T, Q, 0,1,0,-1> E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<3,T, Q, 0,1,1,-1> E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<3,T, Q, 1,0,0,-1> E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<3,T, Q, 1,0,1,-1> E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<3,T, Q, 1,1,0,-1> E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<3,T, Q, 1,1,1,-1> E1 ## E1 ## E1; };

#define GLM_SWIZZLE2_4_MEMBERS(T, Q, E0,E1) \
	struct { detail::_swizzle<4,T, Q, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,1,1> E1 ## E1 ## E1 ## E1; };

#define GLM_SWIZZLE3_2_MEMBERS(T, Q, E0,E1,E2) \
	struct { detail::_swizzle<2,T, Q, 0,0,-1,-2> E0 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 0,1,-1,-2> E0 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 0,2,-1,-2> E0 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 1,0,-1,-2> E1 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 1,1,-1,-2> E1 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 1,2,-1,-2> E1 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 2,0,-1,-2> E2 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 2,1,-1,-2> E2 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 2,2,-1,-2> E2 ## E2; };

#define GLM_SWIZZLE3_3_MEMBERS(T, Q ,E0,E1,E2) \
	struct { detail::_swizzle<3, T, Q, 0,0,0,3> E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,0,1,3> E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,0,2,3> E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,0,3> E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,1,3> E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,2,3> E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,0,3> E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,1,3> E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,2,3> E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,0,3> E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,1,3> E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,2,3> E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,0,3> E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,1,3> E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,2,3> E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,0,3> E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,1,3> E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,2,3> E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,0,3> E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,1,3> E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,2,3> E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,0,3> E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,1,3> E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,2,3> E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,0,3> E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,1,3> E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,2,3> E2 ## E2 ## E2; };

#define GLM_SWIZZLE3_4_MEMBERS(T, Q, E0,E1,E2) \
	struct { detail::_swizzle<4,T, Q, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4,T, Q, 2,2,2,2> E2 ## E2 ## E2 ## E2; };

#define GLM_SWIZZLE4_2_MEMBERS(T, Q, E0,E1,E2,E3) \
	struct { detail::_swizzle<2,T, Q, 0,0,-1,-2> E0 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 0,1,-1,-2> E0 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 0,2,-1,-2> E0 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 0,3,-1,-2> E0 ## E3; }; \
	struct { detail::_swizzle<2,T, Q, 1,0,-1,-2> E1 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 1,1,-1,-2> E1 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 1,2,-1,-2> E1 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 1,3,-1,-2> E1 ## E3; }; \
	struct { detail::_swizzle<2,T, Q, 2,0,-1,-2> E2 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 2,1,-1,-2> E2 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 2,2,-1,-2> E2 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 2,3,-1,-2> E2 ## E3; }; \
	struct { detail::_swizzle<2,T, Q, 3,0,-1,-2> E3 ## E0; }; \
	struct { detail::_swizzle<2,T, Q, 3,1,-1,-2> E3 ## E1; }; \
	struct { detail::_swizzle<2,T, Q, 3,2,-1,-2> E3 ## E2; }; \
	struct { detail::_swizzle<2,T, Q, 3,3,-1,-2> E3 ## E3; };

#define GLM_SWIZZLE4_3_MEMBERS(T, Q, E0,E1,E2,E3) \
	struct { detail::_swizzle<3, T, Q, 0,0,0,3> E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,0,1,3> E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,0,2,3> E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,0,3,3> E0 ## E0 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,0,3> E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,1,3> E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,2,3> E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,1,3,3> E0 ## E1 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,0,3> E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,1,3> E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,2,3> E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,2,3,3> E0 ## E2 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 0,3,0,3> E0 ## E3 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 0,3,1,3> E0 ## E3 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 0,3,2,3> E0 ## E3 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 0,3,3,3> E0 ## E3 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,0,3> E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,1,3> E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,2,3> E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,0,3,3> E1 ## E0 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,0,3> E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,1,3> E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,2,3> E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,1,3,3> E1 ## E1 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,0,3> E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,1,3> E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,2,3> E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,2,3,3> E1 ## E2 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 1,3,0,3> E1 ## E3 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 1,3,1,3> E1 ## E3 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 1,3,2,3> E1 ## E3 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 1,3,3,3> E1 ## E3 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,0,3> E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,1,3> E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,2,3> E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,0,3,3> E2 ## E0 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,0,3> E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,1,3> E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,2,3> E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,1,3,3> E2 ## E1 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,0,3> E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,1,3> E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,2,3> E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,2,3,3> E2 ## E2 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 2,3,0,3> E2 ## E3 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 2,3,1,3> E2 ## E3 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 2,3,2,3> E2 ## E3 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 2,3,3,3> E2 ## E3 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 3,0,0,3> E3 ## E0 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 3,0,1,3> E3 ## E0 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 3,0,2,3> E3 ## E0 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 3,0,3,3> E3 ## E0 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 3,1,0,3> E3 ## E1 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 3,1,1,3> E3 ## E1 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 3,1,2,3> E3 ## E1 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 3,1,3,3> E3 ## E1 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 3,2,0,3> E3 ## E2 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 3,2,1,3> E3 ## E2 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 3,2,2,3> E3 ## E2 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 3,2,3,3> E3 ## E2 ## E3; }; \
	struct { detail::_swizzle<3, T, Q, 3,3,0,3> E3 ## E3 ## E0; }; \
	struct { detail::_swizzle<3, T, Q, 3,3,1,3> E3 ## E3 ## E1; }; \
	struct { detail::_swizzle<3, T, Q, 3,3,2,3> E3 ## E3 ## E2; }; \
	struct { detail::_swizzle<3, T, Q, 3,3,3,3> E3 ## E3 ## E3; };

#define GLM_SWIZZLE4_4_MEMBERS(T, Q, E0,E1,E2,E3) \
	struct { detail::_swizzle<4, T, Q, 0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,0,3> E0 ## E0 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,1,3> E0 ## E0 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,2,3> E0 ## E0 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,3,0> E0 ## E0 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,3,1> E0 ## E0 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,3,2> E0 ## E0 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,0,3,3> E0 ## E0 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,0,3> E0 ## E1 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,1,3> E0 ## E1 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,2,3> E0 ## E1 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,3,0> E0 ## E1 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,3,1> E0 ## E1 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,3,2> E0 ## E1 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,1,3,3> E0 ## E1 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,0,3> E0 ## E2 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,1,3> E0 ## E2 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,2,3> E0 ## E2 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,3,0> E0 ## E2 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,3,1> E0 ## E2 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,3,2> E0 ## E2 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,2,3,3> E0 ## E2 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,0,0> E0 ## E3 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,0,1> E0 ## E3 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,0,2> E0 ## E3 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,0,3> E0 ## E3 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,1,0> E0 ## E3 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,1,1> E0 ## E3 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,1,2> E0 ## E3 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,1,3> E0 ## E3 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,2,0> E0 ## E3 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,2,1> E0 ## E3 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,2,2> E0 ## E3 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,2,3> E0 ## E3 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,3,0> E0 ## E3 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,3,1> E0 ## E3 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,3,2> E0 ## E3 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 0,3,3,3> E0 ## E3 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,0,3> E1 ## E0 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,1,3> E1 ## E0 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,2,3> E1 ## E0 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,3,0> E1 ## E0 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,3,1> E1 ## E0 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,3,2> E1 ## E0 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,0,3,3> E1 ## E0 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,0,3> E1 ## E1 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,1,3> E1 ## E1 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,2,3> E1 ## E1 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,3,0> E1 ## E1 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,3,1> E1 ## E1 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,3,2> E1 ## E1 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,1,3,3> E1 ## E1 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,0,3> E1 ## E2 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,1,3> E1 ## E2 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,2,3> E1 ## E2 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,3,0> E1 ## E2 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,3,1> E1 ## E2 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,3,2> E1 ## E2 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,2,3,3> E1 ## E2 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,0,0> E1 ## E3 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,0,1> E1 ## E3 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,0,2> E1 ## E3 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,0,3> E1 ## E3 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,1,0> E1 ## E3 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,1,1> E1 ## E3 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,1,2> E1 ## E3 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,1,3> E1 ## E3 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,2,0> E1 ## E3 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,2,1> E1 ## E3 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,2,2> E1 ## E3 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,2,3> E1 ## E3 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,3,0> E1 ## E3 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,3,1> E1 ## E3 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,3,2> E1 ## E3 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 1,3,3,3> E1 ## E3 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,0,3> E2 ## E0 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,1,3> E2 ## E0 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,2,3> E2 ## E0 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,3,0> E2 ## E0 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,3,1> E2 ## E0 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,3,2> E2 ## E0 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,0,3,3> E2 ## E0 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,0,3> E2 ## E1 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,1,3> E2 ## E1 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,2,3> E2 ## E1 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,3,0> E2 ## E1 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,3,1> E2 ## E1 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,3,2> E2 ## E1 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,1,3,3> E2 ## E1 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,0,3> E2 ## E2 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,1,3> E2 ## E2 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,2,2> E2 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,2,3> E2 ## E2 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,3,0> E2 ## E2 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,3,1> E2 ## E2 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,3,2> E2 ## E2 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,2,3,3> E2 ## E2 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,0,0> E2 ## E3 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,0,1> E2 ## E3 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,0,2> E2 ## E3 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,0,3> E2 ## E3 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,1,0> E2 ## E3 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,1,1> E2 ## E3 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,1,2> E2 ## E3 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,1,3> E2 ## E3 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,2,0> E2 ## E3 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,2,1> E2 ## E3 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,2,2> E2 ## E3 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,2,3> E2 ## E3 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,3,0> E2 ## E3 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,3,1> E2 ## E3 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,3,2> E2 ## E3 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 2,3,3,3> E2 ## E3 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,0,0> E3 ## E0 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,0,1> E3 ## E0 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,0,2> E3 ## E0 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,0,3> E3 ## E0 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,1,0> E3 ## E0 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,1,1> E3 ## E0 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,1,2> E3 ## E0 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,1,3> E3 ## E0 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,2,0> E3 ## E0 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,2,1> E3 ## E0 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,2,2> E3 ## E0 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,2,3> E3 ## E0 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,3,0> E3 ## E0 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,3,1> E3 ## E0 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,3,2> E3 ## E0 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,0,3,3> E3 ## E0 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,0,0> E3 ## E1 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,0,1> E3 ## E1 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,0,2> E3 ## E1 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,0,3> E3 ## E1 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,1,0> E3 ## E1 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,1,1> E3 ## E1 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,1,2> E3 ## E1 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,1,3> E3 ## E1 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,2,0> E3 ## E1 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,2,1> E3 ## E1 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,2,2> E3 ## E1 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,2,3> E3 ## E1 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,3,0> E3 ## E1 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,3,1> E3 ## E1 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,3,2> E3 ## E1 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,1,3,3> E3 ## E1 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,0,0> E3 ## E2 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,0,1> E3 ## E2 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,0,2> E3 ## E2 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,0,3> E3 ## E2 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,1,0> E3 ## E2 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,1,1> E3 ## E2 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,1,2> E3 ## E2 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,1,3> E3 ## E2 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,2,0> E3 ## E2 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,2,1> E3 ## E2 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,2,2> E3 ## E2 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,2,3> E3 ## E2 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,3,0> E3 ## E2 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,3,1> E3 ## E2 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,3,2> E3 ## E2 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,2,3,3> E3 ## E2 ## E3 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,0,0> E3 ## E3 ## E0 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,0,1> E3 ## E3 ## E0 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,0,2> E3 ## E3 ## E0 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,0,3> E3 ## E3 ## E0 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,1,0> E3 ## E3 ## E1 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,1,1> E3 ## E3 ## E1 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,1,2> E3 ## E3 ## E1 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,1,3> E3 ## E3 ## E1 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,2,0> E3 ## E3 ## E2 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,2,1> E3 ## E3 ## E2 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,2,2> E3 ## E3 ## E2 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,2,3> E3 ## E3 ## E2 ## E3; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,3,0> E3 ## E3 ## E3 ## E0; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,3,1> E3 ## E3 ## E3 ## E1; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,3,2> E3 ## E3 ## E3 ## E2; }; \
	struct { detail::_swizzle<4, T, Q, 3,3,3,3> E3 ## E3 ## E3 ## E3; };
