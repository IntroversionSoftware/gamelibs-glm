#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/type_aligned.hpp>
#include <cstdio>

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

static int test_decl()
{
	int Error(0);

	{
		struct S1
		{
			glm::aligned_vec4 B;
		};

		struct S2
		{
			glm::vec4 B;
		};

		Error += GLM_CHECK_GE(sizeof(S1), sizeof(S2));
	}

	{
		struct S1
		{
			bool A;
			glm::vec3 B;
		};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(push)
#			pragma warning(disable : 4324)
#		endif

		struct S2
		{
			bool A;
			glm::aligned_vec3 B;
		};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(pop)
#		endif

		Error += GLM_CHECK_LE(sizeof(S1), sizeof(S2));
	}

	{
#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(push)
#			pragma warning(disable : 4324)
#		endif

		struct S1
		{
			bool A;
			glm::aligned_vec4 B;
		};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(pop)
#		endif

		struct S2
		{
			bool A;
			glm::vec4 B;
		};

		Error += GLM_CHECK_GE(sizeof(S1), sizeof(S2));
	}

	{
#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(push)
#			pragma warning(disable : 4324)
#		endif

		struct S1
		{
			bool A;
			glm::aligned_dvec4 B;
		};

#		if GLM_COMPILER & GLM_COMPILER_VC
#			pragma warning(pop)
#		endif

		struct S2
		{
			bool A;
			glm::dvec4 B;
		};

		Error += GLM_CHECK_GE(sizeof(S1), sizeof(S2));
	}

	return Error;
}

static int test_mul()
{
	int Error = 0;

	glm::mat4 A(1.0f);
	glm::mat4 B(1.0f);

	glm::mat4 C = A * B;
	Error += GLM_CHECK_EQ(C, glm::mat4(1.0f));

	return Error;
}

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

int main()
{
	int Error(0);

	Error += GLM_TEST(test_decl());
	Error += GLM_TEST(test_mul());

	return GLM_TEST_FINISH(Error);
}
