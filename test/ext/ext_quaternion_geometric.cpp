#include <glm/gtc/constants.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float_precision.hpp>
#include <glm/ext/quaternion_double.hpp>
#include <glm/ext/quaternion_double_precision.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float3_precision.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/ext/vector_double3_precision.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <iostream>

// Helper function to create quaternions consistently regardless of layout
static glm::quat make_quat(float w, float x, float y, float z) {
	return glm::quat::wxyz(w, x, y, z);
}

// Print quaternion components for debugging
static void print_quat(const std::string& label, const glm::quat& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

static int test_length()
{
	int Error = 0;
	float const Epsilon = 0.001f;

	std::cout << "Testing quaternion length:" << std::endl;

	// Test 1: Unit quaternion using scalar constructor
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		float const L = glm::length(A);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Length of identity quaternion should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
	}

	// Test 2: Unit quaternion using scalar + vec3 constructor
	{
		glm::quat const A(1, glm::vec3(0));
		float const L = glm::length(A);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Length of identity quaternion should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
	}

	// Test 3: Unit quaternion using axis-up constructor
	{
		glm::quat const A(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
		float const L = glm::length(A);

		if (!glm::equal(L, 1.0f, Epsilon)) {
			std::cout << "Error: Length of axis-up quaternion should be 1.0, got " << L << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
	}

	// Test 4: Non-unit quaternion
	{
		glm::quat const A = make_quat(2, 1, 0.5, 0.1);
		float const L = glm::length(A);
		float const ExpectedLength = std::sqrt(2 * 2 + 1 * 1 + 0.5 * 0.5 + 0.1 * 0.1);

		if (!glm::equal(L, ExpectedLength, Epsilon)) {
			std::cout << "Error: Length of quaternion incorrect, expected "
				<< ExpectedLength << ", got " << L << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
	}

	return Error;
}

static int test_normalize()
{
	int Error = 0;
	float const Epsilon = 0.001f;

	std::cout << "Testing quaternion normalize:" << std::endl;

	// Test 1: Normalizing unit quaternion (scalar constructor)
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		glm::quat const N = glm::normalize(A);

		if (!glm::all(glm::equal(A, N, Epsilon))) {
			std::cout << "Error: Normalizing identity quaternion should return same quaternion" << std::endl;
			print_quat("Original", A);
			print_quat("Normalized", N);
			Error++;
		}
	}

	// Test 2: Normalizing unit quaternion (scalar + vec3 constructor)
	{
		glm::quat const A(1, glm::vec3(0));
		glm::quat const N = glm::normalize(A);

		if (!glm::all(glm::equal(A, N, Epsilon))) {
			std::cout << "Error: Normalizing identity quaternion should return same quaternion" << std::endl;
			print_quat("Original", A);
			print_quat("Normalized", N);
			Error++;
		}
	}

	// Test 3: Normalizing non-unit quaternion
	{
		glm::quat const A = make_quat(2, 1, 0.5, 0.1);
		float const OriginalLength = glm::length(A);
		glm::quat const N = glm::normalize(A);
		float const NormalizedLength = glm::length(N);

		if (!glm::equal(NormalizedLength, 1.0f, Epsilon)) {
			std::cout << "Error: Normalized quaternion length should be 1.0, got "
				<< NormalizedLength << std::endl;
			print_quat("Original", A);
			print_quat("Normalized", N);
			Error++;
		}

		// Check that direction is preserved
		glm::quat const Scaled = A * (1.0f / OriginalLength);
		if (!glm::all(glm::equal(N, Scaled, Epsilon))) {
			std::cout << "Error: Normalized quaternion direction incorrect" << std::endl;
			print_quat("Normalized", N);
			print_quat("Expected", Scaled);
			Error++;
		}
	}

	return Error;
}

static int test_dot()
{
	int Error = 0;
	float const Epsilon = 0.001f;

	std::cout << "Testing quaternion dot product:" << std::endl;

	// Test 1: Dot product of identical quaternions
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		glm::quat const B = make_quat(1, 0, 0, 0);
		float const C = glm::dot(A, B);

		if (!glm::equal(C, 1.0f, Epsilon)) {
			std::cout << "Error: Dot product of identical unit quaternions should be 1.0, got "
				<< C << std::endl;
			print_quat("Quaternion A", A);
			print_quat("Quaternion B", B);
			Error++;
		}
	}

	// Test 2: Dot product of perpendicular quaternions
	{
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const B = make_quat(0, 0, 1, 0);
		float const C = glm::dot(A, B);

		if (!glm::equal(C, 0.0f, Epsilon)) {
			std::cout << "Error: Dot product of perpendicular quaternions should be 0.0, got "
				<< C << std::endl;
			print_quat("Quaternion A", A);
			print_quat("Quaternion B", B);
			Error++;
		}
	}

	// Test 3: Dot product of opposite quaternions
	{
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const B = make_quat(0, -1, 0, 0);
		float const C = glm::dot(A, B);

		if (!glm::equal(C, -1.0f, Epsilon)) {
			std::cout << "Error: Dot product of opposite quaternions should be -1.0, got "
				<< C << std::endl;
			print_quat("Quaternion A", A);
			print_quat("Quaternion B", B);
			Error++;
		}
	}

	// Test 4: General case
	{
		glm::quat const A = make_quat(0.5f, 0.5f, 0.5f, 0.5f);
		glm::quat const B = make_quat(0.1f, 0.2f, 0.3f, 0.4f);
		float const C = glm::dot(A, B);
		float const Expected = 0.5f * 0.1f + 0.5f * 0.2f + 0.5f * 0.3f + 0.5f * 0.4f;

		if (!glm::equal(C, Expected, Epsilon)) {
			std::cout << "Error: Dot product incorrect, expected "
				<< Expected << ", got " << C << std::endl;
			print_quat("Quaternion A", A);
			print_quat("Quaternion B", B);
			Error++;
		}
	}

	return Error;
}

static int test_cross()
{
	int Error = 0;
	float const Epsilon = 0.001f;

	std::cout << "Testing quaternion cross product:" << std::endl;

	// Test 1: Cross with identity
	{
		glm::quat const Identity = make_quat(1, 0, 0, 0);
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const C = glm::cross(Identity, A);

		if (!glm::all(glm::equal(C, A, Epsilon))) {
			std::cout << "Error: Cross product with identity should return the other quaternion" << std::endl;
			print_quat("Identity", Identity);
			print_quat("Quaternion A", A);
			print_quat("Result", C);
			Error++;
		}
	}

	// Test 2: Cross of i and j basis quaternions
	{
		glm::quat const I = make_quat(0, 1, 0, 0);  // Pure i quaternion
		glm::quat const J = make_quat(0, 0, 1, 0);  // Pure j quaternion
		glm::quat const K = make_quat(0, 0, 0, 1);  // Pure k quaternion

		glm::quat const C = glm::cross(I, J);

		if (!glm::all(glm::equal(C, K, Epsilon))) {
			std::cout << "Error: Cross product of i and j should equal k" << std::endl;
			print_quat("i", I);
			print_quat("j", J);
			print_quat("Result", C);
			print_quat("Expected (k)", K);
			Error++;
		}
	}

	// Test 3: Verify quaternion multiplication (not anticommutative)
	// In GLM, cross(a,b) is actually quaternion multiplication (a*b)
	{
		glm::quat const A = make_quat(0.5f, 0.5f, 0.5f, 0.5f);
		glm::quat const B = make_quat(0.1f, 0.2f, 0.3f, 0.4f);

		glm::quat const C1 = glm::cross(A, B);

		// Expected result for quaternion multiplication
		// Formula: a*b = [w1*w2 - x1*x2 - y1*y2 - z1*z2,
		//                 w1*x2 + x1*w2 + y1*z2 - z1*y2,
		//                 w1*y2 - x1*z2 + y1*w2 + z1*x2,
		//                 w1*z2 + x1*y2 - y1*x2 + z1*w2]
		float w = 0.5f * 0.1f - 0.5f * 0.2f - 0.5f * 0.3f - 0.5f * 0.4f;
		float x = 0.5f * 0.2f + 0.5f * 0.1f + 0.5f * 0.4f - 0.5f * 0.3f;
		float y = 0.5f * 0.3f - 0.5f * 0.4f + 0.5f * 0.1f + 0.5f * 0.2f;
		float z = 0.5f * 0.4f + 0.5f * 0.3f - 0.5f * 0.2f + 0.5f * 0.1f;
		glm::quat Expected = make_quat(w, x, y, z);

		if (!glm::all(glm::equal(C1, Expected, Epsilon))) {
			std::cout << "Error: GLM's cross(a,b) function should perform quaternion multiplication" << std::endl;
			print_quat("cross(A,B)", C1);
			print_quat("Expected", Expected);
			Error++;
		}

		// Verify that cross(a,b) != cross(b,a) for quaternions
		glm::quat const C2 = glm::cross(B, A);
		if (glm::all(glm::equal(C1, C2, Epsilon))) {
			std::cout << "Error: Quaternion multiplication should not be commutative" << std::endl;
			print_quat("A*B", C1);
			print_quat("B*A", C2);
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Geometric Extension Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: " <<
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
		"WXYZ"
#else
		"XYZW"
#endif
		<< std::endl << std::endl;

	Error += test_length();
	Error += test_normalize();
	Error += test_dot();
	Error += test_cross();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
