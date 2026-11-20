#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>

static int test_fastInverseSqrt()
{
	int Error = 0;

	Error += GLM_CHECK(glm::epsilonEqual(glm::fastInverseSqrt(1.0f), 1.0f, 0.01f));
	Error += GLM_CHECK(glm::epsilonEqual(glm::fastInverseSqrt(1.0), 1.0, 0.01));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::fastInverseSqrt(glm::vec2(1.0f)), glm::vec2(1.0f), 0.01f)));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::fastInverseSqrt(glm::dvec3(1.0)), glm::dvec3(1.0), 0.01)));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::fastInverseSqrt(glm::dvec4(1.0)), glm::dvec4(1.0), 0.01)));

	return Error;
}

static int test_fastDistance()
{
	int Error = 0;

	float const A = glm::fastDistance(0.0f, 1.0f);
	float const B = glm::fastDistance(glm::vec2(0.0f), glm::vec2(1.0f, 0.0f));
	float const C = glm::fastDistance(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float const D = glm::fastDistance(glm::vec4(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	Error += GLM_CHECK(glm::epsilonEqual(A, 1.0f, 0.01f));
	Error += GLM_CHECK(glm::epsilonEqual(B, 1.0f, 0.01f));
	Error += GLM_CHECK(glm::epsilonEqual(C, 1.0f, 0.01f));
	Error += GLM_CHECK(glm::epsilonEqual(D, 1.0f, 0.01f));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_fastInverseSqrt());
	Error += GLM_TEST(test_fastDistance());

	return GLM_TEST_FINISH(Error);
}
