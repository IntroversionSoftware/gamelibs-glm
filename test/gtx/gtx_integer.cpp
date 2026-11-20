#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/exponential.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/integer.hpp>
#include <cstdio>
/*
static int test_floor_log2()
{
	int Error = 0;

	for(std::size_t i = 1; i < 1000000; ++i)
	{
		glm::uint A = glm::floor_log2(glm::uint(i));
		glm::uint B = glm::uint(glm::floor(glm::log2(double(i)))); // Will fail with float, lack of accuracy

		Error += GLM_CHECK_EQ(A, B);
		assert(!Error);
	}

	return Error;
}
*/
static int test_log2()
{
	int Error = 0;

	for(std::size_t i = 1; i < 24; ++i)
	{
		glm::uint A = glm::log2(glm::uint(1 << i));
		glm::uint B = glm::uint(glm::log2(double(1 << i)));

		// Powers of two: both are exact
		Error += GLM_CHECK_EQ(A, B);
	}

	return Error;
}

static int test_nlz()
{
	int Error = 0;

	for(glm::uint i = 1; i < glm::uint(33); ++i)
		Error += GLM_CHECK(glm::nlz(i) == 31u - static_cast<glm::uint>(glm::findMSB(i)));
		//printf("%d, %d\n", glm::nlz(i), 31u - glm::findMSB(i));

	return Error;
}

static int test_pow_uint()
{
	int Error = 0;

	glm::uint const p0 = glm::pow(2u, 0u);
	Error += GLM_CHECK_EQ(p0, 1u);

	glm::uint const p1 = glm::pow(2u, 1u);
	Error += GLM_CHECK_EQ(p1, 2u);

	glm::uint const p2 = glm::pow(2u, 2u);
	Error += GLM_CHECK_EQ(p2, 4u);

	return Error;
}

static int test_pow_int()
{
	int Error = 0;

	int const p0 = glm::pow(2, 0u);
	Error += GLM_CHECK_EQ(p0, 1);

	int const p1 = glm::pow(2, 1u);
	Error += GLM_CHECK_EQ(p1, 2);

	int const p2 = glm::pow(2, 2u);
	Error += GLM_CHECK_EQ(p2, 4);

	int const p0n = glm::pow(-2, 0u);
	Error += GLM_CHECK_EQ(p0n, -1);

	int const p1n = glm::pow(-2, 1u);
	Error += GLM_CHECK_EQ(p1n, -2);

	int const p2n = glm::pow(-2, 2u);
	Error += GLM_CHECK_EQ(p2n, 4);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_nlz());
//	Error += test_floor_log2();
	Error += GLM_TEST(test_log2());
	Error += GLM_TEST(test_pow_uint());
	Error += GLM_TEST(test_pow_int());

	return GLM_TEST_FINISH(Error);
}

