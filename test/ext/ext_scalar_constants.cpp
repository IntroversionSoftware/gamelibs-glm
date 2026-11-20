#include "glm_test.hpp"
#include <glm/ext/scalar_constants.hpp>

template <typename valType>
static int test_epsilon()
{
	int Error = 0;

	valType const Test = glm::epsilon<valType>();
	Error += GLM_CHECK(Test > static_cast<valType>(0));

	return Error;
}

template <typename valType>
static int test_pi()
{
	int Error = 0;

	valType const Test = glm::pi<valType>();
	Error += GLM_CHECK(Test > static_cast<valType>(3.14));
	Error += GLM_CHECK(Test < static_cast<valType>(3.15));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_epsilon<float>());
	Error += GLM_TEST(test_epsilon<double>());
	Error += GLM_TEST(test_pi<float>());
	Error += GLM_TEST(test_pi<double>());

	return GLM_TEST_FINISH(Error);
}
