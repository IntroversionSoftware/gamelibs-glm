/// @ref core

#include "./compute_vector_relational.hpp"

namespace glm
{
namespace detail
{
	template<typename T, qualifier Q, bool Aligned>
	struct compute_vec2_add
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x + b.x, a.y + b.y);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_vec2_sub
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x - b.x, a.y - b.y);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_vec2_mul
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x * b.x, a.y * b.y);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_vec2_div
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x / b.x, a.y / b.y);
		}
	};

	template<typename T, qualifier Q, bool Aligned>
	struct compute_vec2_mod
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x % b.x, a.y % b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_and
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x & b.x, a.y & b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_or
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x | b.x, a.y | b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_xor
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x ^ b.x, a.y ^ b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_shift_left
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x << b.x, a.y << b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_shift_right
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &a, vec<2, T, Q> const &b)
		{
			return vec<2, T, Q>(a.x >> b.x, a.y >> b.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_equal
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static bool call(vec<2, T, Q> const &v1, vec<2, T, Q> const &v2)
		{
			return
				detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.x, v2.x) &&
				detail::compute_equal<T, std::numeric_limits<T>::is_iec559>::call(v1.y, v2.y);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_nequal
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static bool call(vec<2, T, Q> const &v1, vec<2, T, Q> const &v2)
		{
			return !compute_vec2_equal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
		}
	};

	template<typename T, qualifier Q, int IsInt, std::size_t Size, bool Aligned>
	struct compute_vec2_bitwise_not
	{
		GLM_FUNC_QUALIFIER GLM_CONSTEXPR static vec<2, T, Q> call(vec<2, T, Q> const &v)
		{
			return vec<2, T, Q>(~v.x, ~v.y);
		}
	};
}//namespace detail

	// -- Implicit basic constructors --

#	if GLM_CONFIG_DEFAULTED_DEFAULT_CTOR == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_DEFAULTED_DEFAULT_CTOR_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec()
#			if GLM_CONFIG_CTOR_INIT != GLM_CTOR_INIT_DISABLE
				: x(0), y(0)
#			endif
		{}
#	endif

#	if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_DEFAULTED_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<2, T, Q> const& v)
			: x(v.x), y(v.y)
		{}
#	endif

	template<typename T, qualifier Q>
	template<qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<2, T, P> const& v)
		: x(v.x), y(v.y)
	{}

	// -- Explicit basic constructors --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(T scalar)
		: x(scalar), y(scalar)
	{}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(T _x, T _y)
		: x(_x), y(_y)
	{}

	// -- Conversion scalar constructors --

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<1, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.x))
	{}

	template<typename T, qualifier Q>
	template<typename A, typename B>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(A _x, B _y)
		: x(static_cast<T>(_x))
		, y(static_cast<T>(_y))
	{}

	template<typename T, qualifier Q>
	template<typename A, typename B>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<1, A, Q> const& _x, B _y)
		: x(static_cast<T>(_x.x))
		, y(static_cast<T>(_y))
	{}

	template<typename T, qualifier Q>
	template<typename A, typename B>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(A _x, vec<1, B, Q> const& _y)
		: x(static_cast<T>(_x))
		, y(static_cast<T>(_y.x))
	{}

	template<typename T, qualifier Q>
	template<typename A, typename B>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<1, A, Q> const& _x, vec<1, B, Q> const& _y)
		: x(static_cast<T>(_x.x))
		, y(static_cast<T>(_y.x))
	{}

	// -- Conversion vector constructors --

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<2, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.y))
	{}

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<3, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.y))
	{}

	template<typename T, qualifier Q>
	template<typename U, qualifier P>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>::vec(vec<4, U, P> const& v)
		: x(static_cast<T>(v.x))
		, y(static_cast<T>(v.y))
	{}

	// -- Component accesses --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T & vec<2, T, Q>::operator[](typename vec<2, T, Q>::length_type i)
	{
		assert(i >= 0 && i < this->length());
		switch(i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		}
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR T const& vec<2, T, Q>::operator[](typename vec<2, T, Q>::length_type i) const
	{
		assert(i >= 0 && i < this->length());
		switch(i)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		}
	}

	// -- Unary arithmetic operators --

#	if GLM_CONFIG_DEFAULTED_FUNCTIONS == GLM_DISABLE
		template<typename T, qualifier Q>
		GLM_DEFAULTED_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator=(vec<2, T, Q> const& v)
		{
			this->x = v.x;
			this->y = v.y;
			return *this;
		}
#	endif

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q>& vec<2, T, Q>::operator=(vec<2, U, Q> const& v)
	{
		vec<2, T, Q> newvec(v);
		return *this = newvec;
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator+=(U scalar)
	{
		return (*this = detail::compute_vec2_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator+=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v.x)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator+=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_add<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator-=(U scalar)
	{
		return (*this = detail::compute_vec2_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator-=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v.x)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator-=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_sub<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator*=(U scalar)
	{
		return (*this = detail::compute_vec2_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator*=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v.x)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator*=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_mul<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator/=(U scalar)
	{
		return (*this = detail::compute_vec2_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator/=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v.x)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator/=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_div<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	// -- Increment and decrement operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator++()
	{
		++this->x;
		++this->y;
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator--()
	{
		--this->x;
		--this->y;
		return *this;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> vec<2, T, Q>::operator++(int)
	{
		vec<2, T, Q> Result(*this);
		++*this;
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> vec<2, T, Q>::operator--(int)
	{
		vec<2, T, Q> Result(*this);
		--*this;
		return Result;
	}

	// -- Unary bit operators --

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator%=(U scalar)
	{
		return (*this = detail::compute_vec2_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator%=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator%=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_mod<T, Q, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator&=(U scalar)
	{
		return (*this = detail::compute_vec2_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator&=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator&=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_and<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator|=(U scalar)
	{
		return (*this = detail::compute_vec2_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator|=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator|=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_or<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator^=(U scalar)
	{
		return (*this = detail::compute_vec2_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator^=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator^=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_xor<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator<<=(U scalar)
	{
		return (*this = detail::compute_vec2_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator<<=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator<<=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_shift_left<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator>>=(U scalar)
	{
		return (*this = detail::compute_vec2_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(scalar)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator>>=(vec<1, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	template<typename T, qualifier Q>
	template<typename U>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> & vec<2, T, Q>::operator>>=(vec<2, U, Q> const& v)
	{
		return (*this = detail::compute_vec2_shift_right<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(*this, vec<2, T, Q>(v)));
	}

	// -- Unary arithmetic operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v)
	{
		return v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(0) -= v;
	}

	// -- Binary arithmetic operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) += scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) += v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(v) += scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v2) += v1;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator+(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) += v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) -= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) -= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) -= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) -= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator-(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) -= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) *= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) *= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator*(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(v) *= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator*(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v2) *= v1;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator*(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) *= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) /= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) /= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator/(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) /= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator/(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) /= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator/(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) /= v2;
	}

	// -- Binary bit operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) %= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) %= v2.x;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator%(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) %= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator%(vec<1, T, Q> const& scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar.x) %= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator%(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) %= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) &= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v, vec<1, T, Q> const& scalar)
	{
		return vec<2, T, Q>(v) &= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator&(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) &= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator&(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) &= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator&(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) &= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) |= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) |= v2.x;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator|(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) |= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator|(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) |= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator|(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) |= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) ^= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) ^= v2.x;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator^(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) ^= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator^(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) ^= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator^(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) ^= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) <<= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) <<= v2.x;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator<<(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) <<= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) <<= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator<<(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) <<= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v, T scalar)
	{
		return vec<2, T, Q>(v) >>= scalar;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v1, vec<1, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) >>= v2.x;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator>>(T scalar, vec<2, T, Q> const& v)
	{
		return vec<2, T, Q>(scalar) >>= v;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<1, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1.x) >>= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator>>(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return vec<2, T, Q>(v1) >>= v2;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, T, Q> operator~(vec<2, T, Q> const& v)
	{
		return detail::compute_vec2_bitwise_not<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v);
	}

	// -- Boolean operators --

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator==(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return detail::compute_vec2_equal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR bool operator!=(vec<2, T, Q> const& v1, vec<2, T, Q> const& v2)
	{
		return detail::compute_vec2_nequal<T, Q, detail::is_int<T>::value, sizeof(T) * 8, detail::is_aligned<Q>::value>::call(v1, v2);
	}

	template<qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, bool, Q> operator&&(vec<2, bool, Q> const& v1, vec<2, bool, Q> const& v2)
	{
		return vec<2, bool, Q>(v1.x && v2.x, v1.y && v2.y);
	}

	template<qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, bool, Q> operator||(vec<2, bool, Q> const& v1, vec<2, bool, Q> const& v2)
	{
		return vec<2, bool, Q>(v1.x || v2.x, v1.y || v2.y);
	}
}//namespace glm

#if GLM_CONFIG_SIMD == GLM_ENABLE
#	include "type_vec2_simd.inl"
#endif
