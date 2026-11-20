#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/scalar_relational.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/glm.hpp>

static int test_lessThan()
{
	int Error = 0;

	Error += GLM_CHECK(glm::lessThan(0, 1));
	Error += GLM_CHECK(!(glm::lessThan(1, 0)));
	Error += GLM_CHECK(!(glm::lessThan(0, 0)));
	Error += GLM_CHECK(!(glm::lessThan(1, 1)));
	Error += GLM_CHECK(glm::lessThan(0.0f, 1.0f));
	Error += GLM_CHECK(!(glm::lessThan(1.0f, 0.0f)));
	Error += GLM_CHECK(!(glm::lessThan(0.0f, 0.0f)));
	Error += GLM_CHECK(!(glm::lessThan(1.0f, 1.0f)));
	Error += GLM_CHECK(glm::lessThan(0.0, 1.0));
	Error += GLM_CHECK(!(glm::lessThan(1.0, 0.0)));
	Error += GLM_CHECK(!(glm::lessThan(0.0, 0.0)));
	Error += GLM_CHECK(!(glm::lessThan(1.0, 1.0)));

	return Error;
}

static int test_lessThanEqual()
{
	int Error = 0;

	Error += GLM_CHECK(glm::lessThanEqual(0, 1));
	Error += GLM_CHECK(!(glm::lessThanEqual(1, 0)));
	Error += GLM_CHECK(glm::lessThanEqual(0, 0));
	Error += GLM_CHECK(glm::lessThanEqual(1, 1));
	Error += GLM_CHECK(glm::lessThanEqual(0.0f, 1.0f));
	Error += GLM_CHECK(!(glm::lessThanEqual(1.0f, 0.0f)));
	Error += GLM_CHECK(glm::lessThanEqual(0.0f, 0.0f));
	Error += GLM_CHECK(glm::lessThanEqual(1.0f, 1.0f));
	Error += GLM_CHECK(glm::lessThanEqual(0.0, 1.0));
	Error += GLM_CHECK(!(glm::lessThanEqual(1.0, 0.0)));
	Error += GLM_CHECK(glm::lessThanEqual(0.0, 0.0));
	Error += GLM_CHECK(glm::lessThanEqual(1.0, 1.0));

	return Error;
}

static int test_greaterThan()
{
	int Error = 0;

	Error += GLM_CHECK(!(glm::greaterThan(0, 1)));
	Error += GLM_CHECK(glm::greaterThan(1, 0));
	Error += GLM_CHECK(!(glm::greaterThan(0, 0)));
	Error += GLM_CHECK(!(glm::greaterThan(1, 1)));
	Error += GLM_CHECK(!(glm::greaterThan(0.0f, 1.0f)));
	Error += GLM_CHECK(glm::greaterThan(1.0f, 0.0f));
	Error += GLM_CHECK(!(glm::greaterThan(0.0f, 0.0f)));
	Error += GLM_CHECK(!(glm::greaterThan(1.0f, 1.0f)));
	Error += GLM_CHECK(!(glm::greaterThan(0.0, 1.0)));
	Error += GLM_CHECK(glm::greaterThan(1.0, 0.0));
	Error += GLM_CHECK(!(glm::greaterThan(0.0, 0.0)));
	Error += GLM_CHECK(!(glm::greaterThan(1.0, 1.0)));

	return Error;
}

static int test_greaterThanEqual()
{
	int Error = 0;

	Error += GLM_CHECK(!(glm::greaterThanEqual(0, 1)));
	Error += GLM_CHECK(glm::greaterThanEqual(1, 0));
	Error += GLM_CHECK(glm::greaterThanEqual(0, 0));
	Error += GLM_CHECK(glm::greaterThanEqual(1, 1));
	Error += GLM_CHECK(!(glm::greaterThanEqual(0.0f, 1.0f)));
	Error += GLM_CHECK(glm::greaterThanEqual(1.0f, 0.0f));
	Error += GLM_CHECK(glm::greaterThanEqual(0.0f, 0.0f));
	Error += GLM_CHECK(glm::greaterThanEqual(1.0f, 1.0f));
	Error += GLM_CHECK(!(glm::greaterThanEqual(0.0, 1.0)));
	Error += GLM_CHECK(glm::greaterThanEqual(1.0, 0.0));
	Error += GLM_CHECK(glm::greaterThanEqual(0.0, 0.0));
	Error += GLM_CHECK(glm::greaterThanEqual(1.0, 1.0));

	return Error;
}

static int test_equal()
{
	int Error = 0;

	Error += GLM_CHECK(!(glm::equal(0, 1)));
	Error += GLM_CHECK(!(glm::equal(1, 0)));
	Error += GLM_CHECK(glm::equal(0, 0));
	Error += GLM_CHECK(glm::equal(1, 1));
	Error += GLM_CHECK(!(glm::equal(0.0f, 1.0f, glm::epsilon<float>())));
	Error += GLM_CHECK(!(glm::equal(1.0f, 0.0f, glm::epsilon<float>())));
	Error += GLM_CHECK_NEAR(0.0f, 0.0f, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(1.0f, 1.0f, glm::epsilon<float>());
	Error += GLM_CHECK(!(glm::equal(0.0, 1.0, glm::epsilon<double>())));
	Error += GLM_CHECK(!(glm::equal(1.0, 0.0, glm::epsilon<double>())));
	Error += GLM_CHECK_NEAR(0.0, 0.0, glm::epsilon<double>());
	Error += GLM_CHECK_NEAR(1.0, 1.0, glm::epsilon<double>());

	return Error;
}

static int test_notEqual()
{
	int Error = 0;

	Error += GLM_CHECK(glm::notEqual(0, 1));
	Error += GLM_CHECK(glm::notEqual(1, 0));
	Error += GLM_CHECK(!(glm::notEqual(0, 0)));
	Error += GLM_CHECK(!(glm::notEqual(1, 1)));
	Error += GLM_CHECK(glm::notEqual(0.0f, 1.0f, glm::epsilon<float>()));
	Error += GLM_CHECK(glm::notEqual(1.0f, 0.0f, glm::epsilon<float>()));
	Error += GLM_CHECK(!(glm::notEqual(0.0f, 0.0f, glm::epsilon<float>())));
	Error += GLM_CHECK(!(glm::notEqual(1.0f, 1.0f, glm::epsilon<float>())));
	Error += GLM_CHECK(glm::notEqual(0.0, 1.0, glm::epsilon<double>()));
	Error += GLM_CHECK(glm::notEqual(1.0, 0.0, glm::epsilon<double>()));
	Error += GLM_CHECK(!(glm::notEqual(0.0, 0.0, glm::epsilon<double>())));
	Error += GLM_CHECK(!(glm::notEqual(1.0, 1.0, glm::epsilon<double>())));

	return Error;
}

static int test_any()
{
	int Error = 0;

	Error += GLM_CHECK(glm::any(true));
	Error += GLM_CHECK(!(glm::any(false)));

	return Error;
}

static int test_all()
{
	int Error = 0;

	Error += GLM_CHECK(glm::all(true));
	Error += GLM_CHECK(!(glm::all(false)));

	return Error;
}

static int test_not()
{
	int Error = 0;

	Error += GLM_CHECK(!(glm::not_(true)));
	Error += GLM_CHECK(glm::not_(false));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_lessThan());
	Error += GLM_TEST(test_lessThanEqual());
	Error += GLM_TEST(test_greaterThan());
	Error += GLM_TEST(test_greaterThanEqual());
	Error += GLM_TEST(test_equal());
	Error += GLM_TEST(test_notEqual());
	Error += GLM_TEST(test_any());
	Error += GLM_TEST(test_all());
	Error += GLM_TEST(test_not());

	return GLM_TEST_FINISH(Error);
}
