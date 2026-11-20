#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// Distance between the rotations two unit quaternions represent: q and -q are the same rotation.
static float rotation_distance(glm::quat const& a, glm::quat const& b)
{
	return glm::min(glm::length(a - b), glm::length(a + b));
}

static int test_lookAt()
{
	int Error(0);
	float const Epsilon = glm::epsilon<float>();

	glm::vec3 eye(0.0f);
	glm::vec3 center(1.1f, -2.0f, 3.1416f);
	glm::vec3 up(-0.17f, 7.23f, -1.744f);
	glm::vec3 direction = glm::normalize(center - eye);

	// Default implementation (right-handed): a unit quaternion matching the lookAt matrix
	{
		glm::quat test_quat = glm::quatLookAt(direction, up);
		glm::quat test_mat = glm::conjugate(glm::quat_cast(glm::lookAt(eye, center, up)));

		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(test_quat), 1.0f, Epsilon), "test_quat", test_quat);
		Error += glm_test::context(GLM_CHECK_LE(rotation_distance(test_quat, test_mat), Epsilon), "test_quat", test_quat, "test_mat", test_mat);
	}

	// Left-handed implementation
	{
		glm::quat test_quatLH = glm::quatLookAtLH(direction, up);
		glm::quat test_matLH = glm::conjugate(glm::quat_cast(glm::lookAtLH(eye, center, up)));

		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(test_quatLH), 1.0f, Epsilon), "test_quatLH", test_quatLH);
		Error += glm_test::context(GLM_CHECK_LE(rotation_distance(test_quatLH, test_matLH), Epsilon), "test_quatLH", test_quatLH, "test_matLH", test_matLH);
	}

	// Right-handed implementation (explicit)
	{
		glm::quat test_quatRH = glm::quatLookAtRH(direction, up);
		glm::quat test_matRH = glm::conjugate(glm::quat_cast(glm::lookAtRH(eye, center, up)));

		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(test_quatRH), 1.0f, Epsilon), "test_quatRH", test_quatRH);
		Error += glm_test::context(GLM_CHECK_LE(rotation_distance(test_quatRH, test_matRH), Epsilon), "test_quatRH", test_quatRH, "test_matRH", test_matRH);
	}

	// LH and RH are different rotations
	{
		glm::quat test_quatLH = glm::quatLookAtLH(direction, up);
		glm::quat test_quatRH = glm::quatLookAtRH(direction, up);

		Error += glm_test::context(GLM_CHECK_GE(rotation_distance(test_quatLH, test_quatRH), Epsilon), "test_quatLH", test_quatLH, "test_quatRH", test_quatRH);
	}

	// With a well-defined up vector, the quaternion turns GLM's forward (-Z) to the direction.
	// (Degenerate up vectors are mathematically undefined, so they aren't tested.)
	{
		glm::vec3 good_up(0.0f, 1.0f, 0.0f);
		glm::quat test_quat = glm::quatLookAt(direction, good_up);

		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(test_quat), 1.0f, Epsilon), "test_quat", test_quat);

		glm::vec3 forward(0.0f, 0.0f, -1.0f);
		glm::vec3 rotated_forward = test_quat * forward;

		float alignment = glm::dot(glm::normalize(rotated_forward), direction);
		Error += glm_test::context(GLM_CHECK_GE(alignment, 0.999f), "direction", direction, "rotated_forward", rotated_forward);
	}

	return Error;
}

static int test_rotation()
{
	int Error(0);
	float const Epsilon = glm::epsilon<float>();

	// The rotation from one vector to another
	{
		glm::vec3 v1(1.0f, 0.0f, 0.0f);
		glm::vec3 v2(0.0f, 1.0f, 0.0f);

		glm::quat q = glm::rotation(v1, v2);
		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(q), 1.0f, Epsilon), "q", q);

		glm::vec3 result = q * v1;
		Error += glm_test::context(GLM_CHECK_LE(glm::distance(result, v2), Epsilon), "result", result, "v2", v2, "q", q);
	}

	// A rotation of pi/2 around Z
	{
		glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 v1(1.0f, 0.0f, 0.0f);
		glm::vec3 expected(0.0f, 1.0f, 0.0f);

		glm::vec3 result = q * v1;
		Error += glm_test::context(GLM_CHECK_LE(glm::distance(result, expected), Epsilon), "result", result, "expected", expected, "q", q);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_lookAt());
	Error += GLM_TEST(test_rotation());

	return GLM_TEST_FINISH(Error);
}
