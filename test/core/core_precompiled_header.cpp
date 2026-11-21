// Builds with all of GLM as its precompiled header (see test/core/CMakeLists.txt). Clang 22 and later
// crash writing a PCH with a compound assignment to a __declspec(property) whose object isn't
// type-dependent but whose right-hand side is, such as this->x += s in a member of a class template,
// so GLM's own code must not contain one where components are properties.

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main()
{
	int Error = 0;

	glm::vec3 v(1.0f, 2.0f, 3.0f);
	v += 1.0f;
	Error += GLM_CHECK_EQ(v, glm::vec3(2.0f, 3.0f, 4.0f));

	return GLM_TEST_FINISH(Error);
}
