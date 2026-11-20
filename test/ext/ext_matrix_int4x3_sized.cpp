#include "glm_test.hpp"
#include <glm/ext/matrix_int4x3_sized.hpp>

static_assert(sizeof(glm::mat<4, 3, glm::int8, glm::packed_highp>) == 12, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::int16, glm::packed_highp>) == 24, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::int32, glm::packed_highp>) == 48, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::mat<4, 3, glm::int64, glm::packed_highp>) == 96, "int64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::i8mat4x3), sizeof(glm::i16mat4x3));
	Error += GLM_CHECK_LT(sizeof(glm::i16mat4x3), sizeof(glm::i32mat4x3));
	Error += GLM_CHECK_LT(sizeof(glm::i32mat4x3), sizeof(glm::i64mat4x3));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
