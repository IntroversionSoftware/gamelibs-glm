#include <glm/gtc/constants.hpp>
#include "glm_test.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/glm.hpp>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

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

static int test_quat_angle()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0

		float A = glm::angle(N);
		Error += GLM_CHECK_NEAR(A, glm::pi<float>() * 0.25f, Epsilon); // Angle should be π/4
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::normalize(glm::vec3(0, 1, 1)));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0

		float A = glm::angle(N);
		Error += GLM_CHECK_NEAR(A, glm::pi<float>() * 0.25f, Epsilon); // Angle should be π/4
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::normalize(glm::vec3(1, 2, 3)));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0

		float A = glm::angle(N);
		Error += GLM_CHECK_NEAR(A, glm::pi<float>() * 0.25f, Epsilon); // Angle should be π/4
	}

	return Error;
}

static int test_quat_angleAxis()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	glm::quat A = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::mix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	Error += GLM_CHECK_NEAR(C, D, Epsilon);

	return Error;
}

static int test_quat_mix()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	glm::quat A = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::mix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	Error += GLM_CHECK_NEAR(C, D, Epsilon);

	return Error;
}

static int test_quat_normalize()
{
	int Error(0);
	const float Epsilon = 0.000001f;


	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 2));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(1, 2, 3));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Normalized length should be 1.0
	}

	return Error;
}

static int test_quat_euler()
{
	int Error = 0;
	const float Epsilon = 0.000001f;


	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		float Roll = glm::roll(q);
		float Pitch = glm::pitch(q);
		float Yaw = glm::yaw(q);
		glm::vec3 Angles = glm::eulerAngles(q);

		Error += GLM_CHECK_NEAR(Angles, glm::vec3(Pitch, Yaw, Roll), Epsilon); // Euler angles don't match individual components
	}

	{
		glm::dquat q = make_quat<double>(1.0, 0.0, 0.0, 1.0);
		double Roll = glm::roll(q);
		double Pitch = glm::pitch(q);
		double Yaw = glm::yaw(q);
		glm::dvec3 Angles = glm::eulerAngles(q);

		Error += GLM_CHECK_NEAR(Angles, glm::dvec3(Pitch, Yaw, Roll), 0.000001); // Double-precision euler angles don't match components
	}

	return Error;
}

static int test_quat_slerp()
{
	int Error = 0;
	float const Epsilon = 0.0001f;


	float sqrt2 = std::sqrt(2.0f) / 2.0f;
	glm::quat id = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat Y90rot = make_quat(sqrt2, 0.0f, sqrt2, 0.0f);

	// Testing a == 0
	// Must be id
	glm::quat id2 = glm::slerp(id, Y90rot, 0.0f);
	Error += GLM_CHECK_NEAR(id, id2, Epsilon); // slerp with t=0 should return first quaternion

	// Testing a == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot2 = glm::slerp(id, Y90rot, 1.0f);
	Error += GLM_CHECK_NEAR(Y90rot, Y90rot2, Epsilon); // slerp with t=1 should return second quaternion

	// Testing standard, easy case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	glm::quat Y45rot1 = glm::slerp(id, Y90rot, 0.5f);
	Error += GLM_CHECK_NEAR(Y45rot1, make_quat(0.924f, 0.0f, 0.383f, 0.0f), 0.01f); // slerp with t=0.5 doesn't produce expected result

	// Testing reverse case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	glm::quat Ym45rot2 = glm::slerp(Y90rot, id, 0.5f);

	// Testing against full circle around the sphere instead of shortest path
	// Must be 45 degrees rotation on Y
	// certainly not a 135 degrees rotation
	glm::quat Y45rot3 = glm::slerp(id, -Y90rot, 0.5f);
	float Y45angle3 = glm::angle(Y45rot3);

	Error += GLM_CHECK_NEAR(Y45angle3, glm::pi<float>() * 0.25f, Epsilon); // Angle of slerp result incorrect

	Error += GLM_CHECK_NEAR(Ym45rot2, Y45rot3, Epsilon); // slerp results don't match for equivalent inputs

	// Same, but inverted
	// Must also be 45 degrees rotation on Y :  0 0.38 0 0.92
	// -0 -0.38 -0 -0.92 is ok too
	glm::quat Y45rot4 = glm::slerp(-Y90rot, id, 0.5f);
	Error += GLM_CHECK_NEAR(Ym45rot2, -Y45rot4, Epsilon); // slerp results don't match with negated input

	// Testing q1 = q2
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot3 = glm::slerp(Y90rot, Y90rot, 0.5f);
	Error += GLM_CHECK_NEAR(Y90rot, Y90rot3, Epsilon); // slerp with equal quaternions should return the same quaternion

	// Testing 180 degrees rotation
	// Must be 90 degrees rotation on almost any axis that is on the XZ plane
	glm::quat XZ90rot = glm::slerp(id, -Y90rot, 0.5f);
	float XZ90angle = glm::angle(XZ90rot); // Must be PI/4 = 0.78;

	Error += GLM_CHECK_NEAR(XZ90angle, glm::pi<float>() * 0.25f, Epsilon); // Angle for 180-degree case incorrect

	// Testing almost equal quaternions (this test should pass through the linear interpolation)
	// Must be 0 0.00X 0 0.99999
	glm::quat almostid = glm::slerp(id, glm::angleAxis(0.1f, glm::vec3(0.0f, 1.0f, 0.0f)), 0.5f);
	Error += GLM_CHECK_NEAR(almostid, make_quat(1.0f, 0.0f, 0.0f, 0.0f), 0.1f); // slerp with nearly identical quaternions doesn't approach identity

	// Testing quaternions with opposite sign
	{
		glm::quat a = make_quat(-1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat result = glm::slerp(a, id, 0.5f);

		Error += GLM_CHECK_NEAR(glm::pow(glm::dot(id, result), 2.f), 1.f, 0.01f); // Dot product squared with identity should be 1
	}

	return Error;
}

static int test_quat_slerp_spins()
{
	int Error = 0;
	float const Epsilon = 0.0001f;


	float sqrt2 = std::sqrt(2.0f) / 2.0f;
	glm::quat id = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat Y90rot = make_quat(sqrt2, 0.0f, sqrt2, 0.0f);
	glm::quat Y180rot = make_quat(0.0f, 0.0f, 1.0f, 0.0f);

	// Testing a == 0, k == 1
	// Must be id
	glm::quat id2 = glm::slerp(id, id, 1.0f, 1);
	Error += GLM_CHECK_NEAR(id, id2, Epsilon); // slerp_spins with t=1, k=1 and identical quaternions should return input

	// Testing a == 1, k == 2
	// Must be id
	glm::quat id3 = glm::slerp(id, id, 1.0f, 2);
	Error += GLM_CHECK_NEAR(id, id3, Epsilon); // slerp_spins with t=1, k=2 and identical quaternions should return input

	// Testing a == 1, k == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	// Negative quaternion is representing same orientation
	glm::quat Y90rot2 = glm::slerp(id, Y90rot, 1.0f, 1);
	Error += GLM_CHECK_NEAR(Y90rot, -Y90rot2, Epsilon); // slerp_spins result incorrect

	// Testing a == 1, k == 2
	// Must be id
	glm::quat Y90rot3 = glm::slerp(id, Y90rot, 8.0f / 9.0f, 2);
	Error += GLM_CHECK_NEAR(id, Y90rot3, Epsilon); // slerp_spins with t=8/9, k=2 should return identity

	// Testing a == 1, k == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot4 = glm::slerp(id, Y90rot, 0.2f, 1);
	Error += GLM_CHECK_NEAR(Y90rot, Y90rot4, Epsilon); // slerp_spins with t=0.2, k=1 should equal second parameter

	// Testing reverse case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	// Negative quaternion is representing same orientation
	glm::quat Ym45rot2 = glm::slerp(Y90rot, id, 0.9f, 1);
	glm::quat Ym45rot3 = glm::slerp(Y90rot, id, 0.5f);
	Error += GLM_CHECK_NEAR(-Ym45rot2, Ym45rot3, Epsilon); // slerp_spins doesn't match slerp for appropriate inputs

	// Testing against full circle around the sphere instead of shortest path
	// Must be 45 degrees rotation on Y
	// certainly not a 135 degrees rotation
	glm::quat Y45rot3 = glm::slerp(id, -Y90rot, 0.5f, 0);
	float Y45angle3 = glm::angle(Y45rot3);
	Error += GLM_CHECK_NEAR(Y45angle3, glm::pi<float>() * 0.25f, Epsilon); // slerp_spins angle incorrect

	Error += GLM_CHECK_NEAR(Ym45rot3, Y45rot3, Epsilon); // slerp_spins results don't match expected values

	// Same, but inverted
	// Must also be 45 degrees rotation on Y :  0 0.38 0 0.92
	// -0 -0.38 -0 -0.92 is ok too
	glm::quat Y45rot4 = glm::slerp(-Y90rot, id, 0.5f, 0);
	Error += GLM_CHECK_NEAR(Ym45rot2, Y45rot4, Epsilon); // slerp_spins with negated input doesn't match expected result

	// Testing q1 = q2 k == 2
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot5 = glm::slerp(Y90rot, Y90rot, 0.5f, 2);
	Error += GLM_CHECK_NEAR(Y90rot, Y90rot5, Epsilon); // slerp_spins with identical quaternions should return input

	// Testing 180 degrees rotation
	// Must be 90 degrees rotation on almost any axis that is on the XZ plane
	glm::quat XZ90rot = glm::slerp(id, -Y90rot, 0.5f, 1);
	float XZ90angle = glm::angle(XZ90rot); // Must be PI*1.25 = 3.92;
	Error += GLM_CHECK_NEAR(XZ90angle, glm::pi<float>() * 1.25f, Epsilon); // slerp_spins angle for 180-degree case incorrect

	// Testing rotation over long arc
	// Distance from id to 90 degrees is 270 degrees, so 2/3 of it should be 180 degrees
	// Negative quaternion is representing same orientation
	glm::quat Neg90rot = glm::slerp(id, Y90rot, 2.0f / 3.0f, -1);
	Error += GLM_CHECK_NEAR(Y180rot, -Neg90rot, Epsilon); // slerp_spins long arc rotation incorrect

	return Error;
}

static int test_quat_mul_vec()
{
	int Error(0);
	const float Epsilon = 0.01f;


	glm::quat q = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::vec3 v(1, 0, 0);
	glm::vec3 u(q * v);
	glm::vec3 w(u * q);

	Error += GLM_CHECK_NEAR(v, w, Epsilon); // Double rotation doesn't return to original vector

	return Error;
}

static int test_mul()
{
	int Error = 0;
	const float Epsilon = 0.01f;


	glm::quat temp1 = glm::normalize(glm::quat(1.0f, glm::vec3(0.0, 1.0, 0.0)));
	glm::quat temp2 = glm::normalize(glm::quat(0.5f, glm::vec3(1.0, 0.0, 0.0)));

	glm::vec3 transformed0 = (temp1 * glm::vec3(0.0, 1.0, 0.0) * glm::inverse(temp1));
	glm::vec3 temp4 = temp2 * transformed0 * glm::inverse(temp2);

	Error += GLM_CHECK_NEAR(temp4, glm::vec3(0.0f, -0.28f, -0.96f), Epsilon); // Combined rotation result incorrect

	glm::quat temp5 = glm::normalize(temp1 * temp2);
	glm::vec3 temp6 = temp5 * glm::vec3(0.0, 1.0, 0.0) * glm::inverse(temp5);

	Error += GLM_CHECK_NEAR(temp6, glm::vec3(-0.48f, 0.36f, -0.8f), Epsilon); // Combined quaternion rotation result incorrect

	glm::quat temp7(1.0f, glm::vec3(0.0, 1.0, 0.0));
	temp7 *= temp5;
	temp7 *= glm::inverse(temp5);

	Error += GLM_CHECK_NEAR(temp7, glm::quat(1.0f, glm::vec3(0.0, 1.0, 0.0)), 0.0001f); // rotation and inverse cancel out

	return Error;
}

static int test_identity()
{
	int Error = 0;
	const float Epsilon = 0.0001f;


	glm::quat const Q = glm::identity<glm::quat>();

	Error += GLM_CHECK_NEAR(Q, make_quat(1.0f, 0.0f, 0.0f, 0.0f), Epsilon); // Identity quaternion should be (1,0,0,0)

	Error += glm_test::context(GLM_CHECK(!glm::any(glm::notEqual(Q, make_quat(1.0f, 0.0f, 0.0f, 0.0f), Epsilon))), "Q", Q);

	glm::mat4 const M = glm::identity<glm::mat4x4>();
	glm::mat4 const N(1.0f);

	Error += GLM_CHECK_NEAR(M, N, Epsilon); // matrix identity doesn't match constructor with 1.0f

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_mul());
	Error += GLM_TEST(test_quat_mul_vec());
	Error += GLM_TEST(test_quat_angle());
	Error += GLM_TEST(test_quat_angleAxis());
	Error += GLM_TEST(test_quat_mix());
	Error += GLM_TEST(test_quat_normalize());
	Error += GLM_TEST(test_quat_euler());
	Error += GLM_TEST(test_quat_slerp());
	Error += GLM_TEST(test_quat_slerp_spins());
	Error += GLM_TEST(test_identity());

	return GLM_TEST_FINISH(Error);
}
