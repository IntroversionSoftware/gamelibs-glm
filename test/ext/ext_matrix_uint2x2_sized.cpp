#include "glm_test.hpp"
#include <glm/ext/matrix_uint2x2_sized.hpp>

static_assert(sizeof(glm::u8mat2x2) == 4, "uint8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::u16mat2x2) == 8, "uint16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::u32mat2x2) == 16, "uint32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::u64mat2x2) == 32, "uint64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::u8mat2x2), sizeof(glm::u16mat2x2));
	Error += GLM_CHECK_LT(sizeof(glm::u16mat2x2), sizeof(glm::u32mat2x2));
	Error += GLM_CHECK_LT(sizeof(glm::u32mat2x2), sizeof(glm::u64mat2x2));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
