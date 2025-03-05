/// @ref core

#include "_vectorize.hpp"
#if(GLM_ARCH & GLM_ARCH_X86 && GLM_COMPILER & GLM_COMPILER_VC)
#	include <intrin.h>
#	pragma intrinsic(_BitScanReverse)
#endif//(GLM_ARCH & GLM_ARCH_X86 && GLM_COMPILER & GLM_COMPILER_VC)
#include <cstring>
#include <limits>

#if !GLM_HAS_EXTENDED_INTEGER_TYPE
#	if GLM_COMPILER & GLM_COMPILER_GCC
#		pragma GCC diagnostic push
#		pragma GCC diagnostic ignored "-Wlong-long"
#	endif
#	if (GLM_COMPILER & GLM_COMPILER_CLANG)
#		pragma clang diagnostic push
#		pragma clang diagnostic ignored "-Wc++11-long-long"
#	endif
#endif

namespace glm{
namespace detail
{
	template<typename T>
	GLM_FUNC_QUALIFIER T mask(T Bits)
	{
		return Bits >= static_cast<T>(sizeof(T) * 8) ? ~static_cast<T>(0) : (static_cast<T>(1) << Bits) - static_cast<T>(1);
	}

	template<typename genIUType, size_t Bits>
	struct compute_findLSB
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			if(Value == 0)
				return -1;

			genIUType bitIndex = static_cast<genIUType>(0);
			while ((Value & 1) == 0) {
				Value >>= 1;
				bitIndex++;
			}
			return bitIndex;
		}
	};

	template<typename genIUType, size_t Bits>
	struct compute_findMSB
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			if(Value == 0)
				return -1;

			genIUType bitIndex = static_cast<genIUType>(Bits);
			do {
				bitIndex--;
			} while (((Value >> bitIndex) & 1) == 0);
			return static_cast<int>(bitIndex);
		}
	};

#if GLM_HAS_BUILTIN(__builtin_ctz)
	template<typename genIUType>
	struct compute_findLSB<genIUType, 32>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			return (Value != static_cast<genIUType>(0)) ? __builtin_ctz(Value) : -1;
		}
	};
#endif

#if GLM_HAS_BUILTIN(__builtin_ctzll)
	template<typename genIUType>
	struct compute_findLSB<genIUType, 64>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			return (Value != static_cast<genIUType>(0)) ? __builtin_ctzll(Value) : -1;
		}
	};
#endif

#if GLM_HAS_BUILTIN(__builtin_clz)
	template<typename genIUType>
	struct compute_findMSB<genIUType, 32>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			int const count = Value ? __builtin_clz(Value) : 32;
			return 31 - count;
		}
	};
#endif

#if GLM_HAS_BUILTIN(__builtin_clzll)
	template<typename genIUType>
	struct compute_findMSB<genIUType, 64>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			int const count = Value ? __builtin_clzll(Value) : 64;
			return 63 - count;
		}
	};
#endif

#if GLM_HAS_BITSCAN_WINDOWS
	template<typename genIUType>
	struct compute_findLSB<genIUType, 32>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			unsigned long Result(0);
			unsigned char IsNotNull = _BitScanForward(&Result, *reinterpret_cast<unsigned long*>(&Value));
			return IsNotNull ? int(Result) : -1;
		}
	};

#if !((GLM_COMPILER & GLM_COMPILER_VC) && (GLM_MODEL == GLM_MODEL_32))
	template<typename genIUType>
	struct compute_findLSB<genIUType, 64>
	{
		GLM_FUNC_QUALIFIER static int call(genIUType Value)
		{
			unsigned long Result(0);
			unsigned char IsNotNull = _BitScanForward64(&Result, *reinterpret_cast<unsigned __int64*>(&Value));
			return IsNotNull ? int(Result) : -1;
		}
	};
#endif
#endif//GLM_HAS_BITSCAN_WINDOWS
}//namespace detail

	// uaddCarry
	GLM_FUNC_QUALIFIER uint uaddCarry(uint const& x, uint const& y, uint & Carry)
	{
		detail::uint64 const Value64(static_cast<detail::uint64>(x) + static_cast<detail::uint64>(y));
		detail::uint64 const Max32((static_cast<detail::uint64>(1) << static_cast<detail::uint64>(32)) - static_cast<detail::uint64>(1));
		Carry = Value64 > Max32 ? 1u : 0u;
		return static_cast<uint>(Value64 % (Max32 + static_cast<detail::uint64>(1)));
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, uint, Q> uaddCarry(vec<L, uint, Q> const& x, vec<L, uint, Q> const& y, vec<L, uint, Q>& Carry)
	{
		vec<L, detail::uint64, Q> Value64(vec<L, detail::uint64, Q>(x) + vec<L, detail::uint64, Q>(y));
		vec<L, detail::uint64, Q> Max32((static_cast<detail::uint64>(1) << static_cast<detail::uint64>(32)) - static_cast<detail::uint64>(1));
		Carry = mix(vec<L, uint, Q>(0), vec<L, uint, Q>(1), greaterThan(Value64, Max32));
		return vec<L, uint, Q>(Value64 % (Max32 + static_cast<detail::uint64>(1)));
	}

	// usubBorrow
	GLM_FUNC_QUALIFIER uint usubBorrow(uint const& x, uint const& y, uint & Borrow)
	{
		Borrow = x >= y ? static_cast<uint>(0) : static_cast<uint>(1);
		if(y >= x)
			return y - x;
		else
			return static_cast<uint>((static_cast<detail::int64>(1) << static_cast<detail::int64>(32)) + (static_cast<detail::int64>(y) - static_cast<detail::int64>(x)));
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, uint, Q> usubBorrow(vec<L, uint, Q> const& x, vec<L, uint, Q> const& y, vec<L, uint, Q>& Borrow)
	{
		Borrow = mix(vec<L, uint, Q>(1), vec<L, uint, Q>(0), greaterThanEqual(x, y));
		vec<L, uint, Q> const YgeX(y - x);
		vec<L, uint, Q> const XgeY(vec<L, uint, Q>((static_cast<detail::int64>(1) << static_cast<detail::int64>(32)) + (vec<L, detail::int64, Q>(y) - vec<L, detail::int64, Q>(x))));
		return mix(XgeY, YgeX, greaterThanEqual(y, x));
	}

	// umulExtended
	GLM_FUNC_QUALIFIER void umulExtended(uint const& x, uint const& y, uint & msb, uint & lsb)
	{
		detail::uint64 Value64 = static_cast<detail::uint64>(x) * static_cast<detail::uint64>(y);
		msb = static_cast<uint>(Value64 >> static_cast<detail::uint64>(32));
		lsb = static_cast<uint>(Value64);
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER void umulExtended(vec<L, uint, Q> const& x, vec<L, uint, Q> const& y, vec<L, uint, Q>& msb, vec<L, uint, Q>& lsb)
	{
		vec<L, detail::uint64, Q> Value64(vec<L, detail::uint64, Q>(x) * vec<L, detail::uint64, Q>(y));
		msb = vec<L, uint, Q>(Value64 >> static_cast<detail::uint64>(32));
		lsb = vec<L, uint, Q>(Value64);
	}

	// imulExtended
	GLM_FUNC_QUALIFIER void imulExtended(int x, int y, int& msb, int& lsb)
	{
		detail::int64 Value64 = static_cast<detail::int64>(x) * static_cast<detail::int64>(y);
		msb = static_cast<int>(Value64 >> static_cast<detail::int64>(32));
		lsb = static_cast<int>(Value64);
	}

	template<length_t L, qualifier Q>
	GLM_FUNC_QUALIFIER void imulExtended(vec<L, int, Q> const& x, vec<L, int, Q> const& y, vec<L, int, Q>& msb, vec<L, int, Q>& lsb)
	{
		vec<L, detail::int64, Q> Value64(vec<L, detail::int64, Q>(x) * vec<L, detail::int64, Q>(y));
		lsb = vec<L, int, Q>(Value64 & static_cast<detail::int64>(0xFFFFFFFF));
		msb = vec<L, int, Q>((Value64 >> static_cast<detail::int64>(32)) & static_cast<detail::int64>(0xFFFFFFFF));
	}

	// bitfieldExtract
	template<typename genIUType>
	GLM_FUNC_QUALIFIER genIUType bitfieldExtract(genIUType Value, int Offset, int Bits)
	{
		return bitfieldExtract(vec<1, genIUType>(Value), Offset, Bits).x;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> bitfieldExtract(vec<L, T, Q> const& Value, int Offset, int Bits)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitfieldExtract' only accept integer inputs");

		return (Value >> static_cast<T>(Offset)) & static_cast<T>(detail::mask(Bits));
	}

	// bitfieldInsert
	template<typename genIUType>
	GLM_FUNC_QUALIFIER genIUType bitfieldInsert(genIUType const& Base, genIUType const& Insert, int Offset, int Bits)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'bitfieldInsert' only accept integer values");

		return bitfieldInsert(vec<1, genIUType>(Base), vec<1, genIUType>(Insert), Offset, Bits).x;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> bitfieldInsert(vec<L, T, Q> const& Base, vec<L, T, Q> const& Insert, int Offset, int Bits)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitfieldInsert' only accept integer values");

		T const Mask = detail::mask(static_cast<T>(Bits)) << Offset;
		return (Base & ~Mask) | ((Insert << static_cast<T>(Offset)) & Mask);
	}

	// bitfieldReverse
	template<typename genIType>
	GLM_FUNC_QUALIFIER genIType bitfieldReverse(genIType sx)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIType>::is_integer, "'bitfieldReverse' only accepts integer values");

		using UnsignedType = typename std::make_unsigned<genIType>::type;
		static_assert(sizeof(UnsignedType) == sizeof(genIType), "Unsigned type must have the same size");

		// Preserve bit representation via memcpy (bit_cast equivalent)
		UnsignedType x;
		std::memcpy(&x, &sx, sizeof(genIType));

		UnsignedType result = 0;
		int numBits = std::numeric_limits<genIType>::digits; // Excludes sign bit for signed types

		for (int i = 0; i < numBits; ++i)
		{
			result = (result << 1) | (x & 1);
			x >>= 1;
		}

		// Convert back to original type safely
		genIType finalResult;
		std::memcpy(&finalResult, &result, sizeof(genIType));
		return finalResult;
	}


	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> bitfieldReverse(vec<L, T, Q> const& v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitfieldReverse' only accept integer values");

		vec<L, typename detail::make_unsigned<T>::type, Q> x(v);
		vec<L, T, Q> r;
		#if GLM_COMPILER & GLM_COMPILER_CLANG
		#pragma unroll
		#endif
		for (uint i = 0; i < L; ++i)
			r[i] = bitfieldReverse(x[i]);
		return r;
	}

	// bitCount
	template<typename genIType>
	GLM_FUNC_QUALIFIER int bitCount(genIType sx)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIType>::is_integer, "'bitCount' only accepts integer values");

		using genIUType = typename std::make_unsigned<genIType>::type;

		// Bit-cast using memcpy for safety in C++11
		genIUType x;
		std::memcpy(&x, &sx, sizeof(genIUType));

		int c = 0;
		while (x) {
			x &= (x - 1);
			c++;
		}
		return c;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> bitCount(vec<L, T, Q> const& v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		return detail::functor1<vec, L, int, typename detail::make_unsigned<T>::type, Q>::call(bitCount, v);
	}

	// findLSB
	template<typename genIUType>
	GLM_FUNC_QUALIFIER int findLSB(genIUType Value)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findLSB' only accept integer values");

		return detail::compute_findLSB<genIUType, sizeof(genIUType) * 8>::call(Value);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> findLSB(vec<L, T, Q> const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'findLSB' only accept integer values");

		return detail::functor1<vec, L, int, T, Q>::call(findLSB, x);
	}

	// findMSB
	template<typename genIUType>
	GLM_FUNC_QUALIFIER int findMSB(genIUType Value)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");

		return detail::compute_findMSB<genIUType, sizeof(genIUType) * 8>::call(Value);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, int, Q> findMSB(vec<L, T, Q> const& x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'findMSB' only accept integer values");

		return detail::functor1<vec, L, int, T, Q>::call(findMSB, x);
	}
}//namespace glm

#if !GLM_HAS_EXTENDED_INTEGER_TYPE
#	if GLM_COMPILER & GLM_COMPILER_GCC
#		pragma GCC diagnostic pop
#	endif
#	if (GLM_COMPILER & GLM_COMPILER_CLANG)
#		pragma clang diagnostic pop
#	endif
#endif

#if GLM_CONFIG_SIMD == GLM_ENABLE
#	include "func_integer_simd.inl"
#endif

