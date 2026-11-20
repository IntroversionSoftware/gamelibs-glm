#include "glm_test.hpp"
#include <glm/detail/setup.hpp>

#if GLM_PLATFORM & GLM_PLATFORM_APPLE // Fail on Github macOS-latest (macOS-13 was fine)
int main()
{
	return 0;
}
#else

#if !defined(GLM_FORCE_PURE) && !defined(GLM_FORCE_DEFAULT_ALIGNED_GENTYPES)
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#endif

#include <glm/glm.hpp>

#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
#include <type_traits>

static_assert(sizeof(glm::bvec4) > sizeof(glm::bvec2), "Invalid sizeof");
static_assert(sizeof(glm::ivec4) > sizeof(glm::uvec2), "Invalid sizeof");
static_assert(sizeof(glm::dvec4) > sizeof(glm::dvec2), "Invalid sizeof");

static_assert(sizeof(glm::bvec4) == sizeof(glm::bvec3), "Invalid sizeof");
static_assert(sizeof(glm::uvec4) == sizeof(glm::uvec3), "Invalid sizeof");
static_assert(sizeof(glm::dvec4) == sizeof(glm::dvec3), "Invalid sizeof");

static int test_storage_aligned()
{
	int Error = 0;

	size_t size1_aligned = sizeof(glm::detail::storage<1, int, true>::type);
	Error += GLM_CHECK_EQ(size1_aligned, sizeof(int) * 1);
	size_t size2_aligned = sizeof(glm::detail::storage<2, int, true>::type);
	Error += GLM_CHECK_EQ(size2_aligned, sizeof(int) * 2);
	size_t size4_aligned = sizeof(glm::detail::storage<4, int, true>::type);
	Error += GLM_CHECK_EQ(size4_aligned, sizeof(int) * 4);

	size_t align1_aligned = alignof(glm::detail::storage<1, int, true>::type);
	Error += GLM_CHECK_EQ(align1_aligned, 4);
	size_t align2_aligned = alignof(glm::detail::storage<2, int, true>::type);
	Error += GLM_CHECK_EQ(align2_aligned, 8);
	size_t align4_aligned = alignof(glm::detail::storage<4, int, true>::type);
	Error += GLM_CHECK_EQ(align4_aligned, 16);

	return Error;
}

static int test_storage_unaligned()
{
	int Error = 0;

	size_t align1_unaligned = alignof(glm::detail::storage<1, int, false>::type);
	Error += GLM_CHECK_EQ(align1_unaligned, sizeof(int));
	size_t align2_unaligned = alignof(glm::detail::storage<2, int, false>::type);
	Error += GLM_CHECK_EQ(align2_unaligned, sizeof(int));
	size_t align3_unaligned = alignof(glm::detail::storage<3, int, false>::type);
	Error += GLM_CHECK_EQ(align3_unaligned, sizeof(int));
	size_t align4_unaligned = alignof(glm::detail::storage<4, int, false>::type);
	Error += GLM_CHECK_EQ(align4_unaligned, sizeof(int));

	return Error;
}

static int test_vec3_aligned()
{
	int Error = 0;

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

	struct Struct1
	{
		glm::vec4 A;
		float B;
		glm::vec3 C;
	};

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

	std::size_t const Size1 = sizeof(Struct1);
	Error += GLM_CHECK_EQ(Size1, 48);

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#elif (GLM_COMPILER & GLM_COMPILER_VC)
#	pragma warning(push)
#	pragma warning(disable: 4324) // 'test_vec3_aligned::Struct2': structure was padded due to alignment specifier 
#endif

	struct Struct2
	{
		glm::vec4 A;
		glm::vec3 B;
		float C;
	};

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#elif (GLM_COMPILER & GLM_COMPILER_VC)
#	pragma warning(pop)
#endif

	std::size_t const Size2 = sizeof(Struct2);
	Error += GLM_CHECK_EQ(Size2, 48);

	return Error;
}

#endif

int main()
{
	int Error = 0;

#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
		Error += GLM_TEST(test_storage_aligned());
		Error += GLM_TEST(test_storage_unaligned());
		Error += GLM_TEST(test_vec3_aligned());
#	endif

	return GLM_TEST_FINISH(Error);
}

#endif//GLM_PLATFORM & GLM_PLATFORM_APPLE
