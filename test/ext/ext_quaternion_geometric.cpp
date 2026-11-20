#include <glm/gtc/constants.hpp>
#include "glm_test.hpp"
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


	// Test 1: Unit quaternion using scalar constructor
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		float const L = glm::length(A);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Length of identity quaternion should be 1.0
	}

	// Test 2: Unit quaternion using scalar + vec3 constructor
	{
		glm::quat const A(1, glm::vec3(0));
		float const L = glm::length(A);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Length of identity quaternion should be 1.0
	}

	// Test 3: Unit quaternion using axis-up constructor
	{
		glm::quat const A(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
		float const L = glm::length(A);

		Error += GLM_CHECK_NEAR(L, 1.0f, Epsilon); // Length of axis-up quaternion should be 1.0
	}

	// Test 4: Non-unit quaternion
	{
		glm::quat const A = make_quat(2, 1, 0.5f, 0.1f);
		float const L = glm::length(A);
		float const ExpectedLength = std::sqrt(2.0f * 2.0f + 1.0f * 1.0f + 0.5f * 0.5f + 0.1f * 0.1f);

		Error += GLM_CHECK_NEAR(L, ExpectedLength, Epsilon); // Length of quaternion incorrect
	}

	return Error;
}

static int test_normalize()
{
	int Error = 0;
	float const Epsilon = 0.001f;


	// Test 1: Normalizing unit quaternion (scalar constructor)
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		glm::quat const N = glm::normalize(A);

		Error += GLM_CHECK_NEAR(A, N, Epsilon); // Normalizing identity quaternion should return same quaternion
	}

	// Test 2: Normalizing unit quaternion (scalar + vec3 constructor)
	{
		glm::quat const A(1, glm::vec3(0));
		glm::quat const N = glm::normalize(A);

		Error += GLM_CHECK_NEAR(A, N, Epsilon); // Normalizing identity quaternion should return same quaternion
	}

	// Test 3: Normalizing non-unit quaternion
	{
		glm::quat const A = make_quat(2, 1, 0.5f, 0.1f);
		float const OriginalLength = glm::length(A);
		glm::quat const N = glm::normalize(A);
		float const NormalizedLength = glm::length(N);

		Error += GLM_CHECK_NEAR(NormalizedLength, 1.0f, Epsilon); // Normalized quaternion length should be 1.0

		// Check that direction is preserved
		glm::quat const Scaled = A * (1.0f / OriginalLength);
		Error += GLM_CHECK_NEAR(N, Scaled, Epsilon); // Normalized quaternion direction incorrect
	}

	return Error;
}

static int test_dot()
{
	int Error = 0;
	float const Epsilon = 0.001f;


	// Test 1: Dot product of identical quaternions
	{
		glm::quat const A = make_quat(1, 0, 0, 0);
		glm::quat const B = make_quat(1, 0, 0, 0);
		float const C = glm::dot(A, B);

		Error += GLM_CHECK_NEAR(C, 1.0f, Epsilon); // Dot product of identical unit quaternions should be 1.0
	}

	// Test 2: Dot product of perpendicular quaternions
	{
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const B = make_quat(0, 0, 1, 0);
		float const C = glm::dot(A, B);

		Error += GLM_CHECK_NEAR(C, 0.0f, Epsilon); // Dot product of perpendicular quaternions should be 0.0
	}

	// Test 3: Dot product of opposite quaternions
	{
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const B = make_quat(0, -1, 0, 0);
		float const C = glm::dot(A, B);

		Error += GLM_CHECK_NEAR(C, -1.0f, Epsilon); // Dot product of opposite quaternions should be -1.0
	}

	// Test 4: General case
	{
		glm::quat const A = make_quat(0.5f, 0.5f, 0.5f, 0.5f);
		glm::quat const B = make_quat(0.1f, 0.2f, 0.3f, 0.4f);
		float const C = glm::dot(A, B);
		float const Expected = 0.5f * 0.1f + 0.5f * 0.2f + 0.5f * 0.3f + 0.5f * 0.4f;

		Error += GLM_CHECK_NEAR(C, Expected, Epsilon); // Dot product incorrect
	}

	return Error;
}

static int test_cross()
{
	int Error = 0;
	float const Epsilon = 0.001f;


	// Test 1: Cross with identity
	{
		glm::quat const Identity = make_quat(1, 0, 0, 0);
		glm::quat const A = make_quat(0, 1, 0, 0);
		glm::quat const C = glm::cross(Identity, A);

		Error += GLM_CHECK_NEAR(C, A, Epsilon); // Cross product with identity should return the other quaternion
	}

	// Test 2: Cross of i and j basis quaternions
	{
		glm::quat const I = make_quat(0, 1, 0, 0);  // Pure i quaternion
		glm::quat const J = make_quat(0, 0, 1, 0);  // Pure j quaternion
		glm::quat const K = make_quat(0, 0, 0, 1);  // Pure k quaternion

		glm::quat const C = glm::cross(I, J);

		Error += GLM_CHECK_NEAR(C, K, Epsilon); // Cross product of i and j should equal k
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

		Error += GLM_CHECK_NEAR(C1, Expected, Epsilon); // GLM's cross(a,b) function should perform quaternion multiplication

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

	Error += GLM_TEST(test_length());
	Error += GLM_TEST(test_normalize());
	Error += GLM_TEST(test_dot());
	Error += GLM_TEST(test_cross());

	return GLM_TEST_FINISH(Error);
}
