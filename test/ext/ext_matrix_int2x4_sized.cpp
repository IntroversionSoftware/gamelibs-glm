#include "glm_test.hpp"
#include <glm/ext/matrix_int2x4_sized.hpp>

static_assert(sizeof(glm::i8mat2x4) == 8, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::i16mat2x4) == 16, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::i32mat2x4) == 32, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::i64mat2x4) == 64, "int64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::i8mat2x4) < sizeof(glm::i16mat2x4));
	Error += GLM_CHECK(sizeof(glm::i16mat2x4) < sizeof(glm::i32mat2x4));
	Error += GLM_CHECK(sizeof(glm::i32mat2x4) < sizeof(glm::i64mat2x4));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
