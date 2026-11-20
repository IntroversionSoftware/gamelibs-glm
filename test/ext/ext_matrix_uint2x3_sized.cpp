#include <glm/ext/matrix_uint2x3_sized.hpp>

static_assert(sizeof(glm::mat<2, 3, glm::uint8, glm::packed_highp>) == 6, "uint8 size isn't 1 byte on this platform");
static_assert(sizeof(glm::mat<2, 3, glm::uint16, glm::packed_highp>) == 12, "uint16 size isn't 2 bytes on this platform");
static_assert(sizeof(glm::mat<2, 3, glm::uint32, glm::packed_highp>) == 24, "uint32 size isn't 4 bytes on this platform");
static_assert(sizeof(glm::mat<2, 3, glm::uint64, glm::packed_highp>) == 48, "uint64 size isn't 8 bytes on this platform");

static int test_comp()
{
	int Error = 0;

	Error += sizeof(glm::u8mat2x3) < sizeof(glm::u16mat2x3) ? 0 : 1;
	Error += sizeof(glm::u16mat2x3) < sizeof(glm::u32mat2x3) ? 0 : 1;
	Error += sizeof(glm::u32mat2x3) < sizeof(glm::u64mat2x3) ? 0 : 1;

	return Error;
}

int main()
{
	int Error = 0;

	Error += test_comp();

	return Error;
}
