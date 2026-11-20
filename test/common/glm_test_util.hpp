// Helpers shared by GLM tests. Unlike glm_test.hpp this includes GLM, so include it after the test's
// own GLM_FORCE_* defines and GLM includes.

#pragma once

#include "glm_test.hpp"

#include <glm/glm.hpp>

#include <cstring>
#include <limits>
#include <random>
#include <type_traits>

namespace glm_test
{
	inline char const* qualifier_name(glm::qualifier Q)
	{
		switch (static_cast<int>(Q))
		{
		case glm::packed_highp: return "packed_highp";
		case glm::packed_mediump: return "packed_mediump";
		case glm::packed_lowp: return "packed_lowp";
#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
		case glm::aligned_highp: return "aligned_highp";
		case glm::aligned_mediump: return "aligned_mediump";
		case glm::aligned_lowp: return "aligned_lowp";
#	endif
		default: return "unknown qualifier";
		}
	}

	// Deterministic random numbers: every test run sees the same sequence.
	inline std::mt19937& rng()
	{
		static std::mt19937 Generator(20261119u);
		return Generator;
	}

	template<typename T>
	T random_value(double Lo, double Hi)
	{
		if constexpr (std::is_floating_point<T>::value)
			return static_cast<T>(std::uniform_real_distribution<double>(Lo, Hi)(rng()));
		else
			return static_cast<T>(std::uniform_int_distribution<long long>(static_cast<long long>(Lo), static_cast<long long>(Hi))(rng()));
	}

	// A vector of random components in [Lo, Hi], none of them zero if NonZero is set.
	template<glm::length_t L, typename T, glm::qualifier Q = glm::packed_highp>
	glm::vec<L, T, Q> random_vec(double Lo, double Hi, bool NonZero = false)
	{
		glm::vec<L, T, Q> Result;
		for (glm::length_t i = 0; i < L; ++i)
		{
			do
				Result[i] = random_value<T>(Lo, Hi);
			while (NonZero && Result[i] == T(0));
		}
		return Result;
	}

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q = glm::packed_highp>
	glm::mat<C, R, T, Q> random_mat(double Lo, double Hi)
	{
		glm::mat<C, R, T, Q> Result;
		for (glm::length_t c = 0; c < C; ++c)
			Result[c] = random_vec<R, T, Q>(Lo, Hi);
		return Result;
	}

	// The vector converted to qualifier Q, with any padding (an aligned vec3's 4th lane) overwritten
	// with garbage: NaN for floating point, 0xA5 bytes otherwise. SIMD paths must ignore the padding.
	template<glm::qualifier Q, glm::length_t L, typename T, glm::qualifier P>
	glm::vec<L, T, Q> to_aligned_dirty(glm::vec<L, T, P> const& v)
	{
		using A = glm::vec<L, T, Q>;
		static_assert(std::is_trivially_copyable<A>::value, "vectors are trivially copyable");
		// Convert normally, then overwrite only the padding bytes. Building the whole vector from a byte
		// buffer instead made GCC 13 route part of a 12-byte copy through an MMX register without EMMS,
		// which poisons later x87 (long double) math such as the standard library's random numbers.
		A Result(v);
		constexpr std::size_t Used = sizeof(T) * L;
		if constexpr (sizeof(A) > Used)
		{
			unsigned char Garbage[sizeof(A) - Used];
			if constexpr (std::is_floating_point<T>::value)
			{
				T const Nan = std::numeric_limits<T>::quiet_NaN();
				for (std::size_t i = 0; i + sizeof(T) <= sizeof(Garbage); i += sizeof(T))
					std::memcpy(Garbage + i, &Nan, sizeof(T));
			}
			else
				std::memset(Garbage, 0xA5, sizeof(Garbage));
			std::memcpy(reinterpret_cast<unsigned char*>(&Result) + Used, Garbage, sizeof(Garbage));
		}
		return Result;
	}

	template<glm::qualifier Q, glm::length_t C, glm::length_t R, typename T, glm::qualifier P>
	glm::mat<C, R, T, Q> to_aligned_dirty(glm::mat<C, R, T, P> const& m)
	{
		glm::mat<C, R, T, Q> Result;
		for (glm::length_t c = 0; c < C; ++c)
			Result[c] = to_aligned_dirty<Q>(m[c]);
		return Result;
	}
}//namespace glm_test
