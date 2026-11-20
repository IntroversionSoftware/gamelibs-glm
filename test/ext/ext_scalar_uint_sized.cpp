#include "glm_test.hpp"
#include <glm/ext/scalar_uint_sized.hpp>

static_assert(sizeof(glm::uint8) == 1, "uint8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::uint16) == 2, "uint16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::uint32) == 4, "uint32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::uint64) == 8, "uint64 size isn't 8 bytes on this platform");
static_assert(sizeof(glm::uint16) == sizeof(unsigned short), "unsigned short size isn't 4 bytes on this platform");
static_assert(sizeof(glm::uint32) == sizeof(unsigned int), "unsigned int size isn't 4 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::uint8), 1);
	Error += GLM_CHECK_EQ(sizeof(glm::uint16), 2);
	Error += GLM_CHECK_EQ(sizeof(glm::uint32), 4);
	Error += GLM_CHECK_EQ(sizeof(glm::uint64), 8);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::uint8), sizeof(glm::uint16));
	Error += GLM_CHECK_LT(sizeof(glm::uint16), sizeof(glm::uint32));
	Error += GLM_CHECK_LT(sizeof(glm::uint32), sizeof(glm::uint64));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
