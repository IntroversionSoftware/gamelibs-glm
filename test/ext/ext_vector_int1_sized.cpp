#include "glm_test.hpp"
#include <glm/ext/vector_int1_sized.hpp>

static_assert(sizeof(glm::i8vec1) == 1, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::i16vec1) == 2, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::i32vec1) == 4, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::i64vec1) == 8, "int64 size isn't 8 bytes on this platform");

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::i8vec1), 1);
	Error += GLM_CHECK_EQ(sizeof(glm::i16vec1), 2);
	Error += GLM_CHECK_EQ(sizeof(glm::i32vec1), 4);
	Error += GLM_CHECK_EQ(sizeof(glm::i64vec1), 8);

	return Error;
}

static int test_comp()
{
	int Error = 0;

	Error += GLM_CHECK_LT(sizeof(glm::i8vec1), sizeof(glm::i16vec1));
	Error += GLM_CHECK_LT(sizeof(glm::i16vec1), sizeof(glm::i32vec1));
	Error += GLM_CHECK_LT(sizeof(glm::i32vec1), sizeof(glm::i64vec1));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_comp());

	return GLM_TEST_FINISH(Error);
}
