#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

// Helper function to create quaternions consistently regardless of layout
template <typename T = float, glm::qualifier Q = glm::defaultp>
static glm::qua<T, Q> make_quat(T w, T x, T y, T z) {
#ifdef GLM_FORCE_QUAT_DATA_XYZW
	// When GLM_FORCE_QUAT_DATA_XYZW is defined, constructor expects (x,y,z,w)
	return glm::qua<T, Q>(x, y, z, w);
#else
	// Default behavior with constructor expecting (w,x,y,z)
	return glm::qua<T, Q>(w, x, y, z);
#endif
}

static int test_quat_fastMix()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::fastMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	// Check all components at once
	bool componentsEqual =
		glm::epsilonEqual(C.x, D.x, Epsilon) &&
		glm::epsilonEqual(C.y, D.y, Epsilon) &&
		glm::epsilonEqual(C.z, D.z, Epsilon) &&
		glm::epsilonEqual(C.w, D.w, Epsilon);

	Error += glm_test::context(GLM_CHECK(componentsEqual), "C", C, "D", D);

	return Error;
}

static int test_quat_shortMix()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::shortMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	// Check all components at once
	bool componentsEqual =
		glm::epsilonEqual(C.x, D.x, Epsilon) &&
		glm::epsilonEqual(C.y, D.y, Epsilon) &&
		glm::epsilonEqual(C.z, D.z, Epsilon) &&
		glm::epsilonEqual(C.w, D.w, Epsilon);

	Error += glm_test::context(GLM_CHECK(componentsEqual), "C", C, "D", D);

	return Error;
}

static int test_orientation()
{
	int Error = 0;
	const float Epsilon = 0.0001f;


	// Test roll (rotation around x-axis)
	{
		// Create a quaternion representing a 90-degree roll
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::roll(q);

		Error += glm_test::context(GLM_CHECK_NEAR(p, glm::pi<float>() * 0.5f, Epsilon), "q", q);
	}

	// Test pitch (rotation around y-axis)
	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::pitch(q);

		Error += glm_test::context(GLM_CHECK_NEAR(p, 0.f, Epsilon), "q", q);
	}

	// Test yaw (rotation around z-axis)
	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		q = glm::normalize(q);

		float p = glm::yaw(q);

		Error += glm_test::context(GLM_CHECK_NEAR(p, 0.f, Epsilon), "q", q);
	}

	// Test with a combined rotation
	{
		// Create a quaternion with a rotation around all three axes
		glm::quat qRoll = glm::angleAxis(glm::radians(30.0f), glm::vec3(1, 0, 0));
		glm::quat qPitch = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
		glm::quat qYaw = glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 0, 1));

		glm::quat q = qYaw * qPitch * qRoll; // Order matters: yaw, then pitch, then roll

		float roll = glm::roll(q);
		float pitch = glm::pitch(q);
		float yaw = glm::yaw(q);

		// Away from gimbal lock the decomposition is unique. GLM names the angles about X, Y and Z
		// pitch, yaw and roll.
		Error += GLM_CHECK_NEAR(glm::degrees(pitch), 30.0f, 0.01f);
		Error += GLM_CHECK_NEAR(glm::degrees(yaw), 45.0f, 0.01f);
		Error += GLM_CHECK_NEAR(glm::degrees(roll), 60.0f, 0.01f);
	}

	return Error;
}

static int test_rotation()
{
	int Error = 0;
	const float Epsilon = glm::epsilon<float>();


	glm::vec3 v(1, 0, 0);
	glm::vec3 u(0, 1, 0);

	glm::quat Rotation = glm::rotation(v, u);
	float Angle = glm::angle(Rotation);

	Error += GLM_CHECK_LT(glm::abs(Angle - glm::pi<float>() * 0.5f), Epsilon); // Rotation angle incorrect

	// Additional test to verify the rotation actually works as expected
	glm::vec3 rotated = Rotation * v;
	Error += glm_test::context(GLM_CHECK(glm::all(glm::epsilonEqual(rotated, u, 0.0001f))), "v", v, "u", u, "rotated", rotated); // Rotation turns v to u

	return Error;
}

static int test_log()
{
	int Error = 0;
	const float Epsilon = 0.0001f;


	glm::vec3 v(1, 0, 0);
	glm::vec3 u(0, 1, 0);

	glm::quat q = glm::rotation(v, u);
	glm::quat p = glm::log(q);
	glm::quat r = glm::exp(p);

	Error += GLM_CHECK_NEAR(q, r, Epsilon); // exp(log(q)) should equal q

	// Additional test with multiple rotations
	for (float angle = 0.1f; angle < glm::pi<float>(); angle += 0.5f) {
		glm::quat rot = glm::angleAxis(angle, glm::normalize(glm::vec3(1, 2, 3)));
		glm::quat logRot = glm::log(rot);
		glm::quat expLogRot = glm::exp(logRot);

		Error += GLM_CHECK_NEAR(rot, expLogRot, Epsilon); // exp(log(q)) should equal q for angle
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_log());
	Error += GLM_TEST(test_rotation());
	Error += GLM_TEST(test_orientation());
	Error += GLM_TEST(test_quat_fastMix());
	Error += GLM_TEST(test_quat_shortMix());

	return GLM_TEST_FINISH(Error);
}
