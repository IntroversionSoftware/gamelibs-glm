#define GLM_FORCE_INLINE
#define GLM_PRECISION_HIGHP_FLOAT
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

static int test_mat()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::mat2), sizeof(glm::highp_mat2));
	Error += GLM_CHECK_EQ(sizeof(glm::mat3), sizeof(glm::highp_mat3));
	Error += GLM_CHECK_EQ(sizeof(glm::mat4), sizeof(glm::highp_mat4));

	Error += GLM_CHECK_EQ(sizeof(glm::mat2x2), sizeof(glm::highp_mat2x2));
	Error += GLM_CHECK_EQ(sizeof(glm::mat2x3), sizeof(glm::highp_mat2x3));
	Error += GLM_CHECK_EQ(sizeof(glm::mat2x4), sizeof(glm::highp_mat2x4));
	Error += GLM_CHECK_EQ(sizeof(glm::mat3x2), sizeof(glm::highp_mat3x2));
	Error += GLM_CHECK_EQ(sizeof(glm::mat3x3), sizeof(glm::highp_mat3x3));
	Error += GLM_CHECK_EQ(sizeof(glm::mat3x4), sizeof(glm::highp_mat3x4));
	Error += GLM_CHECK_EQ(sizeof(glm::mat4x2), sizeof(glm::highp_mat4x2));
	Error += GLM_CHECK_EQ(sizeof(glm::mat4x3), sizeof(glm::highp_mat4x3));
	Error += GLM_CHECK_EQ(sizeof(glm::mat4x4), sizeof(glm::highp_mat4x4));

	return Error;
}

static int test_vec()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::vec2), sizeof(glm::highp_vec2));
	Error += GLM_CHECK_EQ(sizeof(glm::vec3), sizeof(glm::highp_vec3));
	Error += GLM_CHECK_EQ(sizeof(glm::vec4), sizeof(glm::highp_vec4));

	return Error;
}

static int test_dvec()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::dvec2), sizeof(glm::highp_dvec2));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec3), sizeof(glm::highp_dvec3));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec4), sizeof(glm::highp_dvec4));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_mat());
	Error += GLM_TEST(test_vec());
	Error += GLM_TEST(test_dvec());

	return GLM_TEST_FINISH(Error);
}
