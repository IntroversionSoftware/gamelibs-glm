#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>

static int test_epsilon()
{
	int Error = 0;

	{
		float Test = glm::epsilon<float>();
		Error += GLM_CHECK_GT(Test, 0.0f);
	}

	{
		double Test = glm::epsilon<double>();
		Error += GLM_CHECK_GT(Test, 0.0);
	}

	return Error;
}

int main()
{
	int Error(0);

	//float MinHalf = 0.0f;
	//while (glm::half(MinHalf) == glm::half(0.0f))
	//	MinHalf += std::numeric_limits<float>::epsilon();
	Error += GLM_TEST(test_epsilon());

	return GLM_TEST_FINISH(Error);
}
