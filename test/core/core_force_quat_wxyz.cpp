#define GLM_FORCE_QUAT_DATA_WXYZ
#define GLM_FORCE_INLINE

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// With the w, x, y, z layout, the quaternion operations (including the aligned SIMD ones, which
// take x, y, z, w lanes and must not be used here) give the same results as with x, y, z, w.
static int test_operations()
{
	int Error = 0;

	glm::quat const A = glm::quat::wxyz(1.0f, 2.0f, 3.0f, 4.0f);
	glm::quat const B = glm::quat::wxyz(5.0f, 6.0f, 7.0f, 8.0f);
	Error += GLM_CHECK(A[0] == A.w && A[1] == A.x);

	// Hamilton product
	glm::quat const C = A * B;
	Error += GLM_CHECK_EQ(C, glm::quat::wxyz(-60.0f, 12.0f, 30.0f, 24.0f));

	glm::quat D = A;
	D *= B;
	Error += GLM_CHECK_EQ(D, C);

	Error += GLM_CHECK_EQ(glm::conjugate(A), glm::quat::wxyz(1.0f, -2.0f, -3.0f, -4.0f));

	// A quarter turn about z takes x to y.
	glm::quat const R = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	Error += GLM_CHECK_NEAR(R * glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1e-6f);
	Error += GLM_CHECK_NEAR(glm::mat4_cast(R) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1e-6f);
	Error += GLM_CHECK_NEAR(glm::mat3_cast(R) * glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1e-6f);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_operations());

	return GLM_TEST_FINISH(Error);
}

