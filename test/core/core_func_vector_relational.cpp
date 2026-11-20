#include "glm_test.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtc/vec1.hpp>

static int test_not()
{
	int Error = 0;

	{
		glm::bvec1 v(false);
		Error += GLM_CHECK(glm::all(glm::not_(v)));
	}

	{
		glm::bvec2 v(false);
		Error += GLM_CHECK(glm::all(glm::not_(v)));
	}

	{
		glm::bvec3 v(false);
		Error += GLM_CHECK(glm::all(glm::not_(v)));
	}

	{
		glm::bvec4 v(false);
		Error += GLM_CHECK(glm::all(glm::not_(v)));
	}

	return Error;
}

static int test_less()
{
	int Error = 0;

	{
		glm::vec2 const A(1, 2);
		glm::vec2 const B(2, 3);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, B)));
	}

	{
		glm::vec3 const A(1, 2, 3);
		glm::vec3 const B(2, 3, 4);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, B)));
	}

	{
		glm::vec4 const A(1, 2, 3, 4);
		glm::vec4 const B(2, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, B)));
	}

	{
		glm::ivec2 const A(1, 2);
		glm::ivec2 const B(2, 3);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));

		glm::ivec2 const C(1, 3);
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, C)));
	}

	{
		glm::ivec3 const A(1, 2, 3);
		glm::ivec3 const B(2, 3, 4);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));

		glm::ivec3 const C(1, 3, 4);
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, C)));
	}

	{
		glm::ivec4 const A(1, 2, 3, 4);
		glm::ivec4 const B(2, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::lessThan(A, B)));

		glm::ivec4 const C(1, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::lessThanEqual(A, C)));
	}

	return Error;
}

static int test_greater()
{
	int Error = 0;

	{
		glm::vec2 const A(1, 2);
		glm::vec2 const B(2, 3);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(B, A)));
	}

	{
		glm::vec3 const A(1, 2, 3);
		glm::vec3 const B(2, 3, 4);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(B, A)));
	}

	{
		glm::vec4 const A(1, 2, 3, 4);
		glm::vec4 const B(2, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(B, A)));
	}

	{
		glm::ivec2 const A(1, 2);
		glm::ivec2 const B(2, 3);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));

		glm::ivec2 const C(1, 3);
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(C, A)));
	}

	{
		glm::ivec3 const A(1, 2, 3);
		glm::ivec3 const B(2, 3, 4);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));

		glm::ivec3 const C(1, 3, 4);
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(C, A)));
	}

	{
		glm::ivec4 const A(1, 2, 3, 4);
		glm::ivec4 const B(2, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::greaterThan(B, A)));

		glm::ivec4 const C(1, 3, 4, 5);
		Error += GLM_CHECK(glm::all(glm::greaterThanEqual(C, A)));
	}

	return Error;
}

static int test_equal()
{
	int Error = 0;

	{
		glm::ivec2 const A(1, 2);
		glm::ivec2 const B(1, 2);
		Error += GLM_CHECK(glm::all(glm::equal(B, A)));
	}

	{
		glm::ivec3 const A(1, 2, 3);
		glm::ivec3 const B(1, 2, 3);
		Error += GLM_CHECK(glm::all(glm::equal(B, A)));
	}

	{
		glm::ivec4 const A(1, 2, 3, 4);
		glm::ivec4 const B(1, 2, 3, 4);
		Error += GLM_CHECK(glm::all(glm::equal(B, A)));
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_not());
	Error += GLM_TEST(test_less());
	Error += GLM_TEST(test_greater());
	Error += GLM_TEST(test_equal());

	return GLM_TEST_FINISH(Error);
}

