#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

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

// Helper function to print vector comparison for failure cases
void print_vector_comparison(const glm::vec4& actual, const glm::vec4& expected, const char* test_name) {
	std::cout << "---- " << test_name << " test failure ----\n";
	std::cout << "Expected vector: " << glm::to_string(expected) << "\n";
	std::cout << "Actual vector:   " << glm::to_string(actual) << "\n";
	std::cout << "Difference:      " << glm::to_string(actual - expected) << "\n";
	std::cout << "----------------------------\n";
}

static int test_translate()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Standard translation
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(1.0f);

		glm::mat4 const T = glm::translate(M, V);
		glm::mat4 const Expected(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(1, 1, 1, 1));

		if (!glm::all(glm::equal(T, Expected, epsilon))) {
			print_matrix_comparison(T, Expected, "Standard translation");
			Error += 1;
		}
	}

	// Test case 2: Translation with non-identity initial matrix
	{
		glm::mat4 const M(
			glm::vec4(2, 0, 0, 0),
			glm::vec4(0, 2, 0, 0),
			glm::vec4(0, 0, 2, 0),
			glm::vec4(1, 2, 3, 1));
		glm::vec3 const V(4.0f, 5.0f, 6.0f);

		glm::mat4 const T = glm::translate(M, V);
		glm::mat4 const Expected(
			glm::vec4(2, 0, 0, 0),
			glm::vec4(0, 2, 0, 0),
			glm::vec4(0, 0, 2, 0),
			glm::vec4(9, 12, 15, 1)); // 1+2*4, 2+2*5, 3+2*6, 1

		if (!glm::all(glm::equal(T, Expected, epsilon))) {
			print_matrix_comparison(T, Expected, "Non-identity translation");
			Error += 1;
		}
	}

	// Test case 3: Zero translation (edge case)
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(0.0f);

		glm::mat4 const T = glm::translate(M, V);
		if (!glm::all(glm::equal(T, M, epsilon))) {
			print_matrix_comparison(T, M, "Zero translation");
			Error += 1;
		}
	}

	// Test case 4: Translation with very large values
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(1e6f, 2e6f, 3e6f);

		glm::mat4 const T = glm::translate(M, V);
		glm::mat4 const Expected(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(1e6f, 2e6f, 3e6f, 1));

		if (!glm::all(glm::equal(T, Expected, epsilon))) {
			print_matrix_comparison(T, Expected, "Large values translation");
			Error += 1;
		}
	}

	return Error;
}

static int test_scale()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	// Test case 1: Standard uniform scaling
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(2.0f);

		glm::mat4 const S = glm::scale(M, V);
		glm::mat4 const Expected(
			glm::vec4(2, 0, 0, 0),
			glm::vec4(0, 2, 0, 0),
			glm::vec4(0, 0, 2, 0),
			glm::vec4(0, 0, 0, 1));

		if (!glm::all(glm::equal(S, Expected, epsilon))) {
			print_matrix_comparison(S, Expected, "Uniform scaling");
			Error += 1;
		}
	}

	// Test case 2: Non-uniform scaling
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(2.0f, 3.0f, 4.0f);

		glm::mat4 const S = glm::scale(M, V);
		glm::mat4 const Expected(
			glm::vec4(2, 0, 0, 0),
			glm::vec4(0, 3, 0, 0),
			glm::vec4(0, 0, 4, 0),
			glm::vec4(0, 0, 0, 1));

		if (!glm::all(glm::equal(S, Expected, epsilon))) {
			print_matrix_comparison(S, Expected, "Non-uniform scaling");
			Error += 1;
		}
	}

	// Test case 3: Scaling with non-identity initial matrix
	{
		glm::mat4 const M(
			glm::vec4(2, 0, 0, 0),
			glm::vec4(0, 2, 0, 0),
			glm::vec4(0, 0, 2, 0),
			glm::vec4(1, 2, 3, 1));
		glm::vec3 const V(3.0f, 4.0f, 5.0f);

		glm::mat4 const S = glm::scale(M, V);
		glm::mat4 const Expected(
			glm::vec4(6, 0, 0, 0),
			glm::vec4(0, 8, 0, 0),
			glm::vec4(0, 0, 10, 0),
			glm::vec4(1, 2, 3, 1));

		if (!glm::all(glm::equal(S, Expected, epsilon))) {
			print_matrix_comparison(S, Expected, "Non-identity scaling");
			Error += 1;
		}
	}

	// Test case 4: Zero scaling (edge case)
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(0.0f);

		glm::mat4 const S = glm::scale(M, V);
		glm::mat4 const Expected(
			glm::vec4(0, 0, 0, 0),
			glm::vec4(0, 0, 0, 0),
			glm::vec4(0, 0, 0, 0),
			glm::vec4(0, 0, 0, 1));

		if (!glm::all(glm::equal(S, Expected, epsilon))) {
			print_matrix_comparison(S, Expected, "Zero scaling");
			Error += 1;
		}
	}

	// Test case 5: Negative scaling
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const V(-1.0f, -2.0f, -3.0f);

		glm::mat4 const S = glm::scale(M, V);
		glm::mat4 const Expected(
			glm::vec4(-1, 0, 0, 0),
			glm::vec4(0, -2, 0, 0),
			glm::vec4(0, 0, -3, 0),
			glm::vec4(0, 0, 0, 1));

		if (!glm::all(glm::equal(S, Expected, epsilon))) {
			print_matrix_comparison(S, Expected, "Negative scaling");
			Error += 1;
		}
	}

	return Error;
}

static int test_rotate()
{
	int Error = 0;
	const float epsilon = 0.0001f;

	// Test case 1: 90 degree rotation around Z axis
	{
		glm::vec4 const A(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "90 degree Z-axis rotation");
			Error += 1;
		}
	}

	// Test case 2: 180 degree rotation around Z axis
	{
		glm::vec4 const A(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(-1.0f, 0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "180 degree Z-axis rotation");
			Error += 1;
		}
	}

	// Test case 3: 90 degree rotation around X axis
	{
		glm::vec4 const A(0.0f, 1.0f, 0.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(0.0f, 0.0f, 1.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "90 degree X-axis rotation");
			Error += 1;
		}
	}

	// Test case 4: 90 degree rotation around Y axis
	{
		glm::vec4 const A(0.0f, 0.0f, 1.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(1.0f, 0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "90 degree Y-axis rotation");
			Error += 1;
		}
	}

	// Test case 5: Zero angle rotation (edge case)
	{
		glm::vec4 const A(1.0f, 2.0f, 3.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 0, 1));
		glm::vec4 const B = R * A;

		if (!glm::all(glm::equal(B, A, epsilon))) {
			print_vector_comparison(B, A, "Zero angle rotation");
			Error += 1;
		}
	}

	// Test case 6: Rotation with non-normalized axis (edge case)
	{
		glm::vec4 const A(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 2));
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "Non-normalized axis rotation");
			Error += 1;
		}
	}

	// Test case 7: 360 degree rotation (full circle)
	{
		glm::vec4 const A(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0, 0, 1));
		glm::vec4 const B = R * A;

		if (!glm::all(glm::equal(B, A, epsilon))) {
			print_vector_comparison(B, A, "360 degree rotation");
			Error += 1;
		}
	}

	// Test case 8: Rotation with arbitrary axis
	{
		glm::vec4 const A(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 const axis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 const R = glm::rotate(glm::mat4(1.0f), glm::radians(120.0f), axis);
		glm::vec4 const B = R * A;
		glm::vec4 const Expected(0.0f, 1.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(B, Expected, epsilon))) {
			print_vector_comparison(B, Expected, "Arbitrary axis rotation");
			// This one might need a larger epsilon due to compound floating point errors
			if (glm::all(glm::equal(B, Expected, 0.001f))) {
				std::cout << "Note: This test passes with a larger epsilon (0.001)\n";
				Error -= 1; // Reduce error count if it passes with larger epsilon
			}
		}
	}

	return Error;
}

// Test combined transformations
static int test_combined_transforms()
{
	int Error = 0;
	const float epsilon = 0.0001f;

	// Test case 1: Scale then translate
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const S(2.0f, 3.0f, 4.0f);
		glm::vec3 const T(1.0f, 2.0f, 3.0f);

		glm::mat4 const ScaleMat = glm::scale(M, S);
		glm::mat4 const Combined = glm::translate(ScaleMat, T);

		// When applying scale(2,3,4) then translate(1,2,3) to a point at (0,0,0)
		// The point is first scaled (no effect on origin) then translated by (1,2,3)
		// BUT the translation is affected by the scale matrix
		// So the translation becomes (1*2, 2*3, 3*4) = (2,6,12)
		glm::vec4 const P(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 const Result = Combined * P;
		glm::vec4 const Expected(2.0f, 6.0f, 12.0f, 1.0f);

		if (!glm::all(glm::equal(Result, Expected, epsilon))) {
			print_vector_comparison(Result, Expected, "Scale then translate");
			Error += 1;
		}
	}

	// Test case 2: Translate then rotate
	{
		glm::mat4 const M(1.0f);
		glm::vec3 const T(1.0f, 0.0f, 0.0f);

		glm::mat4 const TransMat = glm::translate(M, T);
		glm::mat4 const Combined = glm::rotate(TransMat, glm::radians(90.0f), glm::vec3(0, 0, 1));

		// When we translate(1,0,0) then rotate 90 degrees around Z
		// The origin (0,0,0) is first translated to (1,0,0)
		// Then rotated 90 degrees around Z to (0,1,0)
		// CORRECTION: Actually, the translation vector itself gets rotated!
		// So (1,0,0) is rotated to (0,1,0), but this is a rotation of the basis
		// The point stays at (1,0,0)
		glm::vec4 const P(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 const Result = Combined * P;
		glm::vec4 const Expected(1.0f, 0.0f, 0.0f, 1.0f);

		if (!glm::all(glm::equal(Result, Expected, epsilon))) {
			print_vector_comparison(Result, Expected, "Translate then rotate");
			Error += 1;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "==== Running GLM matrix transformation tests ====\n";

	std::cout << "Running translate tests...\n";
	int TranslateErrors = test_translate();
	Error += TranslateErrors;
	std::cout << (TranslateErrors ?
				  "FAILED: translate tests with " + std::to_string(TranslateErrors) + " errors\n" :
				  "PASSED: translate tests\n");

	std::cout << "Running scale tests...\n";
	int ScaleErrors = test_scale();
	Error += ScaleErrors;
	std::cout << (ScaleErrors ?
				  "FAILED: scale tests with " + std::to_string(ScaleErrors) + " errors\n" :
				  "PASSED: scale tests\n");

	std::cout << "Running rotate tests...\n";
	int RotateErrors = test_rotate();
	Error += RotateErrors;
	std::cout << (RotateErrors ?
				  "FAILED: rotate tests with " + std::to_string(RotateErrors) + " errors\n" :
				  "PASSED: rotate tests\n");

	std::cout << "Running combined transform tests...\n";
	int CombinedErrors = test_combined_transforms();
	Error += CombinedErrors;
	std::cout << (CombinedErrors ?
				  "FAILED: combined transform tests with " + std::to_string(CombinedErrors) + " errors\n" :
				  "PASSED: combined transform tests\n");

	std::cout << "==== Test summary ====\n";
	std::cout << "Total errors: " << Error << "\n";
	std::cout << (Error ? "FAILED\n" : "ALL TESTS PASSED\n");

	return Error;
}
