#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/ext/vector_relational.hpp>
#include <glm/gtx/safe_normalize.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cmath>
#include <limits>

template<typename T>
static int test_safeNormalize()
{
	using vec2 = glm::vec<2, T>;
	using vec3 = glm::vec<3, T>;
	using vec4 = glm::vec<4, T>;

	int Error = 0;
	T const Eps = static_cast<T>(1e-5);

	// A vector with a direction normalizes as normalize does.
	vec3 const A(3, 0, 4);
	Error += GLM_CHECK_NEAR(glm::safeNormalize(A), vec3(0.6, 0, 0.8), Eps);
	Error += GLM_CHECK_NEAR(glm::safeNormalize(A, vec3(0, 0, 1)), vec3(0.6, 0, 0.8), Eps);
	Error += GLM_CHECK_NEAR(glm::safeNormalize(vec2(0, -2)), vec2(0, -1), Eps);
	Error += GLM_CHECK_NEAR(glm::safeNormalize(vec4(1, 1, 1, 1)), vec4(0.5), Eps);

	// So does a short one, as long as its squared length is above epsilon.
	T const Short = std::sqrt(std::numeric_limits<T>::epsilon()) * static_cast<T>(4);
	Error += GLM_CHECK_NEAR(glm::safeNormalize(vec3(0, Short, 0)), vec3(0, 1, 0), Eps);

	// A zero (or too short) vector gives a zero vector, or the fallback.
	vec3 const Zero(0);
	Error += GLM_CHECK_EQ(glm::safeNormalize(Zero), Zero);
	Error += GLM_CHECK_EQ(glm::safeNormalize(Zero, vec3(0, 0, 1)), vec3(0, 0, 1));
	T const Tiny = std::sqrt(std::numeric_limits<T>::epsilon()) / static_cast<T>(4);
	Error += GLM_CHECK_EQ(glm::safeNormalize(vec3(Tiny, 0, 0)), Zero);
	Error += GLM_CHECK_EQ(glm::safeNormalize(vec2(0)), vec2(0));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_safeNormalize<float>());
	Error += GLM_TEST(test_safeNormalize<double>());

	return GLM_TEST_FINISH(Error);
}
