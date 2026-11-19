/// @ref core
/// @file glm/detail/_swizzle_simd.inl
///
/// SIMD specializations of the swizzle operator implementation. Included from _swizzle.hpp, ahead of
/// any vec type definition, so these are visible whenever a vec instantiates its swizzle members.
///
/// The swizzle types only know their own indices, not how wide their source vector is, so every
/// access here is bounded by _swizzle_extent(): the source has at least that many components.

#pragma once

#if GLM_ARCH & GLM_ARCH_CLANG_BIT
namespace glm{
namespace detail
{
	template<typename T, int L>
	using _swizzle_clang_vec = T __attribute__((ext_vector_type(L)));

	// Lanes moved when accessing a swizzle's source. A swizzle within x/y may come from an 8-byte vec2,
	// so it moves exactly two. Anything reaching z or w comes from an aligned vec3/vec4, whose
	// ext_vector storage is four lanes wide (a 3-lane ext_vector is padded to 4), so it moves all four
	// as a single access.
	constexpr int _swizzle_clang_width(length_t N, int E0, int E1, int E2, int E3)
	{
		return _swizzle_extent(N, E0, E1, E2, E3) <= 2 ? 2 : 4;
	}

	// Unaligned, aliasing-safe access to the source vector through the swizzle's _buffer.
	template<typename V>
	GLM_FUNC_QUALIFIER V _swizzle_clang_load(char const* p)
	{
		V v;
		__builtin_memcpy(&v, p, sizeof(V));
		return v;
	}

	template<typename V>
	GLM_FUNC_QUALIFIER void _swizzle_clang_store(char* p, V const& v)
	{
		__builtin_memcpy(p, &v, sizeof(V));
	}

	// The swizzled lanes of v, in swizzle order. Older Clang (e.g. 18) gives __builtin_shufflevector a
	// vector type distinct from the ext_vector alias, so convert the results explicitly.
	template<typename T, length_t N, int E0, int E1, int E2, int E3, typename V>
	GLM_FUNC_QUALIFIER _swizzle_clang_vec<T, N> _swizzle_clang_gather(V const& v)
	{
		if constexpr (N == 2)
			return (_swizzle_clang_vec<T, N>)__builtin_shufflevector(v, v, E0, E1);
		else if constexpr (N == 3)
			return (_swizzle_clang_vec<T, N>)__builtin_shufflevector(v, v, E0, E1, E2);
		else
			return (_swizzle_clang_vec<T, N>)__builtin_shufflevector(v, v, E0, E1, E2, E3);
	}

	// Lane J of the write-back shuffle: the new value from swizzle lane i (offset by Width, i.e. taken
	// from the second operand) if the swizzle writes lane J, otherwise the current value of lane J.
	constexpr int _swizzle_clang_scatter(length_t N, int E0, int E1, int E2, int E3, int Width, int J)
	{
		int const E[4] = {E0, E1, E2, E3};
		for (int i = 0; i < static_cast<int>(N); ++i)
			if (E[i] == J)
				return Width + i;
		return J;
	}

	template<length_t N, typename T, int E0, int E1, int E2, int E3, typename V, typename Op, int... J>
	GLM_FUNC_QUALIFIER void _swizzle_clang_apply_impl(char* p, _swizzle_clang_vec<T, N> const& that, Op const& op, std::integer_sequence<int, J...>)
	{
		V dst = _swizzle_clang_load<V>(p);

		// Read the swizzled components before writing any, matching the scalar path's semantics.
		_swizzle_clang_vec<T, N> const value = op(_swizzle_clang_gather<T, N, E0, E1, E2, E3>(dst), that);

		// Widen the result to the access width (the padding lanes are never selected below), then merge
		// it into the lanes the swizzle targets. Writable swizzles have no repeated indices.
		V const wide = (V)__builtin_shufflevector(value, value, (J < static_cast<int>(N) ? J : -1)...);
		dst = (V)__builtin_shufflevector(dst, wide, _swizzle_clang_scatter(N, E0, E1, E2, E3, sizeof...(J), J)...);

		_swizzle_clang_store(p, dst);
	}

	// Apply op(current, that) to the components selected by a writable swizzle over the vector at p.
	template<length_t N, typename T, int E0, int E1, int E2, int E3, typename S, typename Op>
	GLM_FUNC_QUALIFIER void _swizzle_clang_apply(char* p, S const& that, Op const& op)
	{
		constexpr int Width = _swizzle_clang_width(N, E0, E1, E2, E3);
		// An aligned vec3's storage has a 4th (padding) lane; keep only the swizzle's N.
		_swizzle_clang_vec<T, N> value;
		if constexpr (sizeof(S) == sizeof(_swizzle_clang_vec<T, N>) && N != 3)
			value = that;
		else if constexpr (N == 3)
			value = __builtin_shufflevector(that, that, 0, 1, 2);
		else
			value = __builtin_shufflevector(that, that, 0, 1);
		_swizzle_clang_apply_impl<N, T, E0, E1, E2, E3, _swizzle_clang_vec<T, Width>>(
			p, value, op, std::make_integer_sequence<int, Width>());
	}

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	template<length_t N, typename T, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<N, T, Q, E0, E1, E2, E3, true> : public _swizzle_base0<T, N>
	{
		GLM_FUNC_QUALIFIER vec<N, T, Q> operator ()()  const
		{
			using V = _swizzle_clang_vec<T, _swizzle_clang_width(N, E0, E1, E2, E3)>;

			vec<N, T, Q> Result;
			_swizzle_clang_vec<T, N> const g = _swizzle_clang_gather<T, N, E0, E1, E2, E3>(_swizzle_clang_load<V>(this->_buffer));
			if constexpr (N == 3)
				Result.data = __builtin_shufflevector(g, g, 0, 1, 2, 2); // 4-lane vec3 storage
			else
				Result.data = g;
			return Result;
		}
	};

	// Aligned bool vectors are stored as plain arrays rather than ext_vectors.
	template<length_t N, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<N, bool, Q, E0, E1, E2, E3, true> : public _swizzle_scalar<N, bool, Q, E0, E1, E2, E3>
	{};
#	endif//GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
}//namespace detail
}//namespace glm
#endif//GLM_ARCH & GLM_ARCH_CLANG_BIT

#if (GLM_ARCH & GLM_ARCH_SSE2_BIT) && (GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR)
namespace glm{
namespace detail
{
	// Only swizzles reaching z or w are vectorized: their source is an aligned vec3/vec4, whose storage
	// is a full __m128/__m128i. Narrower ones may come from an 8-byte vec2 and stay scalar. Two-component
	// results are always scalar, since aligned vec2 storage isn't a SIMD register here.

	template<length_t L, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<L, float, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<L, float, Q, E0,E1,E2,E3>
	{
		GLM_FUNC_QUALIFIER vec<L, float, Q> operator ()()  const
		{
			if constexpr (_swizzle_extent(L, E0, E1, E2, E3) <= 2)
				return _swizzle_scalar<L, float, Q, E0,E1,E2,E3>::operator()();
			else
			{
				__m128 data = *reinterpret_cast<__m128 const*>(&this->_buffer);

				vec<L, float, Q> Result;
#				if GLM_ARCH & GLM_ARCH_AVX_BIT
					Result.data = _mm_permute_ps(data, _MM_SHUFFLE(E3, E2, E1, E0));
#				else
					Result.data = _mm_shuffle_ps(data, data, _MM_SHUFFLE(E3, E2, E1, E0));
#				endif
				return Result;
			}
		}
	};

	template<length_t L, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<L, int, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<L, int, Q, E0,E1,E2,E3>
	{
		GLM_FUNC_QUALIFIER vec<L, int, Q> operator ()()  const
		{
			if constexpr (_swizzle_extent(L, E0, E1, E2, E3) <= 2)
				return _swizzle_scalar<L, int, Q, E0,E1,E2,E3>::operator()();
			else
			{
				__m128i data = *reinterpret_cast<__m128i const*>(&this->_buffer);

				vec<L, int, Q> Result;
				Result.data = _mm_shuffle_epi32(data, _MM_SHUFFLE(E3, E2, E1, E0));
				return Result;
			}
		}
	};

	template<length_t L, qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<L, uint, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<L, uint, Q, E0,E1,E2,E3>
	{
		GLM_FUNC_QUALIFIER vec<L, uint, Q> operator ()()  const
		{
			if constexpr (_swizzle_extent(L, E0, E1, E2, E3) <= 2)
				return _swizzle_scalar<L, uint, Q, E0,E1,E2,E3>::operator()();
			else
			{
				__m128i data = *reinterpret_cast<__m128i const*>(&this->_buffer);

				vec<L, uint, Q> Result;
				Result.data = _mm_shuffle_epi32(data, _MM_SHUFFLE(E3, E2, E1, E0));
				return Result;
			}
		}
	};

	template<qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<2, float, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<2, float, Q, E0,E1,E2,E3> {};

	template<qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<2, int, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<2, int, Q, E0,E1,E2,E3> {};

	template<qualifier Q, int E0, int E1, int E2, int E3>
	struct _swizzle_base1<2, uint, Q, E0,E1,E2,E3, true> : public _swizzle_scalar<2, uint, Q, E0,E1,E2,E3> {};
}//namespace detail
}//namespace glm
#endif//(GLM_ARCH & GLM_ARCH_SSE2_BIT) && (GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR)

// NEON has no specializations yet: swizzles use the portable implementation.
