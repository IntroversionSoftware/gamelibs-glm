#include "glm_test.hpp"
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vector_relational.hpp>

static int test_defined()
{
	int Error = 0;

	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(glm::epsilon<float>()))));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(glm::epsilon<float>()))));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(glm::epsilon<float>()))));

	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec2(1.0f), glm::vec2(2.0f), glm::vec2(glm::epsilon<float>()))));
	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec3(1.0f), glm::vec3(2.0f), glm::vec3(glm::epsilon<float>()))));
	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(glm::epsilon<float>()))));

	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec2(0.0f), glm::vec2(0.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec3(0.0f), glm::vec3(0.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::vec4(0.0f), glm::vec4(0.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonEqual(glm::quat(glm::vec3(0.0f)), glm::quat(glm::vec3(0.0f)), glm::epsilon<float>())));

	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec2(1.0f), glm::vec2(2.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec3(1.0f), glm::vec3(2.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::vec4(1.0f), glm::vec4(2.0f), glm::epsilon<float>())));
	Error += GLM_CHECK(glm::all(glm::epsilonNotEqual(glm::quat(glm::vec3(0.0f)), glm::quat(glm::vec3(1.0f)), glm::epsilon<float>())));

	return Error;
}

template<typename T>
static int test_equal()
{
	int Error = 0;

	{
		T A = glm::epsilon<T>();
		T B = glm::epsilon<T>();
		Error += GLM_CHECK(glm::epsilonEqual(A, B, glm::epsilon<T>() * T(2)));
	}

	{
		T A(0);
		T B = static_cast<T>(0) + glm::epsilon<T>();
		Error += GLM_CHECK(glm::epsilonEqual(A, B, glm::epsilon<T>() * T(2)));
	}

	{
		T A(0);
		T B = static_cast<T>(0) - glm::epsilon<T>();
		Error += GLM_CHECK(glm::epsilonEqual(A, B, glm::epsilon<T>() * T(2)));
	}

	{
		T A = static_cast<T>(0) + glm::epsilon<T>();
		T B = static_cast<T>(0);
		Error += GLM_CHECK(glm::epsilonEqual(A, B, glm::epsilon<T>() * T(2)));
	}

	{
		T A = static_cast<T>(0) - glm::epsilon<T>();
		T B = static_cast<T>(0);
		Error += GLM_CHECK(glm::epsilonEqual(A, B, glm::epsilon<T>() * T(2)));
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_defined());
	Error += GLM_TEST(test_equal<float>());
	Error += GLM_TEST(test_equal<double>());

	return GLM_TEST_FINISH(Error);
}


