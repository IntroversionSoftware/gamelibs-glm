#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

// Helper function to print quaternion components with proper precision qualifier
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

// Helper function to print vector components
template <typename T, glm::qualifier Q>
static void print_vec(const std::string& label, const glm::vec<3, T, Q>& v) {
	std::cout << label << ": x=" << v.x << ", y=" << v.y << ", z=" << v.z << std::endl;
}

static int test_lookAt()
{
	int Error(0);
	float const Epsilon = glm::epsilon<float>();

	std::cout << "Testing quatLookAt functions:" << std::endl;

	// Define test vectors
	glm::vec3 eye(0.0f);
	glm::vec3 center(1.1f, -2.0f, 3.1416f);
	glm::vec3 up(-0.17f, 7.23f, -1.744f);
	glm::vec3 direction = glm::normalize(center - eye);

	// Print test vectors for debugging
	print_vec("Eye", eye);
	print_vec("Center", center);
	print_vec("Up", up);
	print_vec("Direction", direction);

	// Test 1: Default implementation (right-handed)
	{
		glm::quat test_quat = glm::quatLookAt(direction, up);
		glm::quat test_mat = glm::conjugate(glm::quat_cast(glm::lookAt(eye, center, up)));

		// Test quaternion is normalized
		float length = glm::length(test_quat);
		if (glm::abs(length - 1.0f) > Epsilon) {
			std::cout << "Error: quatLookAt does not produce unit quaternion. Length: " << length << std::endl;
			print_quat("Quaternion", test_quat);
			Error++;
		}

		// Test quaternion matches matrix approach
		// Note: quaternions q and -q represent the same rotation
		float diff1 = glm::length(test_quat - test_mat);
		float diff2 = glm::length(test_quat + test_mat);
		float min_diff = glm::min(diff1, diff2);

		if (min_diff > Epsilon) {
			std::cout << "Error: quatLookAt doesn't match matrix approach. Min difference: " << min_diff << std::endl;
			print_quat("quatLookAt result", test_quat);
			print_quat("lookAt matrix result", test_mat);
			Error++;
		}
	}

	// Test 2: Left-handed implementation
	{
		glm::quat test_quatLH = glm::quatLookAtLH(direction, up);
		glm::quat test_matLH = glm::conjugate(glm::quat_cast(glm::lookAtLH(eye, center, up)));

		// Test quaternion is normalized
		float length = glm::length(test_quatLH);
		if (glm::abs(length - 1.0f) > Epsilon) {
			std::cout << "Error: quatLookAtLH does not produce unit quaternion. Length: " << length << std::endl;
			print_quat("Quaternion LH", test_quatLH);
			Error++;
		}

		// Test quaternion matches matrix approach
		float diff1 = glm::length(test_quatLH - test_matLH);
		float diff2 = glm::length(test_quatLH + test_matLH);
		float min_diff = glm::min(diff1, diff2);

		if (min_diff > Epsilon) {
			std::cout << "Error: quatLookAtLH doesn't match matrix approach. Min difference: " << min_diff << std::endl;
			print_quat("quatLookAtLH result", test_quatLH);
			print_quat("lookAtLH matrix result", test_matLH);
			Error++;
		}
	}

	// Test 3: Right-handed implementation (explicit)
	{
		glm::quat test_quatRH = glm::quatLookAtRH(direction, up);
		glm::quat test_matRH = glm::conjugate(glm::quat_cast(glm::lookAtRH(eye, center, up)));

		// Test quaternion is normalized
		float length = glm::length(test_quatRH);
		if (glm::abs(length - 1.0f) > Epsilon) {
			std::cout << "Error: quatLookAtRH does not produce unit quaternion. Length: " << length << std::endl;
			print_quat("Quaternion RH", test_quatRH);
			Error++;
		}

		// Test quaternion matches matrix approach
		float diff1 = glm::length(test_quatRH - test_matRH);
		float diff2 = glm::length(test_quatRH + test_matRH);
		float min_diff = glm::min(diff1, diff2);

		if (min_diff > Epsilon) {
			std::cout << "Error: quatLookAtRH doesn't match matrix approach. Min difference: " << min_diff << std::endl;
			print_quat("quatLookAtRH result", test_quatRH);
			print_quat("lookAtRH matrix result", test_matRH);
			Error++;
		}
	}

	// Test 4: Verify LH and RH produce different results
	{
		glm::quat test_quatLH = glm::quatLookAtLH(direction, up);
		glm::quat test_quatRH = glm::quatLookAtRH(direction, up);

		// The quaternions should be different (but they might be negatives of each other)
		float diff1 = glm::length(test_quatLH - test_quatRH);
		float diff2 = glm::length(test_quatLH + test_quatRH);

		// One of these differences should be significant
		if (diff1 < Epsilon && diff2 < Epsilon) {
			std::cout << "Error: LH and RH quaternions are too similar" << std::endl;
			print_quat("LH quaternion", test_quatLH);
			print_quat("RH quaternion", test_quatRH);
			Error++;
		}
	}

	// Test 5: Edge case - we'll skip testing completely degenerate up vectors
	// as they're mathematically undefined and implementation-dependent
	{
		// Instead, let's verify that when the up vector is well-defined,
		// the resulting quaternion correctly aligns with the direction
		glm::vec3 good_up(0.0f, 1.0f, 0.0f);
		glm::quat test_quat = glm::quatLookAt(direction, good_up);

		// The result should be a unit quaternion
		float length = glm::length(test_quat);
		if (glm::abs(length - 1.0f) > Epsilon) {
			std::cout << "Error: quatLookAt does not produce unit quaternion" << std::endl;
			print_quat("Quaternion", test_quat);
			Error++;
		}

		// Verify that the quaternion correctly aligns with the direction
		glm::vec3 forward(0.0f, 0.0f, -1.0f); // GLM's default forward direction
		glm::vec3 rotated_forward = test_quat * forward;

		float alignment = glm::dot(glm::normalize(rotated_forward), direction);
		if (alignment < 0.999f) { // Allow for small numerical errors
			std::cout << "Error: quatLookAt doesn't properly align with direction" << std::endl;
			std::cout << "Alignment value (should be ~1.0): " << alignment << std::endl;
			print_vec("Direction", direction);
			print_vec("Rotated forward", rotated_forward);
			Error++;
		}
	}

	return Error;
}

// Test rotation between vectors
static int test_rotation()
{
	int Error(0);
	float const Epsilon = glm::epsilon<float>();

	std::cout << "\nTesting quaternion rotation functions:" << std::endl;

	// Test case 1: Rotation from one vector to another
	{
		glm::vec3 v1(1.0f, 0.0f, 0.0f);
		glm::vec3 v2(0.0f, 1.0f, 0.0f);

		// Create quaternion that rotates v1 to v2
		glm::quat q = glm::rotation(v1, v2);

		// Verify quaternion is normalized
		float length = glm::length(q);
		if (glm::abs(length - 1.0f) > Epsilon) {
			std::cout << "Error: rotation quaternion is not normalized. Length: " << length << std::endl;
			print_quat("Quaternion", q);
			Error++;
		}

		// Apply rotation and check result
		glm::vec3 result = q * v1;
		if (glm::distance(result, v2) > Epsilon) {
			std::cout << "Error: rotation quaternion doesn't rotate v1 to v2" << std::endl;
			print_vec("v1", v1);
			print_vec("v2", v2);
			print_vec("Result", result);
			print_quat("Quaternion", q);
			Error++;
		}
	}

	// Test case 2: Rotation of pi/2 around axis
	{
		glm::vec3 axis(0.0f, 0.0f, 1.0f);
		float angle = glm::radians(90.0f);

		glm::quat q = glm::angleAxis(angle, axis);

		glm::vec3 v1(1.0f, 0.0f, 0.0f);
		glm::vec3 expected(0.0f, 1.0f, 0.0f);

		glm::vec3 result = q * v1;

		if (glm::distance(result, expected) > Epsilon) {
			std::cout << "Error: 90 degree rotation around Z doesn't work correctly" << std::endl;
			print_vec("v1", v1);
			print_vec("Expected", expected);
			print_vec("Result", result);
			print_quat("Quaternion", q);
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Transform Extension Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: " <<
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
		"WXYZ"
#else
		"XYZW"
#endif
		<< std::endl << std::endl;

	Error += test_lookAt();
	Error += test_rotation();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
