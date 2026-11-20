#include "glm_test.hpp"
#include <glm/ext/scalar_reciprocal.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/scalar_constants.hpp>

static int test_sec()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::sec(0.0), 1.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sec(glm::pi<double>() * 2.0), 1.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sec(glm::pi<double>() * -2.0), 1.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sec(glm::pi<double>() * 1.0), -1.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sec(glm::pi<double>() * -1.0), -1.0, 0.01));

	return Error;
}

static int test_csc()
{
	int Error = 0;

	double const a = glm::csc(glm::pi<double>() * 0.5);
	Error += GLM_CHECK(glm::equal(a, 1.0, 0.01));
	double const b = glm::csc(glm::pi<double>() * -0.5);
	Error += GLM_CHECK(glm::equal(b, -1.0, 0.01));

	return Error;
}

static int test_cot()
{
	int Error = 0;

	double const a = glm::cot(glm::pi<double>() * 0.5);
	Error += GLM_CHECK(glm::equal(a, 0.0, 0.01));
	double const b = glm::cot(glm::pi<double>() * -0.5);
	Error += GLM_CHECK(glm::equal(b, 0.0, 0.01));

	return Error;
}

static int test_asec()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::asec(100000.0), glm::pi<double>() * 0.5, 0.01));
	Error += GLM_CHECK(glm::equal(glm::asec(-100000.0), glm::pi<double>() * 0.5, 0.01));

	return Error;
}

static int test_acsc()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::acsc(100000.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::acsc(-100000.0), 0.0, 0.01));

	return Error;
}

static int test_acot()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::acot(100000.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::acot(-100000.0), glm::pi<double>(), 0.01));
	Error += GLM_CHECK(glm::equal(glm::acot(0.0), glm::pi<double>() * 0.5, 0.01));

	return Error;
}

static int test_sech()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::sech(100000.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sech(-100000.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::sech(0.0), 1.0, 0.01));

	return Error;
}

static int test_csch()
{
	int Error = 0;

	Error += GLM_CHECK(glm::equal(glm::csch(100000.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::csch(-100000.0), 0.0, 0.01));

	return Error;
}

static int test_coth()
{
	int Error = 0;

	double const a = glm::coth(100.0);
	Error += GLM_CHECK(glm::equal(a, 1.0, 0.01));

	double const b = glm::coth(-100.0);
	Error += GLM_CHECK(glm::equal(b, -1.0, 0.01));

	return Error;
}

static int test_asech()
{
	int Error = 0;

	double const a = glm::asech(1.0);
	Error += GLM_CHECK(glm::equal(a, 0.0, 0.01));

	return Error;
}

static int test_acsch()
{
	int Error = 0;

	Error += GLM_CHECK(glm::acsch(0.01) > 1.0);
	Error += GLM_CHECK(glm::acsch(-0.01) < -1.0);

	Error += GLM_CHECK(glm::equal(glm::acsch(100.0), 0.0, 0.01));
	Error += GLM_CHECK(glm::equal(glm::acsch(-100.0), 0.0, 0.01));

	return Error;
}

static int test_acoth()
{
	int Error = 0;

	double const a = glm::acoth(1.00001);
	Error += GLM_CHECK(a > 6.0);

	double const b = glm::acoth(-1.00001);
	Error += GLM_CHECK(b < -6.0);

	double const c = glm::acoth(10000.0);
	Error += GLM_CHECK(glm::equal(c, 0.0, 0.01));

	double const d = glm::acoth(-10000.0);
	Error += GLM_CHECK(glm::equal(d, 0.0, 0.01));

	return Error;
}


int main()
{
	int Error = 0;

	Error += GLM_TEST(test_sec());
	Error += GLM_TEST(test_csc());
	Error += GLM_TEST(test_cot());

	Error += GLM_TEST(test_asec());
	Error += GLM_TEST(test_acsc());
	Error += GLM_TEST(test_acot());

	Error += GLM_TEST(test_sech());
	Error += GLM_TEST(test_csch());
	Error += GLM_TEST(test_coth());

	Error += GLM_TEST(test_asech());
	Error += GLM_TEST(test_acsch());
	Error += GLM_TEST(test_acoth());

	return GLM_TEST_FINISH(Error);
}
