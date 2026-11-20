#include "glm_test.hpp"
#include <glm/ext/matrix_uint4x3_sized.hpp>

static_assert(sizeof(glm::mat<4, 3, glm::uint8, glm::packed_highp>) == 12, "uint8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::uint16, glm::packed_highp>) == 24, "uint16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::uint32, glm::packed_highp>) == 48, "uint32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::uint64, glm::packed_highp>) == 96, "uint64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::u8mat4x3) < sizeof(glm::u16mat4x3));
	Error += GLM_CHECK(sizeof(glm::u16mat4x3) < sizeof(glm::u32mat4x3));
	Error += GLM_CHECK(sizeof(glm::u32mat4x3) < sizeof(glm::u64mat4x3));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
