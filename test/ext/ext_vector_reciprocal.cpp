#include "glm_test.hpp"
#include <glm/ext/vector_reciprocal.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_double1.hpp>

static int test_sec()
{
	int Error = 0;

	glm::dvec1 const a = glm::sec(glm::dvec1(0.0));
	Error += GLM_CHECK_NEAR(a, glm::dvec1(1.0), 0.01);

	glm::dvec1 const b = glm::sec(glm::dvec1(glm::pi<double>() * 2.0));
	Error += GLM_CHECK_NEAR(b, glm::dvec1(1.0), 0.01);

	glm::dvec1 const c = glm::sec(glm::dvec1(glm::pi<double>() * -2.0));
	Error += GLM_CHECK_NEAR(c, glm::dvec1(1.0), 0.01);

	glm::dvec1 const d = glm::sec(glm::dvec1(glm::pi<double>() * 1.0));
	Error += GLM_CHECK_NEAR(d, -glm::dvec1(1.0), 0.01);

	glm::dvec1 const e = glm::sec(glm::dvec1(glm::pi<double>() * -1.0));
	Error += GLM_CHECK_NEAR(e, -glm::dvec1(1.0), 0.01);

	return Error;
}

static int test_csc()
{
	int Error = 0;

	glm::dvec1 const a = glm::csc(glm::dvec1(glm::pi<double>() * 0.5));
	Error += GLM_CHECK_NEAR(a, glm::dvec1(1.0), 0.01);

	glm::dvec1 const b = glm::csc(glm::dvec1(glm::pi<double>() * -0.5));
	Error += GLM_CHECK_NEAR(b, glm::dvec1(-1.0), 0.01);

	return Error;
}

static int test_cot()
{
	int Error = 0;

	glm::dvec1 const a = glm::cot(glm::dvec1(glm::pi<double>() * 0.5));
	Error += GLM_CHECK_NEAR(a, glm::dvec1(0.0), 0.01);

	glm::dvec1 const b = glm::cot(glm::dvec1(glm::pi<double>() * -0.5));
	Error += GLM_CHECK_NEAR(b, glm::dvec1(0.0), 0.01);

	return Error;
}

static int test_asec()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::asec(glm::dvec1(100000.0)), glm::dvec1(glm::pi<double>() * 0.5), 0.01);
	Error += GLM_CHECK_NEAR(glm::asec(glm::dvec1(-100000.0)), glm::dvec1(glm::pi<double>() * 0.5), 0.01);

	return Error;
}

static int test_acsc()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::acsc(glm::dvec1(100000.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::acsc(glm::dvec1(-100000.0)), glm::dvec1(0.0), 0.01);

	return Error;
}

static int test_acot()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::acot(glm::dvec1(100000.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::acot(glm::dvec1(-100000.0)), glm::dvec1(glm::pi<double>()), 0.01);
	Error += GLM_CHECK_NEAR(glm::acot(glm::dvec1(0.0)), glm::dvec1(glm::pi<double>() * 0.5), 0.01);

	return Error;
}

static int test_sech()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::sech(glm::dvec1(100000.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::sech(glm::dvec1(-100000.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::sech(glm::dvec1(0.0)), glm::dvec1(1.0), 0.01);

	return Error;
}

static int test_csch()
{
	int Error = 0;

	Error += GLM_CHECK_NEAR(glm::csch(glm::dvec1(100000.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::csch(glm::dvec1(-100000.0)), glm::dvec1(0.0), 0.01);

	return Error;
}

static int test_coth()
{
	int Error = 0;

	glm::dvec1 const a = glm::coth(glm::dvec1(100.0));
	Error += GLM_CHECK_NEAR(a, glm::dvec1(1.0), 0.01);

	glm::dvec1 const b = glm::coth(glm::dvec1(-100.0));
	Error += GLM_CHECK_NEAR(b, glm::dvec1(-1.0), 0.01);

	return Error;
}

static int test_asech()
{
	int Error = 0;

	glm::dvec1 const a = glm::asech(glm::dvec1(1.0));
	Error += GLM_CHECK_NEAR(a, glm::dvec1(0.0), 0.01);

	return Error;
}

static int test_acsch()
{
	int Error = 0;

	glm::dvec1 const a(glm::acsch(glm::dvec1(0.01)));
	Error += GLM_CHECK_GT(a.x, 1.0);

	glm::dvec1 const b(glm::acsch(glm::dvec1(-0.01)));
	Error += GLM_CHECK_LT(b.x, -1.0);

	Error += GLM_CHECK_NEAR(glm::acsch(glm::dvec1(100.0)), glm::dvec1(0.0), 0.01);
	Error += GLM_CHECK_NEAR(glm::acsch(glm::dvec1(-100.0)), glm::dvec1(0.0), 0.01);

	return Error;
}

static int test_acoth()
{
	int Error = 0;

	glm::dvec1 const a = glm::acoth(glm::dvec1(1.00001));
	Error += GLM_CHECK_GT(a.x, 6.0);

	glm::dvec1 const b = glm::acoth(glm::dvec1(-1.00001));
	Error += GLM_CHECK_LT(b.x, -6.0);

	glm::dvec1 const c = glm::acoth(glm::dvec1(10000.0));
	Error += GLM_CHECK_NEAR(c, glm::dvec1(0.0), 0.01);

	glm::dvec1 const d = glm::acoth(glm::dvec1(-10000.0));
	Error += GLM_CHECK_NEAR(d, glm::dvec1(0.0), 0.01);

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
