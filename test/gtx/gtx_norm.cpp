#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/norm.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

static int test_lMaxNorm()
{
	int Error(0);

	{
		float norm = glm::lMaxNorm(glm::vec3(-1, -2, -3));
		Error += GLM_CHECK(glm::equal(norm, 3.f, 0.00001f));
	}

	{
		float norm = glm::lMaxNorm(glm::vec3(2, 3, 1));
		Error += GLM_CHECK(glm::equal(norm, 3.f, 0.00001f));
	}

	return Error;
}

static int test_lxNorm()
{
	int Error(0);

	{
		unsigned int depth_1 = 1;
		float normA = glm::lxNorm(glm::vec3(2, 3, 1), depth_1);
		float normB = glm::l1Norm(glm::vec3(2, 3, 1));
		Error += GLM_CHECK(glm::equal(normA, normB, 0.00001f));
		Error += GLM_CHECK(glm::equal(normA, 6.f, 0.00001f));
	}

	{
		unsigned int depth_1 = 1;
		float normA = glm::lxNorm(glm::vec3(-1, -2, -3), depth_1);
		float normB = glm::l1Norm(glm::vec3(-1, -2, -3));
		Error += GLM_CHECK(glm::equal(normA, normB, 0.00001f));
		Error += GLM_CHECK(glm::equal(normA, 6.f, 0.00001f));
	}

	{
		unsigned int depth_2 = 2;
		float normA = glm::lxNorm(glm::vec3(2, 3, 1), depth_2);
		float normB = glm::l2Norm(glm::vec3(2, 3, 1));
		Error += GLM_CHECK(glm::equal(normA, normB, 0.00001f));
		Error += GLM_CHECK(glm::equal(normA, 3.741657387f, 0.00001f));
	}

	{
		unsigned int depth_2 = 2;
		float normA = glm::lxNorm(glm::vec3(-1, -2, -3), depth_2);
		float normB = glm::l2Norm(glm::vec3(-1, -2, -3));
		Error += GLM_CHECK(glm::equal(normA, normB, 0.00001f));
		Error += GLM_CHECK(glm::equal(normA, 3.741657387f, 0.00001f));
	}

	{
		unsigned int oddDepth = 3;
		float norm = glm::lxNorm(glm::vec3(2, 3, 1), oddDepth);
		Error += GLM_CHECK(glm::equal(norm, 3.301927249f, 0.00001f));
	}

	{
		unsigned int oddDepth = 3;
		float norm = glm::lxNorm(glm::vec3(-1, -2, -3), oddDepth);
		Error += GLM_CHECK(glm::equal(norm, 3.301927249f, 0.00001f));
	}

	return Error;
}

int main()
{
	int Error(0);

	Error += GLM_TEST(test_lMaxNorm());
	Error += GLM_TEST(test_lxNorm());

	return GLM_TEST_FINISH(Error);
}
