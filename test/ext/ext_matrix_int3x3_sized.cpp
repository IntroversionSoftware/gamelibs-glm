#include <glm/ext/matrix_int3x3_sized.hpp>

static_assert(sizeof(glm::mat<3, 3, glm::int8, glm::packed_highp>) == 9, "int8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::mat<3, 3, glm::int16, glm::packed_highp>) == 18, "int16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::mat<3, 3, glm::int32, glm::packed_highp>) == 36, "int32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::mat<3, 3, glm::int64, glm::packed_highp>) == 72, "int64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += sizeof(glm::i8mat3x3) < sizeof(glm::i16mat3x3) ? 0 : 1;
	Error += sizeof(glm::i16mat3x3) < sizeof(glm::i32mat3x3) ? 0 : 1;
	Error += sizeof(glm::i32mat3x3) < sizeof(glm::i64mat3x3) ? 0 : 1;

	return Error;
}

int main()
{
	int Error = 0;

	Error += test_comp();

	return Error;
}
