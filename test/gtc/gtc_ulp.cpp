#include "glm_test.hpp"
#include <glm/gtc/ulp.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <limits>

static int test_ulp_float_dist()
{
	int Error = 0;

	float A = 1.0f;

	float B = glm::next_float(A);
	Error += GLM_CHECK(glm::notEqual(A, B, 0));
	float C = glm::prev_float(B);
	Error += GLM_CHECK_NEAR(A, C, 0);

	int D = glm::float_distance(A, B);
	Error += GLM_CHECK_EQ(D, 1);
	int E = glm::float_distance(A, C);
	Error += GLM_CHECK_EQ(E, 0);

	return Error;
}

static int test_ulp_float_step()
{
	int Error = 0;

	float A = 1.0f;

	for(int i = 10; i < 1000; i *= 10)
	{
		float B = glm::next_float(A, i);
		Error += GLM_CHECK(glm::notEqual(A, B, 0));
		float C = glm::prev_float(B, i);
		Error += GLM_CHECK_NEAR(A, C, 0);

		int D = glm::float_distance(A, B);
		Error += GLM_CHECK_EQ(D, i);
		int E = glm::float_distance(A, C);
		Error += GLM_CHECK_EQ(E, 0);
	}

	return Error;
}

static int test_ulp_double_dist()
{
	int Error = 0;

	double A = 1.0;

	double B = glm::next_float(A);
	Error += GLM_CHECK(glm::notEqual(A, B, 0));
	double C = glm::prev_float(B);
	Error += GLM_CHECK_NEAR(A, C, 0);

	glm::int64 const D = glm::float_distance(A, B);
	Error += GLM_CHECK_EQ(D, 1);
	glm::int64 const E = glm::float_distance(A, C);
	Error += GLM_CHECK_EQ(E, 0);

	return Error;
}

static int test_ulp_double_step()
{
	int Error = 0;

	double A = 1.0;

	for(int i = 10; i < 1000; i *= 10)
	{
		double B = glm::next_float(A, i);
		Error += GLM_CHECK(glm::notEqual(A, B, 0));
		double C = glm::prev_float(B, i);
		Error += GLM_CHECK_NEAR(A, C, 0);

		glm::int64 const D = glm::float_distance(A, B);
		Error += GLM_CHECK_EQ(D, i);
		glm::int64 const E = glm::float_distance(A, C);
		Error += GLM_CHECK_EQ(E, 0);
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


