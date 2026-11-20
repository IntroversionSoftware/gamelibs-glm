#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <limits>

static int test_adjugate()
{
	int Error = 0;

	const float epsilon = std::numeric_limits<float>::epsilon();

	// mat2
	const glm::mat2 m2(
		2, 3,
		1, 5
	);

	const glm::mat2 eam2(
		5, -3,
		-1, 2
	);

	const glm::mat2 am2 = glm::adjugate(m2);

	Error += GLM_CHECK_NEAR(am2, eam2, epsilon);

	// mat3
	const glm::mat3 m3(
		2, 3, 3,
		1, 5, 4,
		4, 6, 8
	);

	const glm::mat3 eam3(
		16, -6, -3,
		8, 4, -5,
		-14, 0, 7
	);

	const glm::mat3 am3 = glm::adjugate(m3);

	Error += GLM_CHECK_NEAR(am3, eam3, epsilon);

	// mat4
	const glm::mat4 m4(
		2, 3, 3, 1,
		1, 5, 4, 3,
		4, 6, 8, 5,
		-2, -3, -3, 4
	);

	const glm::mat4 eam4(
		97, -30, -15, 17,
		45, 20, -25, 5,
		-91, 0, 35, -21,
		14, 0, 0, 14
	);

	const glm::mat4 am4 = glm::adjugate(m4);

	Error += GLM_CHECK_NEAR(am4, eam4, epsilon);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_adjugate());

	return GLM_TEST_FINISH(Error);
}
