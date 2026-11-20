#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/rotate_vector.hpp>

static int test_rotate()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Rotate vec2 by 90 degrees
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>() * 0.5f);
		glm::vec2 const Expected(0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(A, Expected, epsilon); // vec2 rotation 90°
	}

	// Test case 2: Rotate vec2 by 180 degrees
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>());
		glm::vec2 const Expected(-1.0f, 0.0f);

		Error += GLM_CHECK_NEAR(A, Expected, epsilon); // vec2 rotation 180°
	}

	// Test case 3: Rotate vec2 by 360 degrees (full circle)
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>() * 2.0f);
		glm::vec2 const Expected(1.0f, 0.0f);

		// Use a slightly higher epsilon for this test due to accumulated floating point error
		const float full_circle_epsilon = 0.0001f;
		Error += GLM_CHECK_NEAR(A, Expected, full_circle_epsilon); // vec2 rotation 360°
	}

	// Test case 4: Rotate vec3 by 90 degrees around Z
	{
		glm::vec3 const B = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
		glm::vec3 const Expected(0.0f, 1.0f, 0.0f);

		Error += GLM_CHECK_NEAR(B, Expected, epsilon); // vec3 rotation 90° around Z
	}

	// Test case 5: Rotate vec3 by 90 degrees around Y
	{
		glm::vec3 const B = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, glm::vec3(0, 1, 0));
		glm::vec3 const Expected(0.0f, 0.0f, -1.0f);

		Error += GLM_CHECK_NEAR(B, Expected, epsilon); // vec3 rotation 90° around Y
	}

	// Test case 6: Rotate vec3 by 90 degrees around X
	{
		glm::vec3 const B = glm::rotate(glm::vec3(0, 1, 0), glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0));
		glm::vec3 const Expected(0.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(B, Expected, epsilon); // vec3 rotation 90° around X
	}

	// Test case 7: Rotate vec3 by 90 degrees around arbitrary axis
	{
		glm::vec3 const axis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 const B = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, axis);

		// For rotations around arbitrary axes like (1,1,1), the dot product won't be exactly 0
		// This is mathematically expected - a vector rotated 90° around (1,1,1) won't be perpendicular
		// to the original vector unless the original is perpendicular to the axis
		//
		// Instead, verify the angle between the original and rotated vector is what we expect
		glm::vec3 const original(1, 0, 0);
		float cos_angle = glm::dot(original, B) / (glm::length(original) * glm::length(B));

		// For a rotation around (1,1,1), the angle won't be exactly 90°
		// The expected dot product is 1/3 ≈ 0.333333 for this specific case
		//
		// This is because the rotation preserves the component along the axis
		// and the component of (1,0,0) along (1,1,1)/√3 is 1/√3, so the dot product is 1/3
		float expected_dot = 1.0f / 3.0f;

		Error += glm_test::context(GLM_CHECK_NEAR(cos_angle, expected_dot, 0.0001f), "B", B);
	}

	// Test case 8: Rotate vec4 by 90 degrees around Z
	{
		glm::vec4 const C = glm::rotate(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(C, Expected, epsilon); // vec4 rotation 90° around Z
	}

	// Test case 9: Rotate normalized vs non-normalized axis
	{
		glm::vec3 const normalized_axis = glm::normalize(glm::vec3(0, 0, 2));
		glm::vec3 const non_normalized_axis(0, 0, 2);

		glm::vec3 const B1 = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, normalized_axis);
		glm::vec3 const B2 = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, non_normalized_axis);

		Error += GLM_CHECK_NEAR(B1, B2, epsilon); // Normalized vs non-normalized axis
	}

	return Error;
}

// rotate(v, angle, normal) is the same rotation as glm::rotate(angle, normal), which normalizes the
// normal: so a normal of any length, in either single or double precision.
template<typename T>
static int test_rotate_matches_matrix()
{
	int Error = 0;

	typedef glm::vec<3, T> vec3;
	vec3 const Vectors[] = { vec3(1, 0, 0), vec3(0.3, -2, 5), vec3(-1, 1, -1) };
	vec3 const Normals[] = { vec3(0, 0, 1), vec3(1, 1, 1), vec3(-3, 0.5, 2), vec3(0, 1e-3, 0) };
	T const Angles[] = { T(0), T(0.5), T(-2), glm::pi<T>(), T(5) };
	T const Epsilon = std::is_same<T, float>::value ? T(1e-5) : T(1e-12);

	for (vec3 const& v : Vectors)
		for (vec3 const& n : Normals)
			for (T a : Angles)
			{
				vec3 const Expected = glm::mat<3, 3, T>(glm::rotate(a, n)) * v;
				Error += glm_test::context(GLM_CHECK_NEAR(glm::rotate(v, a, n), Expected, Epsilon * T(5)), "v", v, "n", n, "a", a);
			}

	return Error;
}

static int test_rotateX()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Rotate vec3 around X by 90 degrees
	{
		glm::vec3 const V(0, 1, 0);
		glm::vec3 const D = glm::rotateX(V, glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(D, Expected, epsilon); // rotateX vec3 by 90°
	}

	// Test case 2: Rotate vec3 around X by 90 degrees - vector along X axis (should be invariant)
	{
		glm::vec3 const D = glm::rotateX(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(1.0f, 0.0f, 0.0f);

		Error += GLM_CHECK_NEAR(D, Expected, epsilon); // rotateX vec3 invariant axis
	}

	// Test case 3: Rotate vec3 around X by 180 degrees
	{
		glm::vec3 const D = glm::rotateX(glm::vec3(0, 1, 0), glm::pi<float>());
		glm::vec3 const Expected(0.0f, -1.0f, 0.0f);

		Error += GLM_CHECK_NEAR(D, Expected, epsilon); // rotateX vec3 by 180°
	}

	// Test case 4: Rotate vec4 around X by 90 degrees
	{
		glm::vec4 const E = glm::rotateX(glm::vec4(0, 1, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, 1.0f, 1.0f);

		Error += GLM_CHECK_NEAR(E, Expected, epsilon); // rotateX vec4 by 90°
	}

	// Test case 5: Rotate vec4 around X by 90 degrees - vector along X axis (should be invariant)
	{
		glm::vec4 const E = glm::rotateX(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(1.0f, 0.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(E, Expected, epsilon); // rotateX vec4 invariant axis
	}

	return Error;
}

static int test_rotateY()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Rotate vec3 around Y by 90 degrees
	{
		glm::vec3 const F = glm::rotateY(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 0.0f, -1.0f);

		Error += GLM_CHECK_NEAR(F, Expected, epsilon); // rotateY vec3 by 90°
	}

	// Test case 2: Rotate vec3 around Y by 90 degrees - vector along Y axis (should be invariant)
	{
		glm::vec3 const F = glm::rotateY(glm::vec3(0, 1, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 1.0f, 0.0f);

		Error += GLM_CHECK_NEAR(F, Expected, epsilon); // rotateY vec3 invariant axis
	}

	// Test case 3: Rotate vec3 around Y by 180 degrees
	{
		glm::vec3 const F = glm::rotateY(glm::vec3(1, 0, 0), glm::pi<float>());
		glm::vec3 const Expected(-1.0f, 0.0f, 0.0f);

		Error += GLM_CHECK_NEAR(F, Expected, epsilon); // rotateY vec3 by 180°
	}

	// Test case 4: Rotate vec4 around Y by 90 degrees
	{
		glm::vec4 const G = glm::rotateY(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, -1.0f, 1.0f);

		Error += GLM_CHECK_NEAR(G, Expected, epsilon); // rotateY vec4 by 90°
	}

	// Test case 5: Rotate vec4 around Y by 90 degrees - vector along Y axis (should be invariant)
	{
		glm::vec4 const G = glm::rotateY(glm::vec4(0, 1, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(G, Expected, epsilon); // rotateY vec4 invariant axis
	}

	return Error;
}

static int test_rotateZ()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Rotate vec3 around Z by 90 degrees
	{
		glm::vec3 const H = glm::rotateZ(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 1.0f, 0.0f);

		Error += GLM_CHECK_NEAR(H, Expected, epsilon); // rotateZ vec3 by 90°
	}

	// Test case 2: Rotate vec3 around Z by 90 degrees - vector along Z axis (should be invariant)
	{
		glm::vec3 const H = glm::rotateZ(glm::vec3(0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(H, Expected, epsilon); // rotateZ vec3 invariant axis
	}

	// Test case 3: Rotate vec3 around Z by 180 degrees
	{
		glm::vec3 const H = glm::rotateZ(glm::vec3(1, 0, 0), glm::pi<float>());
		glm::vec3 const Expected(-1.0f, 0.0f, 0.0f);

		Error += GLM_CHECK_NEAR(H, Expected, epsilon); // rotateZ vec3 by 180°
	}

	// Test case 4: Rotate vec4 around Z by 90 degrees
	{
		glm::vec4 const I = glm::rotateZ(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		Error += GLM_CHECK_NEAR(I, Expected, epsilon); // rotateZ vec4 by 90°
	}

	// Test case 5: Rotate vec4 around Z by 90 degrees - vector along Z axis (should be invariant)
	{
		glm::vec4 const I = glm::rotateZ(glm::vec4(0, 0, 1, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, 1.0f, 1.0f);

		Error += GLM_CHECK_NEAR(I, Expected, epsilon); // rotateZ vec4 invariant axis
	}

	return Error;
}

static int test_orientation()
{
	int Error = 0;
	const float epsilon = 0.1f; // Using a higher epsilon as in the original test

	// Test case 1: Basic orientation test with normalized direction
	{
		glm::mat4 const O = glm::orientation(glm::normalize(glm::vec3(1)), glm::vec3(0, 0, 1));

		glm::mat4 const Expected(
			glm::vec4(0.79f, -0.21f, -0.58f, 0.0f),
			glm::vec4(-0.21f, 0.79f, -0.58f, 0.0f),
			glm::vec4(0.58f, 0.58f, 0.58f, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);

		Error += GLM_CHECK_NEAR(O, Expected, epsilon);
	}

	// Test case 2: Orientation with direction along one of the primary axes
	{
		glm::mat4 const O = glm::orientation(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));

		// This should create a matrix that maps Z to Y, which would involve a -90° rotation around X
		// The matrix may apply the rotation differently, so we need to check the actual transformation
		const float custom_epsilon = 0.0001f;

		// Check that the matrix is orthogonal (columns are perpendicular and normalized)
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i != j) {
					float dot_product = glm::dot(glm::vec3(O[i]), glm::vec3(O[j]));
					Error += glm_test::context(GLM_CHECK_NEAR(dot_product, 0.0f, custom_epsilon), "i", i, "j", j);
				}
			}

			float length = glm::length(glm::vec3(O[i]));
			Error += glm_test::context(GLM_CHECK_NEAR(length, 1.0f, custom_epsilon), "i", i);
		}

		// Check that the transformation maps (0,0,1) to either (0,1,0) or (0,-1,0)
		// The GLM implementation appears to produce (0,-1,0) in this case
		glm::vec3 const transformed = O * glm::vec4(0, 0, 1, 0);
		glm::vec3 const expected(0, -1, 0);

		Error += GLM_CHECK_NEAR(transformed, expected, custom_epsilon); // Orientation transformation
	}

	// Test case 3: Orientation with parallel direction and up vectors (should be handled gracefully)
	{
		glm::mat4 const O = glm::orientation(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

		// The function should handle this degenerate case and produce a valid orientation matrix
		// We can test that the matrix is still orthogonal
		const float custom_epsilon = 0.0001f;

		for (int i = 0; i < 3; ++i) {
			float length = glm::length(glm::vec3(O[i]));
			Error += glm_test::context(GLM_CHECK_NEAR(length, 1.0f, custom_epsilon), "i", i);
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_rotate());
	Error += GLM_TEST(test_rotate_matches_matrix<float>());
	Error += GLM_TEST(test_rotate_matches_matrix<double>());
	Error += GLM_TEST(test_rotateX());
	Error += GLM_TEST(test_rotateY());
	Error += GLM_TEST(test_rotateZ());
	Error += GLM_TEST(test_orientation());

	return GLM_TEST_FINISH(Error);
}
