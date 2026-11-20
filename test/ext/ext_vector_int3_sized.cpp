#include "glm_test.hpp"
#include <glm/ext/vector_int3_sized.hpp>

static_assert(sizeof(glm::vec<3, glm::int8, glm::packed_highp>) == 3, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::vec<3, glm::int16, glm::packed_highp>) == 6, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::vec<3, glm::int32, glm::packed_highp>) == 12, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::vec<3, glm::int64, glm::packed_highp>) == 24, "int64 size isn't 8 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::vec<3, glm::int8, glm::packed_highp>) == 3);
	Error += GLM_CHECK(sizeof(glm::vec<3, glm::int16, glm::packed_highp>) == 6);
	Error += GLM_CHECK(sizeof(glm::vec<3, glm::int32, glm::packed_highp>) == 12);
	Error += GLM_CHECK(sizeof(glm::vec<3, glm::int64, glm::packed_highp>) == 24);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::i8vec3) < sizeof(glm::i16vec3));
	Error += GLM_CHECK(sizeof(glm::i16vec3) < sizeof(glm::i32vec3));
	Error += GLM_CHECK(sizeof(glm::i32vec3) < sizeof(glm::i64vec3));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
