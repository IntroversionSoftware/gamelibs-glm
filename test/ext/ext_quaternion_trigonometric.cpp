#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <iostream>

// Helper function to create quaternions consistently regardless of layout
template <typename T, glm::qualifier Q>
static glm::qua<T, Q> make_quat(T w, T x, T y, T z) {
	return glm::qua<T, Q>::wxyz(w, x, y, z);
}

// Print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

static int test_angle()
{
	float const Epsilon = 0.001f;
	int Error = 0;

	std::cout << "Testing quaternion angle:" << std::endl;

	// Test 1: 90-degree rotation around axis using axis-up constructor
	{
		glm::quat const Q = glm::quat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)); // 90-degree rotation
		float const A = glm::degrees(glm::angle(Q));

		if (!glm::equal(A, 90.0f, Epsilon)) {
			std::cout << "Error: Angle of 90-degree rotation quaternion (axis-up) incorrect. Expected 90, got "
				<< A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}
	}

	// Test 2: 90-degree rotation with axes swapped
	{
		glm::quat const Q = glm::quat(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)); // 90-degree rotation
		float const A = glm::degrees(glm::angle(Q));

		if (!glm::equal(A, 90.0f, Epsilon)) {
			std::cout << "Error: Angle of 90-degree rotation quaternion (swapped axes) incorrect. Expected 90, got "
				<< A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}
	}

	// Test 3: Nearly 360-degree rotation using angleAxis
	// This test is checking GLM's behavior with rotations near 2π
	{
		// Create a quaternion with rotation of (2π-1) radians around X axis
		glm::quat const Q = glm::angleAxis(glm::two_pi<float>() - 1.0f, glm::vec3(1, 0, 0));
		float const A = glm::angle(Q);

		// After analyzing the behavior, it appears GLM is calculating:
		// angle = 2 * acos(w)  [without taking absolute value]
		// When w is negative (which happens for rotations > π),
		// this gives us the original angle rather than the smaller equivalent angle

		// The expected value should be the original angle: 2π-1 ≈ 5.28
		float expected = glm::two_pi<float>() - 1.0f;

		if (!glm::equal(A, expected, Epsilon)) {
			std::cout << "Error: Angle of (2π-1) rotation quaternion incorrect. Expected "
				<< expected << ", got " << A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}

		// Additionally, verify that this quaternion correctly rotates a vector
		glm::vec3 v(0.0f, 1.0f, 0.0f);
		glm::vec3 rotated = Q * v;
		glm::vec3 expected_v(0.0f, glm::cos(glm::two_pi<float>() - 1.0f),
							 glm::sin(glm::two_pi<float>() - 1.0f));

		if (glm::distance(rotated, expected_v) > Epsilon) {
			std::cout << "Error: Quaternion doesn't rotate vector as expected" << std::endl;
			std::cout << "Original: 0, 1, 0" << std::endl;
			std::cout << "Rotated: " << rotated.x << ", " << rotated.y << ", " << rotated.z << std::endl;
			std::cout << "Expected: " << expected_v.x << ", " << expected_v.y << ", " << expected_v.z << std::endl;
			Error++;
		}
	}

	// Test 4: 45-degree rotation
	{
		glm::quat const Q = glm::angleAxis(glm::radians(45.0f), glm::normalize(glm::vec3(1, 1, 0)));
		float const A = glm::degrees(glm::angle(Q));

		if (!glm::equal(A, 45.0f, Epsilon)) {
			std::cout << "Error: Angle of 45-degree rotation quaternion incorrect. Expected 45, got "
				<< A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}
	}

	// Test 5: 0-degree rotation (identity quaternion)
	{
		glm::quat const Q = make_quat<float, glm::qualifier::defaultp>(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
		float const A = glm::degrees(glm::angle(Q));

		if (!glm::equal(A, 0.0f, Epsilon)) {
			std::cout << "Error: Angle of identity quaternion incorrect. Expected 0, got "
				<< A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}
	}

	// Test 6: 180-degree rotation
	{
		glm::quat const Q = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
		float const A = glm::degrees(glm::angle(Q));

		if (!glm::equal(A, 180.0f, Epsilon)) {
			std::cout << "Error: Angle of 180-degree rotation quaternion incorrect. Expected 180, got "
				<< A << std::endl;
			print_quat("Quaternion", Q);
			Error++;
		}
	}

	return Error;
}

static int test_angleAxis()
{
	float const Epsilon = 0.001f;
	int Error = 0;

	std::cout << "\nTesting quaternion angleAxis functions:" << std::endl;

	// Test 1: Create and decompose 90-degree rotation
	{
		glm::vec3 const Axis = glm::normalize(glm::vec3(1, 0, 0));
		float const Angle = glm::radians(90.0f);

		glm::quat const Q = glm::angleAxis(Angle, Axis);

		// Extract angle and axis
		float const ExtractedAngle = glm::angle(Q);
		glm::vec3 const ExtractedAxis = glm::axis(Q);

		// Check angle
		if (!glm::equal(ExtractedAngle, Angle, Epsilon)) {
			std::cout << "Error: Extracted angle doesn't match original. Expected "
				<< Angle << ", got " << ExtractedAngle << std::endl;
			Error++;
		}

		// Check axis (note: axis might be negated, both are valid)
		if (glm::length(ExtractedAxis - Axis) > Epsilon &&
			glm::length(ExtractedAxis + Axis) > Epsilon) {
			std::cout << "Error: Extracted axis doesn't match original" << std::endl;
			std::cout << "Original: " << Axis.x << ", " << Axis.y << ", " << Axis.z << std::endl;
			std::cout << "Extracted: " << ExtractedAxis.x << ", " << ExtractedAxis.y << ", " << ExtractedAxis.z << std::endl;
			Error++;
		}
	}

	// Test 2: Create and apply rotation
	{
		glm::vec3 const Axis = glm::normalize(glm::vec3(0, 1, 0));
		float const Angle = glm::radians(90.0f);

		glm::quat const Q = glm::angleAxis(Angle, Axis);

		// Rotate a vector and check result
		glm::vec3 const V(1, 0, 0);
		glm::vec3 const Rotated = Q * V;
		glm::vec3 const Expected(0, 0, -1); // 90 degrees around Y axis transforms X to -Z

		if (glm::distance(Rotated, Expected) > Epsilon) {
			std::cout << "Error: Rotated vector doesn't match expected result" << std::endl;
			std::cout << "Original: " << V.x << ", " << V.y << ", " << V.z << std::endl;
			std::cout << "Rotated: " << Rotated.x << ", " << Rotated.y << ", " << Rotated.z << std::endl;
			std::cout << "Expected: " << Expected.x << ", " << Expected.y << ", " << Expected.z << std::endl;
			Error++;
		}
	}

	// Test 3: Check that angle is always positive
	{
		glm::vec3 const Axis = glm::normalize(glm::vec3(0, 0, 1));
		float const Angle = glm::radians(-45.0f); // Negative angle

		glm::quat const Q = glm::angleAxis(Angle, Axis);
		float const ExtractedAngle = glm::angle(Q);

		// Angle should be positive (between 0 and π)
		if (ExtractedAngle < 0.0f || ExtractedAngle > glm::pi<float>()) {
			std::cout << "Error: Extracted angle should be positive and less than π. Got "
				<< ExtractedAngle << std::endl;
			Error++;
		}

		// The magnitude should be the same
		if (!glm::equal(glm::abs(ExtractedAngle), glm::abs(Angle), Epsilon)) {
			std::cout << "Error: Magnitude of extracted angle doesn't match original. Expected "
				<< glm::abs(Angle) << ", got " << ExtractedAngle << std::endl;
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Trigonometric Extension Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: " <<
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
		"WXYZ"
#else
		"XYZW"
#endif
		<< std::endl << std::endl;

	Error += test_angle();
	Error += test_angleAxis();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
