#include "glm_test.hpp"
#include <glm/glm.hpp>

static int test_length_mat_non_squared()
{
	int Error = 0;

	Error += GLM_CHECK(glm::mat2x3().length() == 2);
	Error += GLM_CHECK(glm::mat2x4().length() == 2);
	Error += GLM_CHECK(glm::mat3x2().length() == 3);
	Error += GLM_CHECK(glm::mat3x4().length() == 3);
	Error += GLM_CHECK(glm::mat4x2().length() == 4);
	Error += GLM_CHECK(glm::mat4x3().length() == 4);

	Error += GLM_CHECK(glm::dmat2x3().length() == 2);
	Error += GLM_CHECK(glm::dmat2x4().length() == 2);
	Error += GLM_CHECK(glm::dmat3x2().length() == 3);
	Error += GLM_CHECK(glm::dmat3x4().length() == 3);
	Error += GLM_CHECK(glm::dmat4x2().length() == 4);
	Error += GLM_CHECK(glm::dmat4x3().length() == 4);

	return Error;
}

static int test_length_mat()
{
	int Error = 0;

	Error += GLM_CHECK(glm::mat2().length() == 2);
	Error += GLM_CHECK(glm::mat3().length() == 3);
	Error += GLM_CHECK(glm::mat4().length() == 4);
	Error += GLM_CHECK(glm::mat2x2().length() == 2);
	Error += GLM_CHECK(glm::mat3x3().length() == 3);
	Error += GLM_CHECK(glm::mat4x4().length() == 4);

	Error += GLM_CHECK(glm::dmat2().length() == 2);
	Error += GLM_CHECK(glm::dmat3().length() == 3);
	Error += GLM_CHECK(glm::dmat4().length() == 4);
	Error += GLM_CHECK(glm::dmat2x2().length() == 2);
	Error += GLM_CHECK(glm::dmat3x3().length() == 3);
	Error += GLM_CHECK(glm::dmat4x4().length() == 4);

	return Error;
}

static int test_length_vec()
{
	int Error = 0;

	Error += GLM_CHECK(glm::vec2().length() == 2);
	Error += GLM_CHECK(glm::vec3().length() == 3);
	Error += GLM_CHECK(glm::vec4().length() == 4);

	Error += GLM_CHECK(glm::ivec2().length() == 2);
	Error += GLM_CHECK(glm::ivec3().length() == 3);
	Error += GLM_CHECK(glm::ivec4().length() == 4);

	Error += GLM_CHECK(glm::uvec2().length() == 2);
	Error += GLM_CHECK(glm::uvec3().length() == 3);
	Error += GLM_CHECK(glm::uvec4().length() == 4);

	Error += GLM_CHECK(glm::dvec2().length() == 2);
	Error += GLM_CHECK(glm::dvec3().length() == 3);
	Error += GLM_CHECK(glm::dvec4().length() == 4);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_length_vec());
	Error += GLM_TEST(test_length_mat());
	Error += GLM_TEST(test_length_mat_non_squared());

	return GLM_TEST_FINISH(Error);
}

