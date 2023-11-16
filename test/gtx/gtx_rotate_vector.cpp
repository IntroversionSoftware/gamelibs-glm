#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/vector_relational.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// Helper function to print vector comparison for failure cases
template<typename VecT>
void print_vector_comparison(const VecT& actual, const VecT& expected, const char* test_name) {
	std::cout << "---- " << test_name << " test failure ----\n";
	std::cout << "Expected vector: " << glm::to_string(expected) << "\n";
	std::cout << "Actual vector:   " << glm::to_string(actual) << "\n";

	// For vectors, calculate and show the difference
	auto diff = actual - expected;
	std::cout << "Difference:      " << glm::to_string(diff) << "\n";

	std::cout << "----------------------------\n";
}

// Helper function to print matrix comparison for failure cases
void print_matrix_comparison(const glm::mat4& actual, const glm::mat4& expected, const char* test_name) {
	std::cout << "---- " << test_name << " test failure ----\n";
	std::cout << "Expected matrix:\n" << glm::to_string(expected) << "\n";
	std::cout << "Actual matrix:\n" << glm::to_string(actual) << "\n";

	// Also print the difference to help identify small precision issues
	glm::mat4 diff;
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			diff[col][row] = actual[col][row] - expected[col][row];
		}
	}
	std::cout << "Difference (actual - expected):\n" << glm::to_string(diff) << "\n";
	std::cout << "----------------------------\n";
}

static int test_rotate()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Rotate vec2 by 90 degrees
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>() * 0.5f);
		glm::vec2 const Expected(0.0f, 1.0f);

		if (!glm::all(glm::equal(A, Expected, epsilon))) {
			print_vector_comparison(A, Expected, "vec2 rotation 90°");
			Error += 1;
		}
	}

	// Test case 2: Rotate vec2 by 180 degrees
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>());
		glm::vec2 const Expected(-1.0f, 0.0f);

		if (!glm::all(glm::equal(A, Expected, epsilon))) {
			print_vector_comparison(A, Expected, "vec2 rotation 180°");
			Error += 1;
		}
	}

	// Test case 3: Rotate vec2 by 360 degrees (full circle)
	{
		glm::vec2 const A = glm::rotate(glm::vec2(1, 0), glm::pi<float>() * 2.0f);
		glm::vec2 const Expected(1.0f, 0.0f);

		// Use a slightly higher epsilon for this test due to accumulated floating point error
		const float full_circle_epsilon = 0.0001f;
		if (!glm::all(glm::equal(A, Expected, full_circle_epsilon))) {
			print_vector_comparison(A, Expected, "vec2 rotation 360°");
			Error += 1;
		}
	}

	// Test case 4: Rotate vec3 by 90 degrees around Z
	{
		glm::vec3 const B = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
		glm::vec3 const Expected(0.0f, 1.0f, 0.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "vec3 rotation 90° around Z");
			Error += 1;
		}
	}

	// Test case 5: Rotate vec3 by 90 degrees around Y
	{
		glm::vec3 const B = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, glm::vec3(0, 1, 0));
		glm::vec3 const Expected(0.0f, 0.0f, -1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "vec3 rotation 90° around Y");
			Error += 1;
		}
	}

	// Test case 6: Rotate vec3 by 90 degrees around X
	{
		glm::vec3 const B = glm::rotate(glm::vec3(0, 1, 0), glm::pi<float>() * 0.5f, glm::vec3(1, 0, 0));
		glm::vec3 const Expected(0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "vec3 rotation 90° around X");
			Error += 1;
		}
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

		if (glm::abs(cos_angle - expected_dot) > 0.0001f) {
			std::cout << "---- vec3 rotation arbitrary axis test failure ----\n";
			std::cout << "Expected dot product: " << expected_dot << "\n";
			std::cout << "Actual dot product:   " << cos_angle << "\n";
			std::cout << "Vector: " << glm::to_string(B) << "\n";
			Error += 1;
		}
	}

	// Test case 8: Rotate vec4 by 90 degrees around Z
	{
		glm::vec4 const C = glm::rotate(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(C, Expected, epsilon))) {
			print_vector_comparison(C, Expected, "vec4 rotation 90° around Z");
			Error += 1;
		}
	}

	// Test case 9: Rotate normalized vs non-normalized axis
	{
		glm::vec3 const normalized_axis = glm::normalize(glm::vec3(0, 0, 2));
		glm::vec3 const non_normalized_axis(0, 0, 2);

		glm::vec3 const B1 = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, normalized_axis);
		glm::vec3 const B2 = glm::rotate(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f, non_normalized_axis);

		if (!glm::all(glm::equal(B1, B2, epsilon))) {
			print_vector_comparison(B1, B2, "Normalized vs non-normalized axis");
			Error += 1;
		}
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

		if (!glm::all(glm::equal(D, Expected, epsilon))) {
			print_vector_comparison(D, Expected, "rotateX vec3 by 90°");
			Error += 1;
		}
	}

	// Test case 2: Rotate vec3 around X by 90 degrees - vector along X axis (should be invariant)
	{
		glm::vec3 const D = glm::rotateX(glm::vec3(1, 0, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(1.0f, 0.0f, 0.0f);

		if (!glm::all(glm::equal(D, Expected, epsilon))) {
			print_vector_comparison(D, Expected, "rotateX vec3 invariant axis");
			Error += 1;
		}
	}

	// Test case 3: Rotate vec3 around X by 180 degrees
	{
		glm::vec3 const D = glm::rotateX(glm::vec3(0, 1, 0), glm::pi<float>());
		glm::vec3 const Expected(0.0f, -1.0f, 0.0f);

		if (!glm::all(glm::equal(D, Expected, epsilon))) {
			print_vector_comparison(D, Expected, "rotateX vec3 by 180°");
			Error += 1;
		}
	}

	// Test case 4: Rotate vec4 around X by 90 degrees
	{
		glm::vec4 const E = glm::rotateX(glm::vec4(0, 1, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, 1.0f, 1.0f);

		if (!glm::all(glm::equal(E, Expected, epsilon))) {
			print_vector_comparison(E, Expected, "rotateX vec4 by 90°");
			Error += 1;
		}
	}

	// Test case 5: Rotate vec4 around X by 90 degrees - vector along X axis (should be invariant)
	{
		glm::vec4 const E = glm::rotateX(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(1.0f, 0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(E, Expected, epsilon))) {
			print_vector_comparison(E, Expected, "rotateX vec4 invariant axis");
			Error += 1;
		}
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

		if (!glm::all(glm::equal(F, Expected, epsilon))) {
			print_vector_comparison(F, Expected, "rotateY vec3 by 90°");
			Error += 1;
		}
	}

	// Test case 2: Rotate vec3 around Y by 90 degrees - vector along Y axis (should be invariant)
	{
		glm::vec3 const F = glm::rotateY(glm::vec3(0, 1, 0), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 1.0f, 0.0f);

		if (!glm::all(glm::equal(F, Expected, epsilon))) {
			print_vector_comparison(F, Expected, "rotateY vec3 invariant axis");
			Error += 1;
		}
	}

	// Test case 3: Rotate vec3 around Y by 180 degrees
	{
		glm::vec3 const F = glm::rotateY(glm::vec3(1, 0, 0), glm::pi<float>());
		glm::vec3 const Expected(-1.0f, 0.0f, 0.0f);

		if (!glm::all(glm::equal(F, Expected, epsilon))) {
			print_vector_comparison(F, Expected, "rotateY vec3 by 180°");
			Error += 1;
		}
	}

	// Test case 4: Rotate vec4 around Y by 90 degrees
	{
		glm::vec4 const G = glm::rotateY(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, -1.0f, 1.0f);

		if (!glm::all(glm::equal(G, Expected, epsilon))) {
			print_vector_comparison(G, Expected, "rotateY vec4 by 90°");
			Error += 1;
		}
	}

	// Test case 5: Rotate vec4 around Y by 90 degrees - vector along Y axis (should be invariant)
	{
		glm::vec4 const G = glm::rotateY(glm::vec4(0, 1, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(G, Expected, epsilon))) {
			print_vector_comparison(G, Expected, "rotateY vec4 invariant axis");
			Error += 1;
		}
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

		if (!glm::all(glm::equal(H, Expected, epsilon))) {
			print_vector_comparison(H, Expected, "rotateZ vec3 by 90°");
			Error += 1;
		}
	}

	// Test case 2: Rotate vec3 around Z by 90 degrees - vector along Z axis (should be invariant)
	{
		glm::vec3 const H = glm::rotateZ(glm::vec3(0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec3 const Expected(0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(H, Expected, epsilon))) {
			print_vector_comparison(H, Expected, "rotateZ vec3 invariant axis");
			Error += 1;
		}
	}

	// Test case 3: Rotate vec3 around Z by 180 degrees
	{
		glm::vec3 const H = glm::rotateZ(glm::vec3(1, 0, 0), glm::pi<float>());
		glm::vec3 const Expected(-1.0f, 0.0f, 0.0f);

		if (!glm::all(glm::equal(H, Expected, epsilon))) {
			print_vector_comparison(H, Expected, "rotateZ vec3 by 180°");
			Error += 1;
		}
	}

	// Test case 4: Rotate vec4 around Z by 90 degrees
	{
		glm::vec4 const I = glm::rotateZ(glm::vec4(1, 0, 0, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(I, Expected, epsilon))) {
			print_vector_comparison(I, Expected, "rotateZ vec4 by 90°");
			Error += 1;
		}
	}

	// Test case 5: Rotate vec4 around Z by 90 degrees - vector along Z axis (should be invariant)
	{
		glm::vec4 const I = glm::rotateZ(glm::vec4(0, 0, 1, 1), glm::pi<float>() * 0.5f);
		glm::vec4 const Expected(0.0f, 0.0f, 1.0f, 1.0f);

		if (!glm::all(glm::equal(I, Expected, epsilon))) {
			print_vector_comparison(I, Expected, "rotateZ vec4 invariant axis");
			Error += 1;
		}
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

		bool row0_match = glm::all(glm::equal(O[0], Expected[0], epsilon));
		bool row1_match = glm::all(glm::equal(O[1], Expected[1], epsilon));
		bool row2_match = glm::all(glm::equal(O[2], Expected[2], epsilon));
		bool row3_match = glm::all(glm::equal(O[3], Expected[3], epsilon));

		if (!row0_match || !row1_match || !row2_match || !row3_match) {
			print_matrix_comparison(O, Expected, "Basic orientation");

			// Detailed row-by-row comparison for better debugging
			if (!row0_match) {
				print_vector_comparison(O[0], Expected[0], "Orientation row 0");
			}
			if (!row1_match) {
				print_vector_comparison(O[1], Expected[1], "Orientation row 1");
			}
			if (!row2_match) {
				print_vector_comparison(O[2], Expected[2], "Orientation row 2");
			}
			if (!row3_match) {
				print_vector_comparison(O[3], Expected[3], "Orientation row 3");
			}

			Error += 1;
		}
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
					if (glm::abs(dot_product) > custom_epsilon) {
						std::cout << "---- Orientation orthogonality test failure ----\n";
						std::cout << "Columns " << i << " and " << j << " are not orthogonal\n";
						std::cout << "Dot product: " << dot_product << "\n";
						Error += 1;
					}
				}
			}

			float length = glm::length(glm::vec3(O[i]));
			if (glm::abs(length - 1.0f) > custom_epsilon) {
				std::cout << "---- Orientation column normalization test failure ----\n";
				std::cout << "Column " << i << " is not normalized\n";
				std::cout << "Length: " << length << "\n";
				Error += 1;
			}
		}

		// Check that the transformation maps (0,0,1) to either (0,1,0) or (0,-1,0)
		// The GLM implementation appears to produce (0,-1,0) in this case
		glm::vec3 const transformed = O * glm::vec4(0, 0, 1, 0);
		glm::vec3 const expected(0, -1, 0);

		if (!glm::all(glm::equal(transformed, expected, custom_epsilon))) {
			print_vector_comparison(transformed, expected, "Orientation transformation");
			Error += 1;
		}
	}

	// Test case 3: Orientation with parallel direction and up vectors (should be handled gracefully)
	{
		glm::mat4 const O = glm::orientation(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

		// The function should handle this degenerate case and produce a valid orientation matrix
		// We can test that the matrix is still orthogonal
		const float custom_epsilon = 0.0001f;

		for (int i = 0; i < 3; ++i) {
			float length = glm::length(glm::vec3(O[i]));
			if (glm::abs(length - 1.0f) > custom_epsilon) {
				std::cout << "---- Parallel vectors orientation test failure ----\n";
				std::cout << "Column " << i << " is not normalized\n";
				std::cout << "Length: " << length << "\n";
				Error += 1;
			}
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "==== Running GLM rotate_vector tests ====\n";

	std::cout << "Running rotate tests...\n";
	int RotateErrors = test_rotate();
	Error += RotateErrors;
	std::cout << (RotateErrors ?
				  "FAILED: rotate tests with " + std::to_string(RotateErrors) + " errors\n" :
				  "PASSED: rotate tests\n");

	std::cout << "Running rotateX tests...\n";
	int RotateXErrors = test_rotateX();
	Error += RotateXErrors;
	std::cout << (RotateXErrors ?
				  "FAILED: rotateX tests with " + std::to_string(RotateXErrors) + " errors\n" :
				  "PASSED: rotateX tests\n");

	std::cout << "Running rotateY tests...\n";
	int RotateYErrors = test_rotateY();
	Error += RotateYErrors;
	std::cout << (RotateYErrors ?
				  "FAILED: rotateY tests with " + std::to_string(RotateYErrors) + " errors\n" :
				  "PASSED: rotateY tests\n");

	std::cout << "Running rotateZ tests...\n";
	int RotateZErrors = test_rotateZ();
	Error += RotateZErrors;
	std::cout << (RotateZErrors ?
				  "FAILED: rotateZ tests with " + std::to_string(RotateZErrors) + " errors\n" :
				  "PASSED: rotateZ tests\n");

	std::cout << "Running orientation tests...\n";
	int OrientationErrors = test_orientation();
	Error += OrientationErrors;
	std::cout << (OrientationErrors ?
				  "FAILED: orientation tests with " + std::to_string(OrientationErrors) + " errors\n" :
				  "PASSED: orientation tests\n");

	std::cout << "==== Test summary ====\n";
	std::cout << "Total errors: " << Error << "\n";
	std::cout << (Error ? "FAILED\n" : "ALL TESTS PASSED\n");

	return Error;
}
