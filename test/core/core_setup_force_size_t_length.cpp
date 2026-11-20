#define GLM_FORCE_SIZE_T_LENGTH
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

template <typename genType>
genType add(genType const& a, genType const& b)
{
	genType result(0);
	for(glm::length_t i = 0; i < a.length(); ++i)
		result[i] = a[i] + b[i];
	return result;
}

int main()
{
	int Error = 0;

	glm::ivec4 v(1);
	Error += GLM_CHECK(add(v, v) == glm::ivec4(2));

	return GLM_TEST_FINISH(Error);
}
