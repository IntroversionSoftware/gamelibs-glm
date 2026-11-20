#include "glm_test.hpp"
#include <glm/ext/vector_uint4_sized.hpp>

static_assert(sizeof(glm::u8vec4) == 4, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::u16vec4) == 8, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::u32vec4) == 16, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::u64vec4) == 32, "int64 size isn't 8 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::u8vec4), 4);
	Error += GLM_CHECK_EQ(sizeof(glm::u16vec4), 8);
	Error += GLM_CHECK_EQ(sizeof(glm::u32vec4), 16);
	Error += GLM_CHECK_EQ(sizeof(glm::u64vec4), 32);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::u8vec4), sizeof(glm::u16vec4));
	Error += GLM_CHECK_LT(sizeof(glm::u16vec4), sizeof(glm::u32vec4));
	Error += GLM_CHECK_LT(sizeof(glm::u32vec4), sizeof(glm::u64vec4));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
