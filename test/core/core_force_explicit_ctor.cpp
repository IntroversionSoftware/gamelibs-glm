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

	// gtc/integer's log2 of an integer vector converts findMSB's int vector explicitly.
	Error += GLM_CHECK_EQ(glm::log2(glm::uvec4(1u, 2u, 8u, 1024u)), glm::uvec4(0u, 1u, 3u, 10u));
	Error += GLM_CHECK_EQ(glm::log2(glm::ivec2(4, 16)), glm::ivec2(2, 4));

	return GLM_TEST_FINISH(Error);
}

