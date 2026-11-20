#include "glm_test.hpp"
#include <glm/ext/vector_relational.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>

static int test_hsv()
{
	int Error = 0;

	glm::vec3 colorHSV = glm::hsvColor(glm::vec3(1.0f, 0.5f, 0.0f));
	glm::vec3 colorRGB = glm::rgbColor(colorHSV);

	Error += GLM_CHECK(glm::all(glm::equal(colorRGB, glm::vec3(1.0f, 0.5f, 0.0f), glm::epsilon<float>())));

	return Error;
}

static int test_saturation()
{
	int Error = 0;

	glm::vec4 Color = glm::saturation(1.0f, glm::vec4(1.0, 0.5, 0.0, 1.0));
	Error += GLM_CHECK(glm::all(glm::equal(Color, glm::vec4(1.0, 0.5, 0.0, 1.0), glm::epsilon<float>())));

	return Error;
}

int main()
{
	int Error(0);

	Error += GLM_TEST(test_hsv());
	Error += GLM_TEST(test_saturation());

	return GLM_TEST_FINISH(Error);
}
