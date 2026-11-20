#define GLM_FORCE_MESSAGES
#include "glm_test.hpp"
#include <glm/vec3.hpp>
#include <cstdio>

static int test_compiler()
{
	int Error(0);

#	if(GLM_COMPILER & GLM_COMPILER_VC)
	{
		switch(GLM_COMPILER)
		{
		case GLM_COMPILER_VC12:
			glm_test::info("Visual C++ 12 - 2013\n");
			break;
		case GLM_COMPILER_VC14:
			glm_test::info("Visual C++ 14 - 2015\n");
			break;
		case GLM_COMPILER_VC15:
			glm_test::info("Visual C++ 15 - 2017\n");
			break;
		case GLM_COMPILER_VC15_3:
			glm_test::info("Visual C++ 15.3 - 2017\n");
			break;
		case GLM_COMPILER_VC15_5:
			glm_test::info("Visual C++ 15.5 - 2017\n");
			break;
		case GLM_COMPILER_VC15_6:
			glm_test::info("Visual C++ 15.6 - 2017\n");
			break;
		case GLM_COMPILER_VC15_7:
			glm_test::info("Visual C++ 15.7 - 2017\n");
			break;
		case GLM_COMPILER_VC15_8:
			glm_test::info("Visual C++ 15.8 - 2017\n");
			break;
		case GLM_COMPILER_VC15_9:
			glm_test::info("Visual C++ 15.9 - 2017\n");
			break;
		case GLM_COMPILER_VC16:
			glm_test::info("Visual C++ 16 - 2019\n");
			break;
		case GLM_COMPILER_VC17:
			glm_test::info("Visual C++ 17 - 2022\n");
			break;
		default:
			glm_test::info("Visual C++ version not detected\n");
			Error += 1;
			break;
		}
	}
#	elif(GLM_COMPILER & GLM_COMPILER_GCC)
	{
		switch(GLM_COMPILER)
		{
		case GLM_COMPILER_GCC46:
			glm_test::info("GCC 4.6\n");
			break;
		case GLM_COMPILER_GCC47:
			glm_test::info("GCC 4.7\n");
			break;
		case GLM_COMPILER_GCC48:
			glm_test::info("GCC 4.8\n");
			break;
		case GLM_COMPILER_GCC49:
			glm_test::info("GCC 4.9\n");
			break;
		case GLM_COMPILER_GCC5:
			glm_test::info("GCC 5\n");
			break;
		case GLM_COMPILER_GCC6:
			glm_test::info("GCC 6\n");
			break;
		case GLM_COMPILER_GCC7:
			glm_test::info("GCC 7\n");
			break;
		case GLM_COMPILER_GCC8:
			glm_test::info("GCC 8\n");
			break;
		case GLM_COMPILER_GCC9:
			glm_test::info("GCC 9\n");
			break;
		case GLM_COMPILER_GCC10:
			glm_test::info("GCC 10\n");
			break;
		case GLM_COMPILER_GCC11:
			glm_test::info("GCC 11\n");
			break;
		case GLM_COMPILER_GCC12:
			glm_test::info("GCC 12\n");
			break;
		case GLM_COMPILER_GCC13:
			glm_test::info("GCC 13\n");
			break;
		case GLM_COMPILER_GCC14:
			glm_test::info("GCC 14\n");
			break;
		default:
			glm_test::info("GCC version not detected\n");
			Error += 1;
			break;
		}
	}
#	elif(GLM_COMPILER & GLM_COMPILER_CUDA)
	{
		glm_test::info("CUDA\n");
	}
#	elif(GLM_COMPILER & GLM_COMPILER_CLANG)
	{
		switch(GLM_COMPILER)
		{
		case GLM_COMPILER_CLANG34:
			glm_test::info("Clang 3.4\n");
			break;
		case GLM_COMPILER_CLANG35:
			glm_test::info("Clang 3.5\n");
			break;
		case GLM_COMPILER_CLANG36:
			glm_test::info("Clang 3.6\n");
			break;
		case GLM_COMPILER_CLANG37:
			glm_test::info("Clang 3.7\n");
			break;
		case GLM_COMPILER_CLANG38:
			glm_test::info("Clang 3.8\n");
			break;
		case GLM_COMPILER_CLANG39:
			glm_test::info("Clang 3.9\n");
			break;
		case GLM_COMPILER_CLANG4:
			glm_test::info("Clang 4\n");
			break;
		case GLM_COMPILER_CLANG5:
			glm_test::info("Clang 5\n");
			break;
		case GLM_COMPILER_CLANG6:
			glm_test::info("Clang 6\n");
			break;
		case GLM_COMPILER_CLANG7:
			glm_test::info("Clang 7\n");
			break;
		case GLM_COMPILER_CLANG8:
			glm_test::info("Clang 8\n");
			break;
		case GLM_COMPILER_CLANG9:
			glm_test::info("Clang 9\n");
			break;
		case GLM_COMPILER_CLANG10:
			glm_test::info("Clang 10\n");
			break;
		case GLM_COMPILER_CLANG11:
			glm_test::info("Clang 11\n");
			break;
		case GLM_COMPILER_CLANG12:
			glm_test::info("Clang 12\n");
			break;
		case GLM_COMPILER_CLANG13:
			glm_test::info("Clang 13\n");
			break;
		case GLM_COMPILER_CLANG14:
			glm_test::info("Clang 14\n");
			break;
		case GLM_COMPILER_CLANG15:
			glm_test::info("Clang 15\n");
			break;
		case GLM_COMPILER_CLANG16:
			glm_test::info("Clang 16\n");
			break;
		case GLM_COMPILER_CLANG17:
			glm_test::info("Clang 17\n");
			break;
		case GLM_COMPILER_CLANG18:
			glm_test::info("Clang 18\n");
			break;
		case GLM_COMPILER_CLANG19:
			glm_test::info("Clang 19\n");
			break;
		default:
			glm_test::info("LLVM version not detected\n");
			break;
		}
	}
#	elif(GLM_COMPILER & GLM_COMPILER_INTEL)
	{
		switch(GLM_COMPILER)
		{
		case GLM_COMPILER_INTEL14:
			glm_test::info("ICC 14 - 2013 SP1\n");
			break;
		case GLM_COMPILER_INTEL15:
			glm_test::info("ICC 15 - 2015\n");
			break;
		case GLM_COMPILER_INTEL16:
			glm_test::info("ICC 16 - 2015\n");
			break;
		case GLM_COMPILER_INTEL17:
			glm_test::info("ICC 17 - 2016\n");
			break;
		case GLM_COMPILER_INTEL18:
			glm_test::info("ICC 18 - 2017\n");
			break;
		case GLM_COMPILER_INTEL19:
			glm_test::info("ICC 19 - 2018\n");
			break;
		case GLM_COMPILER_INTEL21:
			glm_test::info("ICC 21 - 2021\n");
			break;
		default:
			glm_test::info("Intel compiler version not detected\n");
			Error += 1;
			break;
		}
	}
#else
	{
		glm_test::info("Undetected compiler\n");
		Error += 1;
	}
#endif
	return Error;
}

static int test_model()
{
	int Error = 0;

	Error += GLM_CHECK(((sizeof(void*) == 4) && (GLM_MODEL == GLM_MODEL_32)) || ((sizeof(void*) == 8) && (GLM_MODEL == GLM_MODEL_64)));

#	if GLM_MODEL == GLM_MODEL_32
		glm_test::info("GLM_MODEL_32\n");
#	elif GLM_MODEL == GLM_MODEL_64
		glm_test::info("GLM_MODEL_64\n");
#	endif

	return Error;
}

static int test_instruction_set()
{
	int Error = 0;

	glm_test::info("GLM_ARCH: ");

#	if(GLM_ARCH & GLM_ARCH_ARM_BIT)
		glm_test::info("ARM ");
#	elif(GLM_ARCH & GLM_ARCH_NEON_BIT)
		glm_test::info("NEON ");
#	elif(GLM_ARCH & GLM_ARCH_AVX2_BIT)
		glm_test::info("AVX2 ");
#	elif(GLM_ARCH & GLM_ARCH_AVX_BIT)
		glm_test::info("AVX ");
#	elif(GLM_ARCH & GLM_ARCH_SSE42_BIT)
		glm_test::info("SSE4.2 ");
#	elif(GLM_ARCH & GLM_ARCH_SSE41_BIT)
		glm_test::info("SSE4.1 ");
#	elif(GLM_ARCH & GLM_ARCH_SSSE3_BIT)
		glm_test::info("SSSE3 ");
#	elif(GLM_ARCH & GLM_ARCH_SSE3_BIT)
		glm_test::info("SSE3 ");
#	elif(GLM_ARCH & GLM_ARCH_SSE2_BIT)
		glm_test::info("SSE2 ");
#	endif

	glm_test::info("\n");

	return Error;
}

static int test_cpp_version()
{
	glm_test::info("__cplusplus: %d\n", static_cast<int>(__cplusplus));

	glm_test::info("GLM_LANG: ");

#	if(GLM_LANG & GLM_LANG_CXX20_FLAG)
		glm_test::info("C++ 20");
#	elif(GLM_LANG & GLM_LANG_CXX17_FLAG)
		glm_test::info("C++ 17");
#	elif(GLM_LANG & GLM_LANG_CXX14_FLAG)
		glm_test::info("C++ 14");
#	elif(GLM_LANG & GLM_LANG_CXX11_FLAG)
		glm_test::info("C++ 11");
#	elif(GLM_LANG & GLM_LANG_CXX98_FLAG)
		glm_test::info("C++ 98");
#	endif
	glm_test::info("\n");

	return 0;
}

static int test_operators()
{
	glm::ivec3 A(1);
	glm::ivec3 B(1);
	bool R = A != B;
	bool S = A == B;

	return (S && !R) ? 0 : 1;
}

int main()
{
	int Error = 0;

#	if !defined(GLM_FORCE_PLATFORM_UNKNOWN) && !defined(GLM_FORCE_COMPILER_UNKNOWN) && !defined(GLM_FORCE_ARCH_UNKNOWN) && !defined(GLM_FORCE_CXX_UNKNOWN)

		Error += GLM_TEST(test_cpp_version());
		Error += GLM_TEST(test_compiler());
		Error += GLM_TEST(test_model());
		Error += GLM_TEST(test_instruction_set());
		Error += GLM_TEST(test_operators());

#	endif

	return GLM_TEST_FINISH(Error);
}
