#include "glm_test.hpp"
#include <glm/vec2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/texture.hpp>

static int test_levels()
{
	int Error = 0;

	int const Levels = glm::levels(glm::ivec2(3, 2));
	Error += GLM_CHECK_EQ(Levels, 2);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_levels());

	return GLM_TEST_FINISH(Error);
}
