#include <glm/gtc/constants.hpp>
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
#include <iostream>

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

// Print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

// Print vector components for debugging
template <typename T, glm::qualifier Q>
static void print_vec(const std::string& label, const glm::vec<3, T, Q>& v) {
	std::cout << label << ": x=" << v.x << ", y=" << v.y << ", z=" << v.z << std::endl;
}

static int test_quat_angle()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "Testing quaternion angle:" << std::endl;

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}

		float A = glm::angle(N);
		if (!glm::equal(A, glm::pi<float>() * 0.25f, Epsilon)) {
			std::cout << "Error: Angle should be π/4, got " << A << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::normalize(glm::vec3(0, 1, 1)));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}

		float A = glm::angle(N);
		if (!glm::equal(A, glm::pi<float>() * 0.25f, Epsilon)) {
			std::cout << "Error: Angle should be π/4, got " << A << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::normalize(glm::vec3(1, 2, 3)));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}

		float A = glm::angle(N);
		if (!glm::equal(A, glm::pi<float>() * 0.25f, Epsilon)) {
			std::cout << "Error: Angle should be π/4, got " << A << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	return Error;
}

static int test_quat_angleAxis()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "\nTesting quaternion angleAxis:" << std::endl;

	glm::quat A = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::mix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	if (!glm::equal(C.x, D.x, Epsilon) ||
		!glm::equal(C.y, D.y, Epsilon) ||
		!glm::equal(C.z, D.z, Epsilon) ||
		!glm::equal(C.w, D.w, Epsilon)) {
		std::cout << "Error: Mixed quaternion doesn't match expected rotation" << std::endl;
		print_quat("Mixed result", C);
		print_quat("Expected", D);
		Error++;
	}

	return Error;
}

static int test_quat_mix()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "\nTesting quaternion mix:" << std::endl;

	glm::quat A = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::mix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));

	if (!glm::equal(C.x, D.x, Epsilon) ||
		!glm::equal(C.y, D.y, Epsilon) ||
		!glm::equal(C.z, D.z, Epsilon) ||
		!glm::equal(C.w, D.w, Epsilon)) {
		std::cout << "Error: Mixed quaternion doesn't match expected rotation" << std::endl;
		print_quat("Mixed result", C);
		print_quat("Expected", D);
		Error++;
	}

	return Error;
}

static int test_quat_normalize()
{
	int Error(0);
	const float Epsilon = 0.000001f;

	std::cout << "\nTesting quaternion normalize:" << std::endl;

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 2));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	{
		glm::quat Q = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(1, 2, 3));
		glm::quat N = glm::normalize(Q);
		float L = glm::length(N);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized length should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", N);
			Error++;
		}
	}

	return Error;
}

static int test_quat_euler()
{
	int Error = 0;
	const float Epsilon = 0.000001f;

	std::cout << "\nTesting quaternion euler angles:" << std::endl;

	{
		glm::quat q = make_quat(1.0f, 0.0f, 0.0f, 1.0f);
		float Roll = glm::roll(q);
		float Pitch = glm::pitch(q);
		float Yaw = glm::yaw(q);
		glm::vec3 Angles = glm::eulerAngles(q);

		if (!glm::all(glm::equal(Angles, glm::vec3(Pitch, Yaw, Roll), Epsilon))) {
			std::cout << "Error: Euler angles don't match individual components" << std::endl;
			std::cout << "Pitch: " << Pitch << ", Yaw: " << Yaw << ", Roll: " << Roll << std::endl;
			print_vec("eulerAngles", Angles);
			Error++;
		}
	}

	{
		glm::dquat q = make_quat<double>(1.0, 0.0, 0.0, 1.0);
		double Roll = glm::roll(q);
		double Pitch = glm::pitch(q);
		double Yaw = glm::yaw(q);
		glm::dvec3 Angles = glm::eulerAngles(q);

		if (!glm::all(glm::equal(Angles, glm::dvec3(Pitch, Yaw, Roll), 0.000001))) {
			std::cout << "Error: Double-precision euler angles don't match components" << std::endl;
			std::cout << "Pitch: " << Pitch << ", Yaw: " << Yaw << ", Roll: " << Roll << std::endl;
			std::cout << "Angles: " << Angles.x << ", " << Angles.y << ", " << Angles.z << std::endl;
			Error++;
		}
	}

	return Error;
}

static int test_quat_slerp()
{
	int Error = 0;
	float const Epsilon = 0.0001f;

	std::cout << "\nTesting quaternion slerp:" << std::endl;

	float sqrt2 = std::sqrt(2.0f) / 2.0f;
	glm::quat id = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat Y90rot = make_quat(sqrt2, 0.0f, sqrt2, 0.0f);

	// Testing a == 0
	// Must be id
	glm::quat id2 = glm::slerp(id, Y90rot, 0.0f);
	if (!glm::all(glm::equal(id, id2, Epsilon))) {
		std::cout << "Error: slerp with t=0 should return first quaternion" << std::endl;
		print_quat("Original", id);
		print_quat("Result", id2);
		Error++;
	}

	// Testing a == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot2 = glm::slerp(id, Y90rot, 1.0f);
	if (!glm::all(glm::equal(Y90rot, Y90rot2, Epsilon))) {
		std::cout << "Error: slerp with t=1 should return second quaternion" << std::endl;
		print_quat("Original", Y90rot);
		print_quat("Result", Y90rot2);
		Error++;
	}

	// Testing standard, easy case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	glm::quat Y45rot1 = glm::slerp(id, Y90rot, 0.5f);
	if (!glm::all(glm::equal(Y45rot1, make_quat(0.924f, 0.0f, 0.383f, 0.0f), 0.01f))) {
		std::cout << "Error: slerp with t=0.5 doesn't produce expected result" << std::endl;
		print_quat("Result", Y45rot1);
		print_quat("Expected", make_quat(0.924f, 0.0f, 0.383f, 0.0f));
		Error++;
	}

	// Testing reverse case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	glm::quat Ym45rot2 = glm::slerp(Y90rot, id, 0.5f);

	// Testing against full circle around the sphere instead of shortest path
	// Must be 45 degrees rotation on Y
	// certainly not a 135 degrees rotation
	glm::quat Y45rot3 = glm::slerp(id, -Y90rot, 0.5f);
	float Y45angle3 = glm::angle(Y45rot3);

	if (!glm::equal(Y45angle3, glm::pi<float>() * 0.25f, Epsilon)) {
		std::cout << "Error: Angle of slerp result incorrect" << std::endl;
		std::cout << "Expected: " << glm::pi<float>() * 0.25f << ", got: " << Y45angle3 << std::endl;
		Error++;
	}

	if (!glm::all(glm::equal(Ym45rot2, Y45rot3, Epsilon))) {
		std::cout << "Error: slerp results don't match for equivalent inputs" << std::endl;
		print_quat("Result 1", Ym45rot2);
		print_quat("Result 2", Y45rot3);
		Error++;
	}

	// Same, but inverted
	// Must also be 45 degrees rotation on Y :  0 0.38 0 0.92
	// -0 -0.38 -0 -0.92 is ok too
	glm::quat Y45rot4 = glm::slerp(-Y90rot, id, 0.5f);
	if (!glm::all(glm::equal(Ym45rot2, -Y45rot4, Epsilon))) {
		std::cout << "Error: slerp results don't match with negated input" << std::endl;
		print_quat("Result 1", Ym45rot2);
		print_quat("Result 2 (negated)", -Y45rot4);
		Error++;
	}

	// Testing q1 = q2
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot3 = glm::slerp(Y90rot, Y90rot, 0.5f);
	if (!glm::all(glm::equal(Y90rot, Y90rot3, Epsilon))) {
		std::cout << "Error: slerp with equal quaternions should return the same quaternion" << std::endl;
		print_quat("Original", Y90rot);
		print_quat("Result", Y90rot3);
		Error++;
	}

	// Testing 180 degrees rotation
	// Must be 90 degrees rotation on almost any axis that is on the XZ plane
	glm::quat XZ90rot = glm::slerp(id, -Y90rot, 0.5f);
	float XZ90angle = glm::angle(XZ90rot); // Must be PI/4 = 0.78;

	if (!glm::equal(XZ90angle, glm::pi<float>() * 0.25f, Epsilon)) {
		std::cout << "Error: Angle for 180-degree case incorrect" << std::endl;
		std::cout << "Expected: " << glm::pi<float>() * 0.25f << ", got: " << XZ90angle << std::endl;
		Error++;
	}

	// Testing almost equal quaternions (this test should pass through the linear interpolation)
	// Must be 0 0.00X 0 0.99999
	glm::quat almostid = glm::slerp(id, glm::angleAxis(0.1f, glm::vec3(0.0f, 1.0f, 0.0f)), 0.5f);
	if (!glm::all(glm::equal(almostid, make_quat(1.0f, 0.0f, 0.0f, 0.0f), 0.1f))) {
		std::cout << "Error: slerp with nearly identical quaternions doesn't approach identity" << std::endl;
		print_quat("Result", almostid);
		print_quat("Expected", make_quat(1.0f, 0.0f, 0.0f, 0.0f));
		Error++;
	}

	// Testing quaternions with opposite sign
	{
		glm::quat a = make_quat(-1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat result = glm::slerp(a, id, 0.5f);

		if (!glm::equal(glm::pow(glm::dot(id, result), 2.f), 1.f, 0.01f)) {
			std::cout << "Error: Dot product squared with identity should be 1" << std::endl;
			print_quat("Result", result);
			std::cout << "Dot^2: " << glm::pow(glm::dot(id, result), 2.f) << std::endl;
			Error++;
		}
	}

	return Error;
}

static int test_quat_slerp_spins()
{
	int Error = 0;
	float const Epsilon = 0.0001f;

	std::cout << "\nTesting quaternion slerp with spins:" << std::endl;

	float sqrt2 = std::sqrt(2.0f) / 2.0f;
	glm::quat id = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat Y90rot = make_quat(sqrt2, 0.0f, sqrt2, 0.0f);
	glm::quat Y180rot = make_quat(0.0f, 0.0f, 1.0f, 0.0f);

	// Testing a == 0, k == 1
	// Must be id
	glm::quat id2 = glm::slerp(id, id, 1.0f, 1);
	if (!glm::all(glm::equal(id, id2, Epsilon))) {
		std::cout << "Error: slerp_spins with t=1, k=1 and identical quaternions should return input" << std::endl;
		print_quat("Original", id);
		print_quat("Result", id2);
		Error++;
	}

	// Testing a == 1, k == 2
	// Must be id
	glm::quat id3 = glm::slerp(id, id, 1.0f, 2);
	if (!glm::all(glm::equal(id, id3, Epsilon))) {
		std::cout << "Error: slerp_spins with t=1, k=2 and identical quaternions should return input" << std::endl;
		print_quat("Original", id);
		print_quat("Result", id3);
		Error++;
	}

	// Testing a == 1, k == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	// Negative quaternion is representing same orientation
	glm::quat Y90rot2 = glm::slerp(id, Y90rot, 1.0f, 1);
	if (!glm::all(glm::equal(Y90rot, -Y90rot2, Epsilon))) {
		std::cout << "Error: slerp_spins result incorrect" << std::endl;
		print_quat("Expected", Y90rot);
		print_quat("Result", Y90rot2);
		print_quat("Expected (negated)", -Y90rot);
		Error++;
	}

	// Testing a == 1, k == 2
	// Must be id
	glm::quat Y90rot3 = glm::slerp(id, Y90rot, 8.0f / 9.0f, 2);
	if (!glm::all(glm::equal(id, Y90rot3, Epsilon))) {
		std::cout << "Error: slerp_spins with t=8/9, k=2 should return identity" << std::endl;
		print_quat("Result", Y90rot3);
		print_quat("Expected", id);
		Error++;
	}

	// Testing a == 1, k == 1
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot4 = glm::slerp(id, Y90rot, 0.2f, 1);
	if (!glm::all(glm::equal(Y90rot, Y90rot4, Epsilon))) {
		std::cout << "Error: slerp_spins with t=0.2, k=1 should equal second parameter" << std::endl;
		print_quat("Result", Y90rot4);
		print_quat("Expected", Y90rot);
		Error++;
	}

	// Testing reverse case
	// Must be 45 degrees rotation on Y : 0 0.38 0 0.92
	// Negative quaternion is representing same orientation
	glm::quat Ym45rot2 = glm::slerp(Y90rot, id, 0.9f, 1);
	glm::quat Ym45rot3 = glm::slerp(Y90rot, id, 0.5f);
	if (!glm::all(glm::equal(-Ym45rot2, Ym45rot3, Epsilon))) {
		std::cout << "Error: slerp_spins doesn't match slerp for appropriate inputs" << std::endl;
		print_quat("slerp_spins result", Ym45rot2);
		print_quat("slerp result", Ym45rot3);
		print_quat("Negated slerp_spins", -Ym45rot2);
		Error++;
	}

	// Testing against full circle around the sphere instead of shortest path
	// Must be 45 degrees rotation on Y
	// certainly not a 135 degrees rotation
	glm::quat Y45rot3 = glm::slerp(id, -Y90rot, 0.5f, 0);
	float Y45angle3 = glm::angle(Y45rot3);
	if (!glm::equal(Y45angle3, glm::pi<float>() * 0.25f, Epsilon)) {
		std::cout << "Error: slerp_spins angle incorrect" << std::endl;
		std::cout << "Expected: " << glm::pi<float>() * 0.25f << ", got: " << Y45angle3 << std::endl;
		Error++;
	}

	if (!glm::all(glm::equal(Ym45rot3, Y45rot3, Epsilon))) {
		std::cout << "Error: slerp_spins results don't match expected values" << std::endl;
		print_quat("Result 1", Ym45rot3);
		print_quat("Result 2", Y45rot3);
		Error++;
	}

	// Same, but inverted
	// Must also be 45 degrees rotation on Y :  0 0.38 0 0.92
	// -0 -0.38 -0 -0.92 is ok too
	glm::quat Y45rot4 = glm::slerp(-Y90rot, id, 0.5f, 0);
	if (!glm::all(glm::equal(Ym45rot2, Y45rot4, Epsilon))) {
		std::cout << "Error: slerp_spins with negated input doesn't match expected result" << std::endl;
		print_quat("Result 1", Ym45rot2);
		print_quat("Result 2", Y45rot4);
		Error++;
	}

	// Testing q1 = q2 k == 2
	// Must be 90 degrees rotation on Y : 0 0.7 0 0.7
	glm::quat Y90rot5 = glm::slerp(Y90rot, Y90rot, 0.5f, 2);
	if (!glm::all(glm::equal(Y90rot, Y90rot5, Epsilon))) {
		std::cout << "Error: slerp_spins with identical quaternions should return input" << std::endl;
		print_quat("Original", Y90rot);
		print_quat("Result", Y90rot5);
		Error++;
	}

	// Testing 180 degrees rotation
	// Must be 90 degrees rotation on almost any axis that is on the XZ plane
	glm::quat XZ90rot = glm::slerp(id, -Y90rot, 0.5f, 1);
	float XZ90angle = glm::angle(XZ90rot); // Must be PI*1.25 = 3.92;
	if (!glm::equal(XZ90angle, glm::pi<float>() * 1.25f, Epsilon)) {
		std::cout << "Error: slerp_spins angle for 180-degree case incorrect" << std::endl;
		std::cout << "Expected: " << glm::pi<float>() * 1.25f << ", got: " << XZ90angle << std::endl;
		Error++;
	}

	// Testing rotation over long arc
	// Distance from id to 90 degrees is 270 degrees, so 2/3 of it should be 180 degrees
	// Negative quaternion is representing same orientation
	glm::quat Neg90rot = glm::slerp(id, Y90rot, 2.0f / 3.0f, -1);
	if (!glm::all(glm::equal(Y180rot, -Neg90rot, Epsilon))) {
		std::cout << "Error: slerp_spins long arc rotation incorrect" << std::endl;
		print_quat("Result", Neg90rot);
		print_quat("Expected", Y180rot);
		print_quat("Expected (negated)", -Y180rot);
		Error++;
	}

	return Error;
}

static int test_quat_mul_vec()
{
	int Error(0);
	const float Epsilon = 0.01f;

	std::cout << "\nTesting quaternion-vector multiplication:" << std::endl;

	glm::quat q = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::vec3 v(1, 0, 0);
	glm::vec3 u(q * v);
	glm::vec3 w(u * q);

	if (!glm::all(glm::equal(v, w, Epsilon))) {
		std::cout << "Error: Double rotation doesn't return to original vector" << std::endl;
		print_vec("Original", v);
		print_vec("Rotated", u);
		print_vec("Double-rotated", w);
		Error++;
	}

	return Error;
}

static int test_mul()
{
	int Error = 0;
	const float Epsilon = 0.01f;

	std::cout << "\nTesting quaternion multiplication:" << std::endl;

	glm::quat temp1 = glm::normalize(glm::quat(1.0f, glm::vec3(0.0, 1.0, 0.0)));
	glm::quat temp2 = glm::normalize(glm::quat(0.5f, glm::vec3(1.0, 0.0, 0.0)));

	glm::vec3 transformed0 = (temp1 * glm::vec3(0.0, 1.0, 0.0) * glm::inverse(temp1));
	glm::vec3 temp4 = temp2 * transformed0 * glm::inverse(temp2);

	if (!glm::all(glm::equal(temp4, glm::vec3(0.0f, -0.28f, -0.96f), Epsilon))) {
		std::cout << "Error: Combined rotation result incorrect" << std::endl;
		print_vec("Result", temp4);
		print_vec("Expected", glm::vec3(0.0f, -0.28f, -0.96f));
		Error++;
	}

	glm::quat temp5 = glm::normalize(temp1 * temp2);
	glm::vec3 temp6 = temp5 * glm::vec3(0.0, 1.0, 0.0) * glm::inverse(temp5);

	if (!glm::all(glm::equal(temp6, glm::vec3(-0.48f, 0.36f, -0.8f), Epsilon))) {
		std::cout << "Error: Combined quaternion rotation result incorrect" << std::endl;
		print_vec("Result", temp6);
		print_vec("Expected", glm::vec3(-0.48f, 0.36f, -0.8f));
		Error++;
	}

	glm::quat temp7(1.0f, glm::vec3(0.0, 1.0, 0.0));
	temp7 *= temp5;
	temp7 *= glm::inverse(temp5);

	if (glm::any(glm::notEqual(temp7, glm::quat(1.0f, glm::vec3(0.0, 1.0, 0.0)), glm::epsilon<float>()))) {
		std::cout << "Error: Quaternion rotation and inverse should cancel out" << std::endl;
		print_quat("Original", glm::quat(1.0f, glm::vec3(0.0, 1.0, 0.0)));
		print_quat("Result", temp7);
		Error++;
	}

	return Error;
}

static int test_identity()
{
	int Error = 0;
	const float Epsilon = 0.0001f;

	std::cout << "\nTesting quaternion identity:" << std::endl;

	glm::quat const Q = glm::identity<glm::quat>();

	if (!glm::all(glm::equal(Q, make_quat(1.0f, 0.0f, 0.0f, 0.0f), Epsilon))) {
		std::cout << "Error: Identity quaternion should be (1,0,0,0)" << std::endl;
		print_quat("Identity", Q);
		print_quat("Expected", make_quat(1.0f, 0.0f, 0.0f, 0.0f));
		Error++;
	}

	if (glm::any(glm::notEqual(Q, make_quat(1.0f, 0.0f, 0.0f, 0.0f), Epsilon))) {
		std::cout << "Error: notEqual for identity quaternion failed" << std::endl;
		print_quat("Identity", Q);
		Error++;
	}

	glm::mat4 const M = glm::identity<glm::mat4x4>();
	glm::mat4 const N(1.0f);

	if (!glm::all(glm::equal(M, N, Epsilon))) {
		std::cout << "Error: matrix identity doesn't match constructor with 1.0f" << std::endl;
		Error++;
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion GTC Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: ";

#ifdef GLM_FORCE_QUAT_DATA_WXYZ
	std::cout << "WXYZ (storage)";
#else
	std::cout << "XYZW (storage)";
#endif

	std::cout << " / ";

#ifdef GLM_FORCE_QUAT_DATA_XYZW
	std::cout << "XYZW (constructor)" << std::endl;
#else
	std::cout << "WXYZ (constructor)" << std::endl;
#endif
	std::cout << std::endl;

	Error += test_mul();
	Error += test_quat_mul_vec();
	Error += test_quat_angle();
	Error += test_quat_angleAxis();
	Error += test_quat_mix();
	Error += test_quat_normalize();
	Error += test_quat_euler();
	Error += test_quat_slerp();
	Error += test_quat_slerp_spins();
	Error += test_identity();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
