#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/constants.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/gtx/scalar_multiplication.hpp>

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
template<typename MatT>
void print_matrix_comparison(const MatT& actual, const MatT& expected, const char* test_name) {
	std::cout << "---- " << test_name << " test failure ----\n";
	std::cout << "Expected matrix:\n" << glm::to_string(expected) << "\n";
	std::cout << "Actual matrix:\n" << glm::to_string(actual) << "\n";

	// Also print the difference to help identify small precision issues
	auto diff = actual - expected;
	std::cout << "Difference (actual - expected):\n" << glm::to_string(diff) << "\n";
	std::cout << "----------------------------\n";
}

static int test_scalar_vec_multiply()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	std::cout << "Testing scalar-vector multiplication...\n";

	// Test case 1: Basic scalar multiplication with different scalar types
	{
		glm::vec3 v(0.5f, 3.1f, -9.1f);

		// Test multiplication with different scalar types (double, int, unsigned)
		if (!glm::all(glm::equal(v, 1.0 * v, epsilon))) {
			print_vector_comparison(1.0 * v, v, "Double scalar * vec3");
			Error += 1;
		}

		if (!glm::all(glm::equal(v, 1 * v, epsilon))) {
			print_vector_comparison(1 * v, v, "Int scalar * vec3");
			Error += 1;
		}

		if (!glm::all(glm::equal(v, 1u * v, epsilon))) {
			print_vector_comparison(1u * v, v, "Unsigned scalar * vec3");
			Error += 1;
		}
	}

	// Test case 2: Multiplication with various scalar values
	{
		glm::vec3 v(1.0f, 2.0f, 3.0f);
		glm::vec3 expected_2v(2.0f, 4.0f, 6.0f);
		glm::vec3 expected_half_v(0.5f, 1.0f, 1.5f);
		glm::vec3 expected_neg_v(-1.0f, -2.0f, -3.0f);

		if (!glm::all(glm::equal(2.0f * v, expected_2v, epsilon))) {
			print_vector_comparison(2.0f * v, expected_2v, "2.0f * vec3");
			Error += 1;
		}

		if (!glm::all(glm::equal(0.5f * v, expected_half_v, epsilon))) {
			print_vector_comparison(0.5f * v, expected_half_v, "0.5f * vec3");
			Error += 1;
		}

		if (!glm::all(glm::equal(-1.0f * v, expected_neg_v, epsilon))) {
			print_vector_comparison(-1.0f * v, expected_neg_v, "-1.0f * vec3");
			Error += 1;
		}
	}

	// Test case 3: Different vector types (vec2, vec3, vec4)
	{
		glm::vec2 v2(1.0f, 2.0f);
		glm::vec4 v4(1.0f, 2.0f, 3.0f, 4.0f);

		glm::vec2 expected_v2(2.0f, 4.0f);
		glm::vec4 expected_v4(2.0f, 4.0f, 6.0f, 8.0f);

		if (!glm::all(glm::equal(2.0f * v2, expected_v2, epsilon))) {
			print_vector_comparison(2.0f * v2, expected_v2, "2.0f * vec2");
			Error += 1;
		}

		if (!glm::all(glm::equal(2.0f * v4, expected_v4, epsilon))) {
			print_vector_comparison(2.0f * v4, expected_v4, "2.0f * vec4");
			Error += 1;
		}
	}

	// Test case 4: Edge cases - zero scalar, zero vector
	{
		glm::vec3 v(1.0f, 2.0f, 3.0f);
		glm::vec3 zero_vec(0.0f);

		glm::vec3 expected_zero(0.0f, 0.0f, 0.0f);

		if (!glm::all(glm::equal(0.0f * v, expected_zero, epsilon))) {
			print_vector_comparison(0.0f * v, expected_zero, "0.0f * vec3");
			Error += 1;
		}

		if (!glm::all(glm::equal(5.0f * zero_vec, expected_zero, epsilon))) {
			print_vector_comparison(5.0f * zero_vec, expected_zero, "5.0f * zero_vec");
			Error += 1;
		}
	}

	return Error;
}

static int test_scalar_mat_multiply()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	std::cout << "Testing scalar-matrix multiplication...\n";

	// Test case 1: Basic matrix scalar multiplication
	{
		glm::mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);
		glm::mat3 expected_2m(2, 4, 6, 8, 10, 12, 14, 16, 18);
		glm::mat3 result1 = 2.0f * m;
		glm::mat3 result2 = m * 2.0f;

		if (!glm::all(glm::equal(result1, expected_2m, epsilon))) {
			print_matrix_comparison(result1, expected_2m, "2.0f * mat3");
			Error += 1;
		}

		if (!glm::all(glm::equal(result2, expected_2m, epsilon))) {
			print_matrix_comparison(result2, expected_2m, "mat3 * 2.0f");
			Error += 1;
		}
	}

	// Test case 2: Different matrix types (mat2, mat3, mat4)
	{
		glm::mat2 m2(1, 2, 3, 4);
		glm::mat4 m4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

		glm::mat2 expected_m2(0.5f, 1.0f, 1.5f, 2.0f);
		glm::mat4 expected_m4(0.5f, 1.0f, 1.5f, 2.0f,
		                      2.5f, 3.0f, 3.5f, 4.0f,
		                      4.5f, 5.0f, 5.5f, 6.0f,
		                      6.5f, 7.0f, 7.5f, 8.0f);

		glm::mat2 result_m2 = 0.5f * m2;
		glm::mat4 result_m4 = 0.5f * m4;

		if (!glm::all(glm::equal(result_m2, expected_m2, epsilon))) {
			print_matrix_comparison(result_m2, expected_m2, "0.5f * mat2");
			Error += 1;
		}

		if (!glm::all(glm::equal(result_m4, expected_m4, epsilon))) {
			print_matrix_comparison(result_m4, expected_m4, "0.5f * mat4");
			Error += 1;
		}
	}

	return Error;
}

static int test_combined_operations()
{
	int Error = 0;
	const float epsilon = glm::epsilon<float>();

	std::cout << "Testing combined operations...\n";

	// Test case 1: Matrix-vector-scalar combined operations
	{
		glm::vec3 v(0.5f, 3.1f, -9.1f);
		glm::mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);

		glm::vec3 w = 0.5f * m * v;

		// Test various algebraic equivalencies
		if (!glm::all(glm::equal((m*v)/2.0f, w, epsilon))) {
			print_vector_comparison((m*v)/2.0f, w, "(m*v)/2");
			Error += 1;
		}

		if (!glm::all(glm::equal(m*(v/2.0f), w, epsilon))) {
			print_vector_comparison(m*(v/2.0f), w, "m*(v/2)");
			Error += 1;
		}

		// For (m/2.0f)*v, we need to create a new matrix and check
		glm::mat3 half_m = m / 2.0f;
		glm::vec3 result_half_m_v = half_m * v;

		if (!glm::all(glm::equal(result_half_m_v, w, epsilon))) {
			print_vector_comparison(result_half_m_v, w, "(m/2)*v");
			Error += 1;
		}

		// For (0.5f*m)*v, we need to create a new matrix and check
		glm::mat3 scaled_m = 0.5f * m;
		glm::vec3 result_scaled_m_v = scaled_m * v;

		if (!glm::all(glm::equal(result_scaled_m_v, w, epsilon))) {
			print_vector_comparison(result_scaled_m_v, w, "(0.5*m)*v");
			Error += 1;
		}

		if (!glm::all(glm::equal(0.5f*(m*v), w, epsilon))) {
			print_vector_comparison(0.5f*(m*v), w, "0.5*(m*v)");
			Error += 1;
		}
	}

	// Test case 2: More complex combined operations
	{
		glm::vec3 v(1.0f, 2.0f, 3.0f);
		glm::mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);

		// Test associativity of multiple operations
		glm::vec3 result1 = 2.0f * (3.0f * (m * v));
		glm::vec3 result2 = (2.0f * 3.0f) * (m * v);

		// For result3, we need to create the scaled matrix first
		glm::mat3 scaled_m = (2.0f * 3.0f) * m;
		glm::vec3 result3 = scaled_m * v;

		if (!glm::all(glm::equal(result1, result2, epsilon))) {
			print_vector_comparison(result1, result2, "2.0*(3.0*(m*v)) vs (2.0*3.0)*(m*v)");
			Error += 1;
		}

		if (!glm::all(glm::equal(result2, result3, epsilon))) {
			print_vector_comparison(result2, result3, "(2.0*3.0)*(m*v) vs (2.0*3.0*m)*v");
			Error += 1;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "==== Running GLM scalar multiplication tests ====\n";

	int VecErrors = test_scalar_vec_multiply();
	Error += VecErrors;
	std::cout << (VecErrors ?
	              "FAILED: scalar-vector multiplication tests with " + std::to_string(VecErrors) + " errors\n" :
	              "PASSED: scalar-vector multiplication tests\n");

	int MatErrors = test_scalar_mat_multiply();
	Error += MatErrors;
	std::cout << (MatErrors ?
	              "FAILED: scalar-matrix multiplication tests with " + std::to_string(MatErrors) + " errors\n" :
	              "PASSED: scalar-matrix multiplication tests\n");

	int CombinedErrors = test_combined_operations();
	Error += CombinedErrors;
	std::cout << (CombinedErrors ?
	              "FAILED: combined operations tests with " + std::to_string(CombinedErrors) + " errors\n" :
	              "PASSED: combined operations tests\n");

	std::cout << "==== Test summary ====\n";
	std::cout << "Total errors: " << Error << "\n";
	std::cout << (Error ? "FAILED\n" : "ALL TESTS PASSED\n");

	return Error;
}
