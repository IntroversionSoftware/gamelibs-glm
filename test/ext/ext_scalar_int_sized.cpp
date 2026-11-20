#include "glm_test.hpp"
#include <glm/ext/scalar_int_sized.hpp>

static_assert(sizeof(glm::int8) == 1, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::int16) == 2, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::int32) == 4, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::int64) == 8, "int64 size isn't 8 bytes on this platform");
static_assert(sizeof(glm::int16) == sizeof(short), "signed short size isn't 4 bytes on this platform");
static_assert(sizeof(glm::int32) == sizeof(int), "signed int size isn't 4 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::int8), 1);
	Error += GLM_CHECK_EQ(sizeof(glm::int16), 2);
	Error += GLM_CHECK_EQ(sizeof(glm::int32), 4);
	Error += GLM_CHECK_EQ(sizeof(glm::int64), 8);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::int8), sizeof(glm::int16));
	Error += GLM_CHECK_LT(sizeof(glm::int16), sizeof(glm::int32));
	Error += GLM_CHECK_LT(sizeof(glm::int32), sizeof(glm::int64));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
