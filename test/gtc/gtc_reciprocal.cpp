#include "glm_test.hpp"
#include <glm/gtc/reciprocal.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/vector_float2.hpp>

// gtc/reciprocal.hpp brings in the scalar and vector forms (tested in ext_scalar_reciprocal and
// ext_vector_reciprocal).
static int test_header()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::sec(0.0f), 1.0f, 0.0001f);
	Error += GLM_CHECK_NEAR(glm::sec(glm::vec2(0.0f)), glm::vec2(1.0f), 0.0001f);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_header());

	return GLM_TEST_FINISH(Error);
}
