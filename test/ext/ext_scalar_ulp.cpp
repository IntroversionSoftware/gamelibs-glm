#include "glm_test.hpp"
#include <glm/ext/scalar_ulp.hpp>
#include <glm/ext/scalar_relational.hpp>

static int test_ulp_float_dist()
{
	int Error = 0;

	float A = 1.0f;

	float B = glm::nextFloat(A);
	Error += GLM_CHECK(glm::notEqual(A, B, 0));
	float C = glm::prevFloat(B);
	Error += GLM_CHECK(glm::equal(A, C, 0));

	int D = glm::floatDistance(A, B);
	Error += GLM_CHECK(D == 1);
	int E = glm::floatDistance(A, C);
	Error += GLM_CHECK(E == 0);

	return Error;
}

static int test_ulp_float_step()
{
	int Error = 0;

	float A = 1.0f;

	for(int i = 10; i < 1000; i *= 10)
	{
		float B = glm::nextFloat(A, i);
		Error += GLM_CHECK(glm::notEqual(A, B, 0));
		float C = glm::prevFloat(B, i);
		Error += GLM_CHECK(glm::equal(A, C, 0));

		int D = glm::floatDistance(A, B);
		Error += GLM_CHECK(D == i);
		int E = glm::floatDistance(A, C);
		Error += GLM_CHECK(E == 0);
	}

	return Error;
}

static int test_ulp_double_dist()
{
	int Error = 0;

	double A = 1.0;

	double B = glm::nextFloat(A);
	Error += GLM_CHECK(glm::notEqual(A, B, 0));
	double C = glm::prevFloat(B);
	Error += GLM_CHECK(glm::equal(A, C, 0));

	glm::int64 const D = glm::floatDistance(A, B);
	Error += GLM_CHECK(D == 1);
	glm::int64 const E = glm::floatDistance(A, C);
	Error += GLM_CHECK(E == 0);

	return Error;
}

static int test_ulp_double_step()
{
	int Error = 0;

	double A = 1.0;

	for(int i = 10; i < 1000; i *= 10)
	{
		double B = glm::nextFloat(A, i);
		Error += GLM_CHECK(glm::notEqual(A, B, 0));
		double C = glm::prevFloat(B, i);
		Error += GLM_CHECK(glm::equal(A, C, 0));

		glm::int64 const D = glm::floatDistance(A, B);
		Error += GLM_CHECK(D == i);
		glm::int64 const E = glm::floatDistance(A, C);
		Error += GLM_CHECK(E == 0);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_ulp_float_dist());
	Error += GLM_TEST(test_ulp_float_step());
	Error += GLM_TEST(test_ulp_double_dist());
	Error += GLM_TEST(test_ulp_double_step());

	return GLM_TEST_FINISH(Error);
}
