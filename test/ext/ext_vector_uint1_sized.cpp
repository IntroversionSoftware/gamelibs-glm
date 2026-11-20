#include "glm_test.hpp"
#include <glm/ext/vector_uint1_sized.hpp>

static_assert(sizeof(glm::u8vec1) == 1, "uint8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::u16vec1) == 2, "uint16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::u32vec1) == 4, "uint32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::u64vec1) == 8, "uint64 size isn't 8 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::u8vec1) == 1);
	Error += GLM_CHECK(sizeof(glm::u16vec1) == 2);
	Error += GLM_CHECK(sizeof(glm::u32vec1) == 4);
	Error += GLM_CHECK(sizeof(glm::u64vec1) == 8);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::u8vec1) < sizeof(glm::u16vec1));
	Error += GLM_CHECK(sizeof(glm::u16vec1) < sizeof(glm::u32vec1));
	Error += GLM_CHECK(sizeof(glm::u32vec1) < sizeof(glm::u64vec1));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
