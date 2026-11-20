#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/gradient_paint.hpp>

static int test_radialGradient()
{
	int Error = 0;

	float Gradient = glm::radialGradient(glm::vec2(0), 1.0f, glm::vec2(1), glm::vec2(0.5));
	Error += GLM_CHECK_NE(Gradient, 0.0f);

	return Error;
}

static int test_linearGradient()
{
	int Error = 0;

	float Gradient = glm::linearGradient(glm::vec2(0), glm::vec2(1), glm::vec2(0.5));
	Error += GLM_CHECK_NE(Gradient, 0.0f);

	return Error;
}

int main()
{
	int Error = 0;

    Error += GLM_TEST(test_radialGradient());
    Error += GLM_TEST(test_linearGradient());

	return GLM_TEST_FINISH(Error);
}


