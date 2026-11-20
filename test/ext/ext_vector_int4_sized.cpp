#include "glm_test.hpp"
#include <glm/ext/vector_int4_sized.hpp>

static_assert(sizeof(glm::i8vec4) == 4, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::i16vec4) == 8, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::i32vec4) == 16, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::i64vec4) == 32, "int64 size isn't 8 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::i8vec4) == 4);
	Error += GLM_CHECK(sizeof(glm::i16vec4) == 8);
	Error += GLM_CHECK(sizeof(glm::i32vec4) == 16);
	Error += GLM_CHECK(sizeof(glm::i64vec4) == 32);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::i8vec4) < sizeof(glm::i16vec4));
	Error += GLM_CHECK(sizeof(glm::i16vec4) < sizeof(glm::i32vec4));
	Error += GLM_CHECK(sizeof(glm::i32vec4) < sizeof(glm::i64vec4));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
