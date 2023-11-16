#include <glm/integer.hpp>
#include <glm/vector_relational.hpp>
#include <glm/ext/vector_int1.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/ext/vector_uint1.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <glm/ext/vector_uint3.hpp>
#include <glm/ext/vector_uint4.hpp>
#include <glm/ext/scalar_int_sized.hpp>
#include <glm/ext/scalar_uint_sized.hpp>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

enum result
{
	SUCCESS,
	FAIL,
	ASSERT,
	STATIC_ASSERT
};

// Helper for detailed logging
#define LOG_TEST_START(test_name) \
    std::cout << "\n=== RUNNING TEST: " << test_name << " ===" << std::endl

#define LOG_TEST_RESULT(test_name, error_count) \
    std::cout << "=== TEST " << test_name << ": " << \
    (error_count > 0 ? "FAILED with " : "PASSED with ") << \
    error_count << (error_count == 1 ? " error" : " errors") << " ===" << std::endl

#define LOG_ERROR(expected, actual, message) \
    std::cout << "ERROR: " << message << "\n" \
    << "  Expected: " << expected << "\n" \
    << "  Actual:   " << actual << std::endl

// Helper for printing hex values
template<typename T>
std::string toHexString(T value) {
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::hex << std::setw(sizeof(T)*2) << std::setfill('0') << value;
    return ss.str();
}

namespace bitfieldInsert
{
	template<typename genType>
	struct type
	{
		genType		Base;
		genType		Insert;
		int			Offset;
		int			Bits;
		genType		Return;
	};

	typedef type<glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0x00000000, 0xffffffff,  0, 32, 0xffffffff},
		{0x00000000, 0xffffffff,  0, 31, 0x7fffffff},
		{0x00000000, 0xffffffff,  0,  0, 0x00000000},
		{0xff000000, 0x000000ff,  8,  8, 0xff00ff00},
		{0xffff0000, 0xffff0000, 16, 16, 0x00000000},
		{0x0000ffff, 0x0000ffff, 16, 16, 0xffffffff}
	};

	static int test()
	{
		LOG_TEST_START("bitfieldInsert");
		int Error = 0;
		glm::uint count = sizeof(Data32) / sizeof(typeU32);

		for(glm::uint i = 0; i < count; ++i)
		{
			glm::uint Return = glm::bitfieldInsert(
				Data32[i].Base,
				Data32[i].Insert,
				Data32[i].Offset,
				Data32[i].Bits);

			bool Success = (Data32[i].Return == Return);
            if (!Success) {
                LOG_ERROR(toHexString(Data32[i].Return), toHexString(Return),
                          "bitfieldInsert failed for case " + std::to_string(i) + ":\n"
                          "  Base: " + toHexString(Data32[i].Base) + "\n" +
                          "  Insert: " + toHexString(Data32[i].Insert) + "\n" +
                          "  Offset: " + std::to_string(Data32[i].Offset) + "\n" +
                          "  Bits: " + std::to_string(Data32[i].Bits));
                Error++;
            }
		}

		LOG_TEST_RESULT("bitfieldInsert", Error);
		return Error;
	}
}//bitfieldInsert

namespace bitfieldExtract
{
	template<typename genType>
	struct type
	{
		genType		Value;
		int			Offset;
		int			Bits;
		genType		Return;
		result		Result;
	};

	typedef type<glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0xffffffff, 0,32, 0xffffffff, SUCCESS},
		{0xffffffff, 8, 0, 0x00000000, SUCCESS},
		{0x00000000, 0,32, 0x00000000, SUCCESS},
		{0x0f0f0f0f, 0,32, 0x0f0f0f0f, SUCCESS},
		{0x00000000, 8, 0, 0x00000000, SUCCESS},
		{0x80000000,31, 1, 0x00000001, SUCCESS},
		{0x7fffffff,31, 1, 0x00000000, SUCCESS},
		{0x00000300, 8, 8, 0x00000003, SUCCESS},
		{0x0000ff00, 8, 8, 0x000000ff, SUCCESS},
		{0xfffffff0, 0, 5, 0x00000010, SUCCESS},
		{0x000000ff, 1, 3, 0x00000007, SUCCESS},
		{0x000000ff, 0, 3, 0x00000007, SUCCESS},
		{0x00000000, 0, 2, 0x00000000, SUCCESS},
		{0xffffffff, 0, 8, 0x000000ff, SUCCESS},
		{0xffff0000,16,16, 0x0000ffff, SUCCESS},
		{0xfffffff0, 0, 8, 0x00000000, FAIL},
		{0xffffffff,16,16, 0x00000000, FAIL},
		//{0xffffffff,32, 1, 0x00000000, ASSERT}, // Throw an assert
		//{0xffffffff, 0,33, 0x00000000, ASSERT}, // Throw an assert
		//{0xffffffff,16,16, 0x00000000, ASSERT}, // Throw an assert
	};

	static int test()
	{
		LOG_TEST_START("bitfieldExtract");
		int Error = 0;

		glm::uint count = sizeof(Data32) / sizeof(typeU32);

		for(glm::uint i = 0; i < count; ++i)
		{
			glm::uint Return = glm::bitfieldExtract(
				Data32[i].Value,
				Data32[i].Offset,
				Data32[i].Bits);

			bool Compare = Data32[i].Return == Return;

            // For SUCCESS cases, Compare should be true
            // For FAIL cases, Compare should be false
            bool TestPassed = (Data32[i].Result == SUCCESS && Compare) ||
                              (Data32[i].Result == FAIL && !Compare);

			if (!TestPassed) {
                std::string expectedResult = (Data32[i].Result == SUCCESS) ? "match" : "not match";
                LOG_ERROR(expectedResult, (Compare ? "match" : "not match"),
                          "bitfieldExtract case " + std::to_string(i) + ":\n"
                          "  Value: " + toHexString(Data32[i].Value) + "\n" +
                          "  Offset: " + std::to_string(Data32[i].Offset) + "\n" +
                          "  Bits: " + std::to_string(Data32[i].Bits) + "\n" +
                          "  Expected: " + toHexString(Data32[i].Return) + "\n" +
                          "  Expected result: " + (Data32[i].Result == SUCCESS ? "SUCCESS" : "FAIL"));
                Error++;
            }
		}

		LOG_TEST_RESULT("bitfieldExtract", Error);
		return Error;
	}
}//extractField

namespace bitfieldReverse
{
	template<glm::length_t L, typename T, glm::qualifier Q>
	GLM_FUNC_QUALIFIER glm::vec<L, T, Q> bitfieldReverseLoop(glm::vec<L, T, Q> const& v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitfieldReverse' only accept integer values");

		glm::vec<L, T, Q> Result(0);
		T const BitSize = static_cast<T>(sizeof(T) * 8);
		for(T i = 0; i < BitSize; ++i)
		{
			glm::vec<L, T, Q> const BitSet(v & (static_cast<T>(1) << i));
			glm::vec<L, T, Q> const BitFirst(BitSet >> i);
			Result |= BitFirst << (BitSize - 1 - i);
		}
		return Result;
	}

	template<typename T>
	GLM_FUNC_QUALIFIER T bitfieldReverseLoop(T v)
	{
		return bitfieldReverseLoop(glm::vec<1, T>(v)).x;
	}

	GLM_FUNC_QUALIFIER glm::uint32 bitfieldReverseUint32(glm::uint32 x)
	{
		x = (x & 0x55555555) <<  1 | (x & 0xAAAAAAAA) >>  1;
		x = (x & 0x33333333) <<  2 | (x & 0xCCCCCCCC) >>  2;
		x = (x & 0x0F0F0F0F) <<  4 | (x & 0xF0F0F0F0) >>  4;
		x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
		x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
		return x;
	}

	GLM_FUNC_QUALIFIER glm::uint64 bitfieldReverseUint64(glm::uint64 x)
	{
		x = (x & 0x5555555555555555) <<  1 | (x & 0xAAAAAAAAAAAAAAAA) >>  1;
		x = (x & 0x3333333333333333) <<  2 | (x & 0xCCCCCCCCCCCCCCCC) >>  2;
		x = (x & 0x0F0F0F0F0F0F0F0F) <<  4 | (x & 0xF0F0F0F0F0F0F0F0) >>  4;
		x = (x & 0x00FF00FF00FF00FF) <<  8 | (x & 0xFF00FF00FF00FF00) >>  8;
		x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
		x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
		return x;
	}

	template<bool EXEC = false>
	struct compute_bitfieldReverseStep
	{
		template<glm::length_t L, typename T, glm::qualifier Q>
		GLM_FUNC_QUALIFIER static glm::vec<L, T, Q> call(glm::vec<L, T, Q> const& v, T, T)
		{
			return v;
		}
	};

	template<>
	struct compute_bitfieldReverseStep<true>
	{
		template<glm::length_t L, typename T, glm::qualifier Q>
		GLM_FUNC_QUALIFIER static glm::vec<L, T, Q> call(glm::vec<L, T, Q> const& v, T Mask, T Shift)
		{
			return (v & Mask) << Shift | (v & (~Mask)) >> Shift;
		}
	};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(push)
#			pragma warning(disable : 4309)
#		endif

	template<glm::length_t L, typename T, glm::qualifier Q>
	GLM_FUNC_QUALIFIER glm::vec<L, T, Q> bitfieldReverseOps(glm::vec<L, T, Q> const& v)
	{
		glm::vec<L, T, Q> x(v);
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  2>::call(x, static_cast<T>(0x5555555555555555ull), static_cast<T>( 1));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  4>::call(x, static_cast<T>(0x3333333333333333ull), static_cast<T>( 2));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  8>::call(x, static_cast<T>(0x0F0F0F0F0F0F0F0Full), static_cast<T>( 4));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 16>::call(x, static_cast<T>(0x00FF00FF00FF00FFull), static_cast<T>( 8));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 32>::call(x, static_cast<T>(0x0000FFFF0000FFFFull), static_cast<T>(16));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 64>::call(x, static_cast<T>(0x00000000FFFFFFFFull), static_cast<T>(32));
		return x;
	}

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(pop)
#		endif

	template<typename genType>
	GLM_FUNC_QUALIFIER genType bitfieldReverseOps(genType x)
	{
		return bitfieldReverseOps(glm::vec<1, genType, glm::defaultp>(x)).x;
	}

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

	template<typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
		result		Result;
	};

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

	typedef type<glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0x00000001, 0x80000000, SUCCESS},
		{0x0000000f, 0xf0000000, SUCCESS},
		{0x000000ff, 0xff000000, SUCCESS},
		{0xf0000000, 0x0000000f, SUCCESS},
		{0xff000000, 0x000000ff, SUCCESS},
		{0xffffffff, 0xffffffff, SUCCESS},
		{0x00000000, 0x00000000, SUCCESS}
	};

	typedef type<glm::uint64> typeU64;

	typeU64 const Data64[] =
	{
		{0x00000000000000ff, 0xff00000000000000, SUCCESS},
		{0x000000000000000f, 0xf000000000000000, SUCCESS},
		{0xf000000000000000, 0x000000000000000f, SUCCESS},
		{0xffffffffffffffff, 0xffffffffffffffff, SUCCESS},
		{0x0000000000000000, 0x0000000000000000, SUCCESS}
	};

	static int test32_bitfieldReverse()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = glm::bitfieldReverse(Data32[i].Value);

			bool Compare = Data32[i].Return == Return;

			if((Data32[i].Result == SUCCESS && !Compare) || (Data32[i].Result != SUCCESS && Compare))
            {
                LOG_ERROR(toHexString(Data32[i].Return), toHexString(Return),
                          "test32_bitfieldReverse case " + std::to_string(i) + ":\n"
                          "  Value: " + toHexString(Data32[i].Value));
                Error++;
            }
		}

		return Error;
	}

	static int test32_bitfieldReverseLoop()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseLoop(Data32[i].Value);

			bool Compare = Data32[i].Return == Return;

			if((Data32[i].Result == SUCCESS && !Compare) || (Data32[i].Result != SUCCESS && Compare))
            {
                LOG_ERROR(toHexString(Data32[i].Return), toHexString(Return),
                          "test32_bitfieldReverseLoop case " + std::to_string(i) + ":\n"
                          "  Value: " + toHexString(Data32[i].Value));
                Error++;
            }
		}

		return Error;
	}

	static int test32_bitfieldReverseUint32()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseUint32(Data32[i].Value);

			bool Compare = Data32[i].Return == Return;

			if((Data32[i].Result == SUCCESS && !Compare) || (Data32[i].Result != SUCCESS && Compare))
            {
                LOG_ERROR(toHexString(Data32[i].Return), toHexString(Return),
                          "test32_bitfieldReverseUint32 case " + std::to_string(i) + ":\n"
                          "  Value: " + toHexString(Data32[i].Value));
                Error++;
            }
		}

		return Error;
	}

	static int test32_bitfieldReverseOps()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseOps(Data32[i].Value);

			bool Compare = Data32[i].Return == Return;

			if((Data32[i].Result == SUCCESS && !Compare) || (Data32[i].Result != SUCCESS && Compare))
            {
                LOG_ERROR(toHexString(Data32[i].Return), toHexString(Return),
                          "test32_bitfieldReverseOps case " + std::to_string(i) + ":\n"
                          "  Value: " + toHexString(Data32[i].Value));
                Error++;
            }
		}

		return Error;
	}

	static int test64_bitfieldReverse()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = glm::bitfieldReverse(Data64[i].Value);

			bool Compare = Data64[i].Return == Return;

			if((Data64[i].Result == SUCCESS && !Compare) || (Data64[i].Result != SUCCESS && Compare))
            {
                std::stringstream ss;
                ss << "test64_bitfieldReverse case " << i << ":\n"
                   << "  Value: 0x" << std::hex << Data64[i].Value;
                LOG_ERROR("0x" + std::to_string(Data64[i].Return), "0x" + std::to_string(Return), ss.str());
                Error++;
            }
		}

		return Error;
	}

	static int test64_bitfieldReverseLoop()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseLoop(Data64[i].Value);

			bool Compare = Data64[i].Return == Return;

			if((Data64[i].Result == SUCCESS && !Compare) || (Data64[i].Result != SUCCESS && Compare))
            {
                std::stringstream ss;
                ss << "test64_bitfieldReverseLoop case " << i << ":\n"
                   << "  Value: 0x" << std::hex << Data64[i].Value;
                LOG_ERROR("0x" + std::to_string(Data64[i].Return), "0x" + std::to_string(Return), ss.str());
                Error++;
            }
		}

		return Error;
	}

	static int test64_bitfieldReverseUint64()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseUint64(Data64[i].Value);

			bool Compare = Data64[i].Return == Return;

			if((Data64[i].Result == SUCCESS && !Compare) || (Data64[i].Result != SUCCESS && Compare))
            {
                std::stringstream ss;
                ss << "test64_bitfieldReverseUint64 case " << i << ":\n"
                   << "  Value: 0x" << std::hex << Data64[i].Value;
                LOG_ERROR("0x" + std::to_string(Data64[i].Return), "0x" + std::to_string(Return), ss.str());
                Error++;
            }
		}

		return Error;
	}

	static int test64_bitfieldReverseOps()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);

		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseOps(Data64[i].Value);

			bool Compare = Data64[i].Return == Return;

			if((Data64[i].Result == SUCCESS && !Compare) || (Data64[i].Result != SUCCESS && Compare))
            {
                std::stringstream ss;
                ss << "test64_bitfieldReverseOps case " << i << ":\n"
                   << "  Value: 0x" << std::hex << Data64[i].Value;
                LOG_ERROR("0x" + std::to_string(Data64[i].Return), "0x" + std::to_string(Return), ss.str());
                Error++;
            }
		}

		return Error;
	}

	static int test()
	{
		LOG_TEST_START("bitfieldReverse");
		int Error = 0;

		Error += test32_bitfieldReverse();
		Error += test32_bitfieldReverseLoop();
		Error += test32_bitfieldReverseUint32();
		Error += test32_bitfieldReverseOps();

		Error += test64_bitfieldReverse();
		Error += test64_bitfieldReverseLoop();
		Error += test64_bitfieldReverseUint64();
		Error += test64_bitfieldReverseOps();

		LOG_TEST_RESULT("bitfieldReverse", Error);
		return Error;
	}

	static int perf32(glm::uint32 Count)
	{
		int Error = 0;

		std::vector<glm::uint32> Data;
		Data.resize(static_cast<std::size_t>(Count));

		std::clock_t Timestamps0 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = glm::bitfieldReverse(k);

		std::clock_t Timestamps1 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseLoop(k);

		std::clock_t Timestamps2 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseUint32(k);

		std::clock_t Timestamps3 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseOps(k);

		std::clock_t Timestamps4 = std::clock();

		std::printf("glm::bitfieldReverse: %d clocks\n", static_cast<int>(Timestamps1 - Timestamps0));
		std::printf("bitfieldReverseLoop: %d clocks\n", static_cast<int>(Timestamps2 - Timestamps1));
		std::printf("bitfieldReverseUint32: %d clocks\n", static_cast<int>(Timestamps3 - Timestamps2));
		std::printf("bitfieldReverseOps: %d clocks\n", static_cast<int>(Timestamps4 - Timestamps3));

		return Error;
	}

	static int perf64(glm::uint64 Count)
	{
		int Error = 0;

		std::vector<glm::uint64> Data;
		Data.resize(static_cast<std::size_t>(Count));

		std::clock_t Timestamps0 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[static_cast<std::size_t>(k)] = glm::bitfieldReverse(k);

		std::clock_t Timestamps1 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[static_cast<std::size_t>(k)] = bitfieldReverseLoop<glm::uint64>(k);

		std::clock_t Timestamps2 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[static_cast<std::size_t>(k)] = bitfieldReverseUint64(k);

		std::clock_t Timestamps3 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[static_cast<std::size_t>(k)] = bitfieldReverseOps(k);

		std::clock_t Timestamps4 = std::clock();

		std::printf("glm::bitfieldReverse - 64: %d clocks\n", static_cast<int>(Timestamps1 - Timestamps0));
		std::printf("bitfieldReverseLoop - 64: %d clocks\n", static_cast<int>(Timestamps2 - Timestamps1));
		std::printf("bitfieldReverseUint - 64: %d clocks\n", static_cast<int>(Timestamps3 - Timestamps2));
		std::printf("bitfieldReverseOps - 64: %d clocks\n", static_cast<int>(Timestamps4 - Timestamps3));

		return Error;
	}

	static int perf(std::size_t Samples)
	{
		int Error = 0;

		Error += perf32(static_cast<glm::uint32>(Samples));
		Error += perf64(static_cast<glm::uint64>(Samples));

		return Error;
	}
}//bitfieldReverse

namespace findMSB
{
	template<typename genType, typename retType>
	struct type
	{
		genType		Value;
		retType		Return;
	};

#	if GLM_HAS_BITSCAN_WINDOWS
		template<typename genIUType>
		static int findMSB_intrinsic(genIUType Value)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");

			if(Value == 0)
				return -1;

			unsigned long Result(0);
			_BitScanReverse(&Result, Value);
			return int(Result);
		}
#	endif//GLM_HAS_BITSCAN_WINDOWS

#	if GLM_ARCH & GLM_ARCH_AVX && GLM_COMPILER & GLM_COMPILER_VC
		template<typename genIUType>
		static int findMSB_avx(genIUType Value)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");

			if(Value == 0)
				return -1;

			return int(_tzcnt_u32(Value));
		}
#	endif//GLM_ARCH & GLM_ARCH_AVX && GLM_PLATFORM & GLM_PLATFORM_WINDOWS

	template<typename genIUType>
	static int findMSB_095(genIUType Value)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");
		
		if(Value == genIUType(0) || Value == genIUType(-1))
			return -1;
		else if(Value > 0)
		{
			genIUType Bit = genIUType(-1);
			for(genIUType tmp = Value; tmp > 0; tmp >>= 1, ++Bit){}
			return static_cast<int>(Bit);
		}
		else //if(Value < 0)
		{
			int const BitCount(sizeof(genIUType) * 8);
			int MostSignificantBit(-1);
			for(int BitIndex(0); BitIndex < BitCount; ++BitIndex)
				MostSignificantBit = (Value & (1 << BitIndex)) ? MostSignificantBit : BitIndex;
			assert(MostSignificantBit >= 0);
			return MostSignificantBit;
		}
	}

	template<typename genIUType>
	static int findMSB_nlz1(genIUType x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");

		if (x == 0)
			return -1;

		int n = 0;
		if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
		if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
		if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
		if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
		if (x <= 0x7FFFFFFF) {n = n + 1;}
		return 31 - n;
	}

	static int findMSB_nlz2(unsigned int x)
	{
		unsigned int y;
		int n = 32;

		y = x >>16;  if (y != 0) {n = n -16;  x = y;}
		y = x >> 8;  if (y != 0) {n = n - 8;  x = y;}
		y = x >> 4;  if (y != 0) {n = n - 4;  x = y;}
		y = x >> 2;  if (y != 0) {n = n - 2;  x = y;}
		y = x >> 1;  if (y != 0) return n - 2;
		return 32 - (n - static_cast<int>(x));
	}

	static int findMSB_pop(unsigned int x)
	{
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >>16);
		return 31 - glm::bitCount(~x);
	}

	static int perf_int(std::size_t Count)
	{
		type<int, int> const Data[] =
		{
			{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		std::clock_t Timestamps0 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
		{
			int Result = glm::findMSB(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps1 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
		{
			int Result = findMSB_nlz1(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps2 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
		{
			int Result = findMSB_nlz2(static_cast<unsigned int>(Data[i].Value));
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps3 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
		{
			int Result = findMSB_095(static_cast<unsigned int>(Data[i].Value));
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps4 = std::clock();

#		if GLM_HAS_BITSCAN_WINDOWS
			for(std::size_t k = 0; k < Count; ++k)
			for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
			{
				int Result = findMSB_intrinsic(Data[i].Value);
				Error += Data[i].Return == Result ? 0 : 1;
			}
#		endif//GLM_HAS_BITSCAN_WINDOWS

		std::clock_t Timestamps5 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
		{
			int Result = findMSB_pop(static_cast<unsigned int>(Data[i].Value));
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps6 = std::clock();

#		if GLM_ARCH & GLM_ARCH_AVX && GLM_COMPILER & GLM_COMPILER_VC
			for(std::size_t k = 0; k < Count; ++k)
			for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int, int>); ++i)
			{
				int Result = findMSB_avx(Data[i].Value);
				Error += Data[i].Return == Result ? 0 : 1;
			}

			std::clock_t Timestamps7 = std::clock();
#		endif

		std::printf("glm::findMSB: %d clocks\n", static_cast<int>(Timestamps1 - Timestamps0));
		std::printf("findMSB - nlz1: %d clocks\n", static_cast<int>(Timestamps2 - Timestamps1));
		std::printf("findMSB - nlz2: %d clocks\n", static_cast<int>(Timestamps3 - Timestamps2));
		std::printf("findMSB - 0.9.5: %d clocks\n", static_cast<int>(Timestamps4 - Timestamps3));

#		if GLM_HAS_BITSCAN_WINDOWS
			std::printf("findMSB - intrinsics: %d clocks\n", static_cast<int>(Timestamps5 - Timestamps4));
#		endif//GLM_HAS_BITSCAN_WINDOWS
		std::printf("findMSB - pop: %d clocks\n", static_cast<int>(Timestamps6 - Timestamps5));

#		if GLM_ARCH & GLM_ARCH_AVX && GLM_COMPILER & GLM_COMPILER_VC
			std::printf("findMSB - avx tzcnt: %d clocks\n", static_cast<int>(Timestamps7 - Timestamps6));
#		endif//GLM_ARCH & GLM_ARCH_AVX && GLM_PLATFORM & GLM_PLATFORM_WINDOWS

		return Error;
	}

	static int test_ivec4()
	{
		type<glm::ivec4, glm::ivec4> const Data[] =
		{
			{glm::ivec4(0x00000000), glm::ivec4(-1)},
			{glm::ivec4(0x00000001), glm::ivec4( 0)},
			{glm::ivec4(0x00000002), glm::ivec4( 1)},
			{glm::ivec4(0x00000003), glm::ivec4( 1)},
			{glm::ivec4(0x00000004), glm::ivec4( 2)},
			{glm::ivec4(0x00000005), glm::ivec4( 2)},
			{glm::ivec4(0x00000007), glm::ivec4( 2)},
			{glm::ivec4(0x00000008), glm::ivec4( 3)},
			{glm::ivec4(0x00000010), glm::ivec4( 4)},
			{glm::ivec4(0x00000020), glm::ivec4( 5)},
			{glm::ivec4(0x00000040), glm::ivec4( 6)},
			{glm::ivec4(0x00000080), glm::ivec4( 7)},
			{glm::ivec4(0x00000100), glm::ivec4( 8)},
			{glm::ivec4(0x00000200), glm::ivec4( 9)},
			{glm::ivec4(0x00000400), glm::ivec4(10)},
			{glm::ivec4(0x00000800), glm::ivec4(11)},
			{glm::ivec4(0x00001000), glm::ivec4(12)},
			{glm::ivec4(0x00002000), glm::ivec4(13)},
			{glm::ivec4(0x00004000), glm::ivec4(14)},
			{glm::ivec4(0x00008000), glm::ivec4(15)},
			{glm::ivec4(0x00010000), glm::ivec4(16)},
			{glm::ivec4(0x00020000), glm::ivec4(17)},
			{glm::ivec4(0x00040000), glm::ivec4(18)},
			{glm::ivec4(0x00080000), glm::ivec4(19)},
			{glm::ivec4(0x00100000), glm::ivec4(20)},
			{glm::ivec4(0x00200000), glm::ivec4(21)},
			{glm::ivec4(0x00400000), glm::ivec4(22)},
			{glm::ivec4(0x00800000), glm::ivec4(23)},
			{glm::ivec4(0x01000000), glm::ivec4(24)},
			{glm::ivec4(0x02000000), glm::ivec4(25)},
			{glm::ivec4(0x04000000), glm::ivec4(26)},
			{glm::ivec4(0x08000000), glm::ivec4(27)},
			{glm::ivec4(0x10000000), glm::ivec4(28)},
			{glm::ivec4(0x20000000), glm::ivec4(29)},
			{glm::ivec4(0x40000000), glm::ivec4(30)}
		};

		int Error(0);

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<glm::ivec4, glm::ivec4>); ++i)
		{
			glm::ivec4 Result0 = glm::findMSB(Data[i].Value);
			bool Success = glm::all(glm::equal(Data[i].Return, Result0));
			
			if (!Success) {
				std::stringstream ss;
				ss << "Test ivec4 case " << i << " failed:\n"
				   << "  Input: " << toHexString(Data[i].Value.x) << " " 
				   << toHexString(Data[i].Value.y) << " "
				   << toHexString(Data[i].Value.z) << " "
				   << toHexString(Data[i].Value.w) << "\n"
				   << "  Expected: " << Data[i].Return.x << " "
				   << Data[i].Return.y << " "
				   << Data[i].Return.z << " "
				   << Data[i].Return.w << "\n"
				   << "  Actual: " << Result0.x << " "
				   << Result0.y << " "
				   << Result0.z << " "
				   << Result0.w;
				std::cout << ss.str() << std::endl;
				Error++;
			}
		}

		return Error;
	}

	static int test_int()
	{
		typedef type<glm::uint, int> entry;

		entry const Data[] =
		{
			{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(entry); ++i)
		{
			int Result0 = glm::findMSB(Data[i].Value);
			bool Success = Data[i].Return == Result0;
			
			if (!Success) {
				LOG_ERROR(Data[i].Return, Result0,
					  "findMSB test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(Data[i].Value));
				Error++;
			}
		}

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(entry); ++i)
		{
			int Result0 = findMSB_nlz1(Data[i].Value);
			bool Success = Data[i].Return == Result0;
			
			if (!Success) {
				LOG_ERROR(Data[i].Return, Result0,
					  "findMSB_nlz1 test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(Data[i].Value));
				Error++;
			}
		}

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(entry); ++i)
		{
			int Result0 = findMSB_095(Data[i].Value);
			bool Success = Data[i].Return == Result0;
			
			if (!Success) {
				LOG_ERROR(Data[i].Return, Result0,
					  "findMSB_095 test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(Data[i].Value));
				Error++;
			}
		}

#		if GLM_HAS_BITSCAN_WINDOWS
			for(std::size_t i = 0; i < sizeof(Data) / sizeof(entry); ++i)
			{
				int Result0 = findMSB_intrinsic(Data[i].Value);
				bool Success = Data[i].Return == Result0;
				
				if (!Success) {
					LOG_ERROR(Data[i].Return, Result0,
						  "findMSB_intrinsic test_int() case " + std::to_string(i) + ":\n" +
						  "  Value: " + toHexString(Data[i].Value));
					Error++;
				}
			}
#		endif//GLM_HAS_BITSCAN_WINDOWS

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(entry); ++i)
		{
			int Result0 = findMSB_pop(Data[i].Value);
			bool Success = Data[i].Return == Result0;
			
			if (!Success) {
				LOG_ERROR(Data[i].Return, Result0,
					  "findMSB_pop test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(Data[i].Value));
				Error++;
			}
		}

		return Error;
	}

	static int test()
	{
		LOG_TEST_START("findMSB");
		int Error(0);

		Error += test_ivec4();
		Error += test_int();

		LOG_TEST_RESULT("findMSB", Error);
		return Error;
	}

	static int perf(std::size_t Samples)
	{
		int Error(0);

		Error += perf_int(Samples);

		return Error;
	}
}//findMSB

namespace findLSB
{
	template<typename genType, typename retType>
	struct type
	{
		genType		Value;
		retType		Return;
	};

	typedef type<int, int> entry;

	entry const DataI32[] =
	{
		{0x00000001,  0},
		{0x00000003,  0},
		{0x00000002,  1},
		// {0x80000000, 31}, // Clang generates an error with this
		{0x00010000, 16},
		{0x7FFF0000, 16},
		{0x7F000000, 24},
		{0x7F00FF00,  8},
		{0x00000000, -1}
	};

#	if GLM_HAS_BITSCAN_WINDOWS
		template<typename genIUType>
		static int findLSB_intrinsic(genIUType Value)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findLSB' only accept integer values");

			if(Value == 0)
				return -1;

			unsigned long Result(0);
			_BitScanForward(&Result, Value);
			return int(Result);
		}
#	endif

	template<typename genIUType>
	static int findLSB_095(genIUType Value)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findLSB' only accept integer values");
		if(Value == 0)
			return -1;

		genIUType Bit;
		for(Bit = genIUType(0); !(Value & (1 << Bit)); ++Bit){}
		return Bit;
	}

	template<typename genIUType>
	static int findLSB_ntz2(genIUType x)
	{
		if(x == 0)
			return -1;

		return glm::bitCount(~x & (x - static_cast<genIUType>(1)));
	}

	template<typename genIUType>
	static int findLSB_branchfree(genIUType x)
	{
		bool IsNull(x == 0);
		int const Keep(!IsNull);
		int const Discard(IsNull);

		return static_cast<int>(glm::bitCount(~x & (x - static_cast<genIUType>(1)))) * Keep + Discard * -1;
	}

	static int test_int()
	{
		int Error(0);

		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = glm::findLSB(DataI32[i].Value);
			bool Success = DataI32[i].Return == Result;
			
			if (!Success) {
				LOG_ERROR(DataI32[i].Return, Result,
					  "findLSB test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}
		}

		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_095(DataI32[i].Value);
			bool Success = DataI32[i].Return == Result;
			
			if (!Success) {
				LOG_ERROR(DataI32[i].Return, Result,
					  "findLSB_095 test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}
		}

#		if GLM_HAS_BITSCAN_WINDOWS
			for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
			{
				int Result = findLSB_intrinsic(DataI32[i].Value);
				bool Success = DataI32[i].Return == Result;
				
				if (!Success) {
					LOG_ERROR(DataI32[i].Return, Result,
						  "findLSB_intrinsic test_int() case " + std::to_string(i) + ":\n" +
						  "  Value: " + toHexString(DataI32[i].Value));
					Error++;
				}
			}
#		endif

		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_ntz2(DataI32[i].Value);
			bool Success = DataI32[i].Return == Result;
			
			if (!Success) {
				LOG_ERROR(DataI32[i].Return, Result,
					  "findLSB_ntz2 test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}
		}

		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_branchfree(DataI32[i].Value);
			bool Success = DataI32[i].Return == Result;
			
			if (!Success) {
				LOG_ERROR(DataI32[i].Return, Result,
					  "findLSB_branchfree test_int() case " + std::to_string(i) + ":\n" +
					  "  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}
		}

		return Error;
	}

	static int test()
	{
		LOG_TEST_START("findLSB");
		int Error(0);

		Error += test_int();

		LOG_TEST_RESULT("findLSB", Error);
		return Error;
	}

	static int perf_int(std::size_t Count)
	{
		int Error(0);

		std::clock_t Timestamps0 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = glm::findLSB(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps1 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_095(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps2 = std::clock();

#		if GLM_HAS_BITSCAN_WINDOWS
			for(std::size_t k = 0; k < Count; ++k)
			for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
			{
				int Result = findLSB_intrinsic(DataI32[i].Value);
				Error += DataI32[i].Return == Result ? 0 : 1;
			}
#		endif

		std::clock_t Timestamps3 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_ntz2(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps4 = std::clock();

		for(std::size_t k = 0; k < Count; ++k)
		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(entry); ++i)
		{
			int Result = findLSB_branchfree(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps5 = std::clock();

		std::printf("glm::findLSB: %d clocks\n", static_cast<int>(Timestamps1 - Timestamps0));
		std::printf("findLSB - 0.9.5: %d clocks\n", static_cast<int>(Timestamps2 - Timestamps1));

#		if GLM_HAS_BITSCAN_WINDOWS
			std::printf("findLSB - intrinsics: %d clocks\n", static_cast<int>(Timestamps3 - Timestamps2));
#		endif

		std::printf("findLSB - ntz2: %d clocks\n", static_cast<int>(Timestamps4 - Timestamps3));
		std::printf("findLSB - branchfree: %d clocks\n", static_cast<int>(Timestamps5 - Timestamps4));

		return Error;
	}

	static int perf(std::size_t Samples)
	{
		int Error(0);

		Error += perf_int(Samples);

		return Error;
	}
}//namespace findLSB

namespace uaddCarry
{
	static int test()
	{
		LOG_TEST_START("uaddCarry");
		int Error(0);
		
		{
			glm::uint x = std::numeric_limits<glm::uint>::max();
			glm::uint y = 0;
			glm::uint Carry = 0;
			glm::uint Result = glm::uaddCarry(x, y, Carry);

			if (Carry != 0) {
				LOG_ERROR(0, Carry, "uaddCarry: Expected Carry to be 0 when adding 0 to max uint");
				Error++;
			}
			if (Result != std::numeric_limits<glm::uint>::max()) {
				LOG_ERROR(std::numeric_limits<glm::uint>::max(), Result, 
					"uaddCarry: Expected Result to be max uint when adding 0 to max uint");
				Error++;
			}
		}

		{
			glm::uint x = std::numeric_limits<glm::uint>::max();
			glm::uint y = 1;
			glm::uint Carry = 0;
			glm::uint Result = glm::uaddCarry(x, y, Carry);

			if (Carry != 1) {
				LOG_ERROR(1, Carry, "uaddCarry: Expected Carry to be 1 when adding 1 to max uint");
				Error++;
			}
			if (Result != 0) {
				LOG_ERROR(0, Result, "uaddCarry: Expected Result to be 0 when adding 1 to max uint");
				Error++;
			}
		}

		{
			glm::uvec1 x(std::numeric_limits<glm::uint>::max());
			glm::uvec1 y(0);
			glm::uvec1 Carry(0);
			glm::uvec1 Result(glm::uaddCarry(x, y, Carry));

			if (!glm::all(glm::equal(Carry, glm::uvec1(0)))) {
				LOG_ERROR("uvec1(0)", "uvec1(" + std::to_string(Carry.x) + ")", 
					"uaddCarry: Expected Carry to be uvec1(0) when adding uvec1(0) to uvec1(max)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec1(std::numeric_limits<glm::uint>::max())))) {
				LOG_ERROR("uvec1(max uint)", "uvec1(" + std::to_string(Result.x) + ")", 
					"uaddCarry: Expected Result to be uvec1(max) when adding uvec1(0) to uvec1(max)");
				Error++;
			}
		}

		{
			glm::uvec1 x(std::numeric_limits<glm::uint>::max());
			glm::uvec1 y(1);
			glm::uvec1 Carry(0);
			glm::uvec1 Result(glm::uaddCarry(x, y, Carry));

			if (!glm::all(glm::equal(Carry, glm::uvec1(1)))) {
				LOG_ERROR("uvec1(1)", "uvec1(" + std::to_string(Carry.x) + ")", 
					"uaddCarry: Expected Carry to be uvec1(1) when adding uvec1(1) to uvec1(max)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec1(0)))) {
				LOG_ERROR("uvec1(0)", "uvec1(" + std::to_string(Result.x) + ")", 
					"uaddCarry: Expected Result to be uvec1(0) when adding uvec1(1) to uvec1(max)");
				Error++;
			}
		}

		LOG_TEST_RESULT("uaddCarry", Error);
		return Error;
	}
}//namespace uaddCarry

namespace usubBorrow
{
	static int test()
	{
		LOG_TEST_START("usubBorrow");
		int Error(0);
		
		{
			glm::uint x = 16;
			glm::uint y = 17;
			glm::uint Borrow = 0;
			glm::uint Result = glm::usubBorrow(x, y, Borrow);

			if (Borrow != 1) {
				LOG_ERROR(1, Borrow, "usubBorrow: Expected Borrow to be 1 when subtracting 17 from 16");
				Error++;
			}
			if (Result != 1) {
				LOG_ERROR(1, Result, "usubBorrow: Expected Result to be 1 when subtracting 17 from 16");
				Error++;
			}
		}

		{
			glm::uvec1 x(16);
			glm::uvec1 y(17);
			glm::uvec1 Borrow(0);
			glm::uvec1 Result(glm::usubBorrow(x, y, Borrow));

			if (!glm::all(glm::equal(Borrow, glm::uvec1(1)))) {
				LOG_ERROR("uvec1(1)", "uvec1(" + std::to_string(Borrow.x) + ")", 
					"usubBorrow: Expected Borrow to be uvec1(1) when subtracting uvec1(17) from uvec1(16)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec1(1)))) {
				LOG_ERROR("uvec1(1)", "uvec1(" + std::to_string(Result.x) + ")", 
					"usubBorrow: Expected Result to be uvec1(1) when subtracting uvec1(17) from uvec1(16)");
				Error++;
			}
		}

		{
			glm::uvec2 x(16);
			glm::uvec2 y(17);
			glm::uvec2 Borrow(0);
			glm::uvec2 Result(glm::usubBorrow(x, y, Borrow));

			if (!glm::all(glm::equal(Borrow, glm::uvec2(1)))) {
				std::stringstream ss;
				ss << "uvec2(1, 1) vs uvec2(" << Borrow.x << ", " << Borrow.y << ")";
				LOG_ERROR("uvec2(1, 1)", ss.str(), 
					"usubBorrow: Expected Borrow to be uvec2(1, 1) when subtracting uvec2(17) from uvec2(16)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec2(1)))) {
				std::stringstream ss;
				ss << "uvec2(1, 1) vs uvec2(" << Result.x << ", " << Result.y << ")";
				LOG_ERROR("uvec2(1, 1)", ss.str(), 
					"usubBorrow: Expected Result to be uvec2(1, 1) when subtracting uvec2(17) from uvec2(16)");
				Error++;
			}
		}

		{
			glm::uvec3 x(16);
			glm::uvec3 y(17);
			glm::uvec3 Borrow(0);
			glm::uvec3 Result(glm::usubBorrow(x, y, Borrow));

			if (!glm::all(glm::equal(Borrow, glm::uvec3(1)))) {
				std::stringstream ss;
				ss << "uvec3(1, 1, 1) vs uvec3(" << Borrow.x << ", " << Borrow.y << ", " << Borrow.z << ")";
				LOG_ERROR("uvec3(1, 1, 1)", ss.str(), 
					"usubBorrow: Expected Borrow to be uvec3(1, 1, 1) when subtracting uvec3(17) from uvec3(16)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec3(1)))) {
				std::stringstream ss;
				ss << "uvec3(1, 1, 1) vs uvec3(" << Result.x << ", " << Result.y << ", " << Result.z << ")";
				LOG_ERROR("uvec3(1, 1, 1)", ss.str(), 
					"usubBorrow: Expected Result to be uvec3(1, 1, 1) when subtracting uvec3(17) from uvec3(16)");
				Error++;
			}
		}

		{
			glm::uvec4 x(16);
			glm::uvec4 y(17);
			glm::uvec4 Borrow(0);
			glm::uvec4 Result(glm::usubBorrow(x, y, Borrow));

			if (!glm::all(glm::equal(Borrow, glm::uvec4(1)))) {
				std::stringstream ss;
				ss << "uvec4(1, 1, 1, 1) vs uvec4(" << Borrow.x << ", " << Borrow.y << ", " 
				   << Borrow.z << ", " << Borrow.w << ")";
				LOG_ERROR("uvec4(1, 1, 1, 1)", ss.str(), 
					"usubBorrow: Expected Borrow to be uvec4(1) when subtracting uvec4(17) from uvec4(16)");
				Error++;
			}
			if (!glm::all(glm::equal(Result, glm::uvec4(1)))) {
				std::stringstream ss;
				ss << "uvec4(1, 1, 1, 1) vs uvec4(" << Result.x << ", " << Result.y << ", " 
				   << Result.z << ", " << Result.w << ")";
				LOG_ERROR("uvec4(1, 1, 1, 1)", ss.str(), 
					"usubBorrow: Expected Result to be uvec4(1) when subtracting uvec4(17) from uvec4(16)");
				Error++;
			}
		}

		LOG_TEST_RESULT("usubBorrow", Error);
		return Error;
	}
}//namespace usubBorrow

namespace umulExtended
{
	static int test()
	{
		LOG_TEST_START("umulExtended");
		int Error(0);
		
		{
			glm::uint x = 2;
			glm::uint y = 3;
			glm::uint msb = 0;
			glm::uint lsb = 0;
			glm::umulExtended(x, y, msb, lsb);

			if (msb != 0) {
				LOG_ERROR(0, msb, "umulExtended: Expected MSB to be 0 when multiplying 2 by 3");
				Error++;
			}
			if (lsb != 6) {
				LOG_ERROR(6, lsb, "umulExtended: Expected LSB to be 6 when multiplying 2 by 3");
				Error++;
			}
		}

		{
			glm::uvec1 x(2);
			glm::uvec1 y(3);
			glm::uvec1 msb(0);
			glm::uvec1 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::uvec1(0)))) {
				LOG_ERROR("uvec1(0)", "uvec1(" + std::to_string(msb.x) + ")", 
					"umulExtended: Expected MSB to be uvec1(0) when multiplying uvec1(2) by uvec1(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::uvec1(6)))) {
				LOG_ERROR("uvec1(6)", "uvec1(" + std::to_string(lsb.x) + ")", 
					"umulExtended: Expected LSB to be uvec1(6) when multiplying uvec1(2) by uvec1(3)");
				Error++;
			}
		}

		{
			glm::uvec2 x(2);
			glm::uvec2 y(3);
			glm::uvec2 msb(0);
			glm::uvec2 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::uvec2(0)))) {
				std::stringstream ss;
				ss << "uvec2(0, 0) vs uvec2(" << msb.x << ", " << msb.y << ")";
				LOG_ERROR("uvec2(0, 0)", ss.str(), 
					"umulExtended: Expected MSB to be uvec2(0) when multiplying uvec2(2) by uvec2(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::uvec2(6)))) {
				std::stringstream ss;
				ss << "uvec2(6, 6) vs uvec2(" << lsb.x << ", " << lsb.y << ")";
				LOG_ERROR("uvec2(6, 6)", ss.str(), 
					"umulExtended: Expected LSB to be uvec2(6) when multiplying uvec2(2) by uvec2(3)");
				Error++;
			}
		}

		{
			glm::uvec3 x(2);
			glm::uvec3 y(3);
			glm::uvec3 msb(0);
			glm::uvec3 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::uvec3(0)))) {
				std::stringstream ss;
				ss << "uvec3(0, 0, 0) vs uvec3(" << msb.x << ", " << msb.y << ", " << msb.z << ")";
				LOG_ERROR("uvec3(0, 0, 0)", ss.str(), 
					"umulExtended: Expected MSB to be uvec3(0) when multiplying uvec3(2) by uvec3(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::uvec3(6)))) {
				std::stringstream ss;
				ss << "uvec3(6, 6, 6) vs uvec3(" << lsb.x << ", " << lsb.y << ", " << lsb.z << ")";
				LOG_ERROR("uvec3(6, 6, 6)", ss.str(), 
					"umulExtended: Expected LSB to be uvec3(6) when multiplying uvec3(2) by uvec3(3)");
				Error++;
			}
		}

		{
			glm::uvec4 x(2);
			glm::uvec4 y(3);
			glm::uvec4 msb(0);
			glm::uvec4 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::uvec4(0)))) {
				std::stringstream ss;
				ss << "uvec4(0, 0, 0, 0) vs uvec4(" << msb.x << ", " << msb.y << ", " 
				   << msb.z << ", " << msb.w << ")";
				LOG_ERROR("uvec4(0, 0, 0, 0)", ss.str(), 
					"umulExtended: Expected MSB to be uvec4(0) when multiplying uvec4(2) by uvec4(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::uvec4(6)))) {
				std::stringstream ss;
				ss << "uvec4(6, 6, 6, 6) vs uvec4(" << lsb.x << ", " << lsb.y << ", " 
				   << lsb.z << ", " << lsb.w << ")";
				LOG_ERROR("uvec4(6, 6, 6, 6)", ss.str(), 
					"umulExtended: Expected LSB to be uvec4(6) when multiplying uvec4(2) by uvec4(3)");
				Error++;
			}
		}

		LOG_TEST_RESULT("umulExtended", Error);
		return Error;
	}
}//namespace umulExtended

namespace imulExtended
{
	static int test()
	{
		LOG_TEST_START("imulExtended");
		int Error(0);
		
		{
			int x = 2;
			int y = 3;
			int msb = 0;
			int lsb = 0;
			glm::imulExtended(x, y, msb, lsb);

			if (msb != 0) {
				LOG_ERROR(0, msb, "imulExtended: Expected MSB to be 0 when multiplying 2 by 3");
				Error++;
			}
			if (lsb != 6) {
				LOG_ERROR(6, lsb, "imulExtended: Expected LSB to be 6 when multiplying 2 by 3");
				Error++;
			}
		}

		{
			glm::ivec1 x(2);
			glm::ivec1 y(3);
			glm::ivec1 msb(0);
			glm::ivec1 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::ivec1(0)))) {
				LOG_ERROR("ivec1(0)", "ivec1(" + std::to_string(msb.x) + ")", 
					"imulExtended: Expected MSB to be ivec1(0) when multiplying ivec1(2) by ivec1(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::ivec1(6)))) {
				LOG_ERROR("ivec1(6)", "ivec1(" + std::to_string(lsb.x) + ")", 
					"imulExtended: Expected LSB to be ivec1(6) when multiplying ivec1(2) by ivec1(3)");
				Error++;
			}
		}

		{
			glm::ivec2 x(2);
			glm::ivec2 y(3);
			glm::ivec2 msb(0);
			glm::ivec2 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::ivec2(0)))) {
				std::stringstream ss;
				ss << "ivec2(0, 0) vs ivec2(" << msb.x << ", " << msb.y << ")";
				LOG_ERROR("ivec2(0, 0)", ss.str(), 
					"imulExtended: Expected MSB to be ivec2(0) when multiplying ivec2(2) by ivec2(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::ivec2(6)))) {
				std::stringstream ss;
				ss << "ivec2(6, 6) vs ivec2(" << lsb.x << ", " << lsb.y << ")";
				LOG_ERROR("ivec2(6, 6)", ss.str(), 
					"imulExtended: Expected LSB to be ivec2(6) when multiplying ivec2(2) by ivec2(3)");
				Error++;
			}
		}

		{
			glm::ivec3 x(2);
			glm::ivec3 y(3);
			glm::ivec3 msb(0);
			glm::ivec3 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::ivec3(0)))) {
				std::stringstream ss;
				ss << "ivec3(0, 0, 0) vs ivec3(" << msb.x << ", " << msb.y << ", " << msb.z << ")";
				LOG_ERROR("ivec3(0, 0, 0)", ss.str(), 
					"imulExtended: Expected MSB to be ivec3(0) when multiplying ivec3(2) by ivec3(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::ivec3(6)))) {
				std::stringstream ss;
				ss << "ivec3(6, 6, 6) vs ivec3(" << lsb.x << ", " << lsb.y << ", " << lsb.z << ")";
				LOG_ERROR("ivec3(6, 6, 6)", ss.str(), 
					"imulExtended: Expected LSB to be ivec3(6) when multiplying ivec3(2) by ivec3(3)");
				Error++;
			}
		}

		{
			glm::ivec4 x(2);
			glm::ivec4 y(3);
			glm::ivec4 msb(0);
			glm::ivec4 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			if (!glm::all(glm::equal(msb, glm::ivec4(0)))) {
				std::stringstream ss;
				ss << "ivec4(0, 0, 0, 0) vs ivec4(" << msb.x << ", " << msb.y << ", " 
				   << msb.z << ", " << msb.w << ")";
				LOG_ERROR("ivec4(0, 0, 0, 0)", ss.str(), 
					"imulExtended: Expected MSB to be ivec4(0) when multiplying ivec4(2) by ivec4(3)");
				Error++;
			}
			if (!glm::all(glm::equal(lsb, glm::ivec4(6)))) {
				std::stringstream ss;
				ss << "ivec4(6, 6, 6, 6) vs ivec4(" << lsb.x << ", " << lsb.y << ", " 
				   << lsb.z << ", " << lsb.w << ")";
				LOG_ERROR("ivec4(6, 6, 6, 6)", ss.str(), 
					"imulExtended: Expected LSB to be ivec4(6) when multiplying ivec4(2) by ivec4(3)");
				Error++;
			}
		}

		LOG_TEST_RESULT("imulExtended", Error);
		return Error;
	}
}//namespace imulExtended

namespace bitCount
{
	template<typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
	};

	type<int> const DataI32[] =
	{
		{0x00000001,  1},
		{0x00000003,  2},
		{0x00000002,  1},
		{0x7fffffff, 31},
		{0x00000000,  0}
	};

	template<typename T>
	inline int bitCount_if(T v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		int Count(0);
		for(T i = 0, n = static_cast<T>(sizeof(T) * 8); i < n; ++i)
		{
			if(v & static_cast<T>(1 << i))
				++Count;
		}
		return Count;
	}

	template<typename T>
	inline int bitCount_vec(T v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		int Count(0);
		for(T i = 0, n = static_cast<T>(sizeof(T) * 8); i < n; ++i)
		{
			Count += static_cast<int>((v >> i) & static_cast<T>(1));
		}
		return Count;
	}

	template<bool EXEC = false>
	struct compute_bitfieldBitCountStep
	{
		template<glm::length_t L, typename T, glm::qualifier Q>
		GLM_FUNC_QUALIFIER static glm::vec<L, T, Q> call(glm::vec<L, T, Q> const& v, T, T)
		{
			return v;
		}
	};

	template<>
	struct compute_bitfieldBitCountStep<true>
	{
		template<glm::length_t L, typename T, glm::qualifier Q>
		GLM_FUNC_QUALIFIER static glm::vec<L, T, Q> call(glm::vec<L, T, Q> const& v, T Mask, T Shift)
		{
			return (v & Mask) + ((v >> Shift) & Mask);
		}
	};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(push)
#			pragma warning(disable : 4309)
#		endif

	template<glm::length_t L, typename T, glm::qualifier Q>
	static glm::vec<L, int, Q> bitCount_bitfield(glm::vec<L, T, Q> const& v)
	{
		glm::vec<L, typename glm::detail::make_unsigned<T>::type, Q> x(v);
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >=  2>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x5555555555555555ull), static_cast<typename glm::detail::make_unsigned<T>::type>( 1));
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >=  4>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x3333333333333333ull), static_cast<typename glm::detail::make_unsigned<T>::type>( 2));
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >=  8>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x0F0F0F0F0F0F0F0Full), static_cast<typename glm::detail::make_unsigned<T>::type>( 4));
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >= 16>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x00FF00FF00FF00FFull), static_cast<typename glm::detail::make_unsigned<T>::type>( 8));
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >= 32>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x0000FFFF0000FFFFull), static_cast<typename glm::detail::make_unsigned<T>::type>(16));
		x = compute_bitfieldBitCountStep<sizeof(T) * 8 >= 64>::call(x, static_cast<typename glm::detail::make_unsigned<T>::type>(0x00000000FFFFFFFFull), static_cast<typename glm::detail::make_unsigned<T>::type>(32));
		return glm::vec<L, int, Q>(x);
	}

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(pop)
#		endif

	template<typename genType>
	static int bitCount_bitfield(genType x)
	{
		return bitCount_bitfield(glm::vec<1, genType, glm::defaultp>(x)).x;
	}

	static int perf(std::size_t Size)
	{
		int Error(0);

		std::vector<int> v;
		v.resize(Size);

		std::vector<glm::ivec4> w;
		w.resize(Size);


		std::clock_t TimestampsA = std::clock();

		// bitCount - TimeIf
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = bitCount_if(static_cast<int>(i));
		}

		std::clock_t TimestampsB = std::clock();

		// bitCount - TimeVec
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = bitCount_vec(i);
		}

		std::clock_t TimestampsC = std::clock();

		// bitCount - TimeDefault
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = glm::bitCount(i);
		}

		std::clock_t TimestampsD = std::clock();

		// bitCount - TimeVec4
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				w[i] = glm::bitCount(glm::ivec4(static_cast<int>(i)));
		}

		std::clock_t TimestampsE = std::clock();

		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = bitCount_bitfield(static_cast<int>(i));
		}

		std::clock_t TimestampsF = std::clock();

		std::printf("bitCount - TimeIf %d\n", static_cast<int>(TimestampsB - TimestampsA));
		std::printf("bitCount - TimeVec %d\n", static_cast<int>(TimestampsC - TimestampsB));
		std::printf("bitCount - TimeDefault %d\n", static_cast<int>(TimestampsD - TimestampsC));
		std::printf("bitCount - TimeVec4 %d\n", static_cast<int>(TimestampsE - TimestampsD));
		std::printf("bitCount - bitfield %d\n", static_cast<int>(TimestampsF - TimestampsE));

		return Error;
	}

	static int test()
	{
		LOG_TEST_START("bitCount");
		int Error(0);

		for(std::size_t i = 0, n = sizeof(DataI32) / sizeof(type<int>); i < n; ++i)
		{
			int ResultA = glm::bitCount(DataI32[i].Value);
			bool SuccessA = DataI32[i].Return == ResultA;
			if (!SuccessA) {
				LOG_ERROR(DataI32[i].Return, ResultA, 
					"glm::bitCount failed for case " + std::to_string(i) + ":\n" +
					"  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}

			int ResultB = bitCount_if(DataI32[i].Value);
			bool SuccessB = DataI32[i].Return == ResultB;
			if (!SuccessB) {
				LOG_ERROR(DataI32[i].Return, ResultB, 
					"bitCount_if failed for case " + std::to_string(i) + ":\n" +
					"  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}

			int ResultC = bitCount_vec(DataI32[i].Value);
			bool SuccessC = DataI32[i].Return == ResultC;
			if (!SuccessC) {
				LOG_ERROR(DataI32[i].Return, ResultC, 
					"bitCount_vec failed for case " + std::to_string(i) + ":\n" +
					"  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}

			int ResultE = bitCount_bitfield(DataI32[i].Value);
			bool SuccessE = DataI32[i].Return == ResultE;
			if (!SuccessE) {
				LOG_ERROR(DataI32[i].Return, ResultE, 
					"bitCount_bitfield failed for case " + std::to_string(i) + ":\n" +
					"  Value: " + toHexString(DataI32[i].Value));
				Error++;
			}
		}

		LOG_TEST_RESULT("bitCount", Error);
		return Error;
	}
}//bitCount

int main()
{
	std::cout << "\n=======================================\n";
	std::cout << "GLM Integer Functions Test Suite\n";
	std::cout << "=======================================\n";
	
	int Error = 0;

	Error += ::bitCount::test();
	Error += ::bitfieldReverse::test();
	Error += ::findMSB::test();
	Error += ::findLSB::test();
	Error += ::umulExtended::test();
	Error += ::imulExtended::test();
	Error += ::uaddCarry::test();
	Error += ::usubBorrow::test();
	Error += ::bitfieldInsert::test();
	Error += ::bitfieldExtract::test();

#	ifdef NDEBUG
		std::size_t const Samples = 1000;
#	else
		std::size_t const Samples = 1;
#	endif

	if (Error == 0) {
		std::cout << "\n=======================================\n";
		std::cout << "Performance tests:\n";
		std::cout << "=======================================\n";
		
		::bitCount::perf(Samples);
		::bitfieldReverse::perf(Samples);
		::findMSB::perf(Samples);
		::findLSB::perf(Samples);
	}

	std::cout << "\n=======================================\n";
	std::cout << "Total errors: " << Error << "\n";
	std::cout << "=======================================\n";

	return Error;
}
