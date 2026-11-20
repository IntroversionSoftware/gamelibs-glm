#define GLM_FORCE_EXPLICIT_CTOR

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main()
{
	int Error = 0;

	glm::ivec4 B(1);
	Error += GLM_CHECK_EQ(B, glm::ivec4(1));

	//glm::vec4 A = B;

	return GLM_TEST_FINISH(Error);
}

