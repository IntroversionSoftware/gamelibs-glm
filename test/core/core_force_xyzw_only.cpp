#define GLM_FORCE_XYZW_ONLY

#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

static int test_comp()
{
	int Error = 0;

	{
		glm::ivec1 A(1);
		Error += GLM_CHECK(A.x == 1);
	}

	{
		glm::ivec2 A(1, 2);
		Error += GLM_CHECK(A.x == 1);
		Error += GLM_CHECK(A.y == 2);
	}

	{
		glm::ivec3 A(1, 2, 3);
		Error += GLM_CHECK(A.x == 1);
		Error += GLM_CHECK(A.y == 2);
		Error += GLM_CHECK(A.z == 3);
	}

	{
		glm::ivec4 A(1, 2, 3, 4);
		Error += GLM_CHECK(A.x == 1);
		Error += GLM_CHECK(A.y == 2);
		Error += GLM_CHECK(A.z == 3);
		Error += GLM_CHECK(A.w == 4);
	}

	return Error;
}

static int test_constexpr()
{
	int Error = 0;

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_comp());
	Error += GLM_TEST(test_constexpr());

	return GLM_TEST_FINISH(Error);
}
