#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext/scalar_int_sized.hpp>

static int test_bit_operator()
{
	int Error = 0;

	glm::ivec4 const a(1);
	glm::ivec4 const b = ~a;
	Error += GLM_CHECK(glm::all(glm::equal(b, glm::ivec4(-2))));

	glm::int32 const c(1);
	glm::int32 const d = ~c;
	Error += GLM_CHECK(d == -2);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_bit_operator());

	return GLM_TEST_FINISH(Error);
}
