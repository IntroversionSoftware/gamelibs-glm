#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/matrix_query.hpp>

static int test_isNull()
{
	int Error(0);

	bool TestA = glm::isNull(glm::mat4(0), 0.00001f);
	Error += GLM_CHECK(TestA);

	return Error;
}

static int test_isIdentity()
{
	int Error(0);

	{
		bool TestA = glm::isIdentity(glm::mat2(1), 0.00001f);
		Error += GLM_CHECK(TestA);
	}
	{
		bool TestA = glm::isIdentity(glm::mat3(1), 0.00001f);
		Error += GLM_CHECK(TestA);
	}
	{
		bool TestA = glm::isIdentity(glm::mat4(1), 0.00001f);
		Error += GLM_CHECK(TestA);
	}

	return Error;
}

static int test_isNormalized()
{
	int Error(0);

	bool TestA = glm::isNormalized(glm::mat4(1), 0.00001f);
	Error += GLM_CHECK(TestA);

	return Error;
}

static int test_isOrthogonal()
{
	int Error(0);

	{
		bool TestA = glm::isOrthogonal(glm::mat4(1), 0.00001f);
		Error += GLM_CHECK(TestA);
	}
	{
		bool TestA = glm::isOrthogonal(glm::mat4(0), 0.00001f);
		Error += GLM_CHECK(!(TestA));
	}

	return Error;
}

int main()
{
	int Error(0);

	Error += GLM_TEST(test_isNull());
	Error += GLM_TEST(test_isIdentity());
	Error += GLM_TEST(test_isNormalized());
	Error += GLM_TEST(test_isOrthogonal());

	return GLM_TEST_FINISH(Error);
}


