#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <ctime>
#include <cstdio>

using namespace glm;

static int test_matrixCompMult()
{
	int Error(0);

	{
		mat2 m(0, 1, 2, 3);
		mat2 n = matrixCompMult(m, m);
		mat2 expected = mat2(0, 1, 4, 9);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat2x3 m(0, 1, 2, 3, 4, 5);
		mat2x3 n = matrixCompMult(m, m);
		mat2x3 expected = mat2x3(0, 1, 4, 9, 16, 25);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat2x4 m(0, 1, 2, 3, 4, 5, 6, 7);
		mat2x4 n = matrixCompMult(m, m);
		mat2x4 expected = mat2x4(0, 1, 4, 9, 16, 25, 36, 49);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3 m(0, 1, 2, 3, 4, 5, 6, 7, 8);
		mat3 n = matrixCompMult(m, m);
		mat3 expected = mat3(0, 1, 4, 9, 16, 25, 36, 49, 64);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3x2 m(0, 1, 2, 3, 4, 5);
		mat3x2 n = matrixCompMult(m, m);
		mat3x2 expected = mat3x2(0, 1, 4, 9, 16, 25);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3x4 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
		mat3x4 n = matrixCompMult(m, m);
		mat3x4 expected = mat3x4(0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		mat4 n = matrixCompMult(m, m);
		mat4 expected = mat4(0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4x2 m(0, 1, 2, 3, 4, 5, 6, 7);
		mat4x2 n = matrixCompMult(m, m);
		mat4x2 expected = mat4x2(0, 1, 4, 9, 16, 25, 36, 49);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4x3 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
		mat4x3 n = matrixCompMult(m, m);
		mat4x3 expected = mat4x3(0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121);
		Error += all(equal(n, expected, epsilon<float>())) ? 0 : 1;
	}

	return Error;
}

static int test_outerProduct()
{
	int Error = 0;

	glm::mat2 m0 = glm::outerProduct(glm::vec2(1.0f), glm::vec2(1.0f));
	glm::mat2 n0(1, 1, 1, 1);
	Error += all(equal(m0, n0, epsilon<float>())) ? 0 : 1;

	glm::mat3 m1 = glm::outerProduct(glm::vec3(1.0f), glm::vec3(1.0f));
	glm::mat3 n1(1, 1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m1, n1, epsilon<float>())) ? 0 : 1;

	glm::mat4 m2 = glm::outerProduct(glm::vec4(1.0f), glm::vec4(1.0f));
	glm::mat4 n2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m2, n2, epsilon<float>())) ? 0 : 1;

	glm::mat2x3 m3 = glm::outerProduct(glm::vec3(1.0f), glm::vec2(1.0f));
	glm::mat2x3 n3(1, 1, 1, 1, 1, 1);
	Error += all(equal(m3, n3, epsilon<float>())) ? 0 : 1;

	glm::mat2x4 m4 = glm::outerProduct(glm::vec4(1.0f), glm::vec2(1.0f));
	glm::mat2x4 n4(1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m4, n4, epsilon<float>())) ? 0 : 1;

	glm::mat3x2 m5 = glm::outerProduct(glm::vec2(1.0f), glm::vec3(1.0f));
	glm::mat3x2 n5(1, 1, 1, 1, 1, 1);
	Error += all(equal(m5, n5, epsilon<float>())) ? 0 : 1;

	glm::mat3x4 m6 = glm::outerProduct(glm::vec4(1.0f), glm::vec3(1.0f));
	glm::mat3x4 n6(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m6, n6, epsilon<float>())) ? 0 : 1;

	glm::mat4x2 m7 = glm::outerProduct(glm::vec2(1.0f), glm::vec4(1.0f));
	glm::mat4x2 n7(1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m7, n7, epsilon<float>())) ? 0 : 1;

	glm::mat4x3 m8 = glm::outerProduct(glm::vec3(1.0f), glm::vec4(1.0f));
	glm::mat4x3 n8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	Error += all(equal(m8, n8, epsilon<float>())) ? 0 : 1;

	return Error;
}

static int test_transpose()
{
	int Error(0);

	{
		mat2 const m(0, 1, 2, 3);
		mat2 const t = transpose(m);
		mat2 const expected = mat2(0, 2, 1, 3);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat2x3 m(0, 1, 2, 3, 4, 5);
		mat3x2 t = transpose(m);
		mat3x2 const expected = mat3x2(0, 3, 1, 4, 2, 5);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat2x4 m(0, 1, 2, 3, 4, 5, 6, 7);
		mat4x2 t = transpose(m);
		mat4x2 const expected = mat4x2(0, 4, 1, 5, 2, 6, 3, 7);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3 m(0, 1, 2, 3, 4, 5, 6, 7, 8);
		mat3 t = transpose(m);
		mat3 const expected = mat3(0, 3, 6, 1, 4, 7, 2, 5, 8);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3x2 m(0, 1, 2, 3, 4, 5);
		mat2x3 t = transpose(m);
		mat2x3 const expected = mat2x3(0, 2, 4, 1, 3, 5);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat3x4 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
		mat4x3 t = transpose(m);
		mat4x3 const expected = mat4x3(0, 4, 8, 1, 5, 9, 2, 6, 10, 3, 7, 11);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		mat4 t = transpose(m);
		mat4 const expected = mat4(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4x2 m(0, 1, 2, 3, 4, 5, 6, 7);
		mat2x4 t = transpose(m);
		mat2x4 const expected = mat2x4(0, 2, 4, 6, 1, 3, 5, 7);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	{
		mat4x3 m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
		mat3x4 t = transpose(m);
		mat3x4 const expected = mat3x4(0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11);
		Error += all(equal(t, expected, epsilon<float>())) ? 0 : 1;
	}

	return Error;
}

static int test_determinant()
{
	int Error = 0;

	// Test 2x2 matrices
	{
		// Identity matrix - determinant should be 1
		glm::mat2 m0(1.0f, 0.0f, 0.0f, 1.0f);
		float det0 = glm::determinant(m0);
		Error += glm::abs(det0 - 1.0f) < glm::epsilon<float>() ? 0 : 1;

		// Simple 2x2 case: [[1,2],[3,4]] - determinant = 1*4 - 2*3 = -2
		glm::mat2 m1(1.0f, 3.0f, 2.0f, 4.0f);
		float det1 = glm::determinant(m1);
		Error += glm::abs(det1 - (-2.0f)) < glm::epsilon<float>() ? 0 : 1;

		// Singular matrix (zero determinant)
		glm::mat2 m2(1.0f, 2.0f, 2.0f, 4.0f);
		float det2 = glm::determinant(m2);
		Error += glm::abs(det2 - 0.0f) < glm::epsilon<float>() ? 0 : 1;

		// Scaling matrix - determinant should be product of diagonal elements
		glm::mat2 m3(2.0f, 0.0f, 0.0f, 3.0f);
		float det3 = glm::determinant(m3);
		Error += glm::abs(det3 - 6.0f) < glm::epsilon<float>() ? 0 : 1;
	}

	// Test 3x3 matrices
	{
		// Identity matrix
		glm::mat3 m0(1.0f);
		float det0 = glm::determinant(m0);
		Error += glm::abs(det0 - 1.0f) < glm::epsilon<float>() ? 0 : 1;

		// Simple 3x3 case: [[1,2,3],[0,1,4],[5,6,0]]
		// determinant = 1*(1*0 - 4*6) - 2*(0*0 - 4*5) + 3*(0*6 - 1*5) = 1*(-24) - 2*(-20) + 3*(-5) = -24 + 40 - 15 = 1
		glm::mat3 m1(1.0f, 0.0f, 5.0f,    // first column
					 2.0f, 1.0f, 6.0f,    // second column
					 3.0f, 4.0f, 0.0f);   // third column
		float det1 = glm::determinant(m1);
		Error += glm::abs(det1 - 1.0f) < glm::epsilon<float>() ? 0 : 1;

		// Singular 3x3 matrix (row 3 = row 1 + row 2)
		glm::mat3 m2(1.0f, 2.0f, 3.0f,    // first column
					 2.0f, 3.0f, 5.0f,    // second column
					 3.0f, 5.0f, 8.0f);   // third column (sum of first two)
		float det2 = glm::determinant(m2);
		Error += glm::abs(det2 - 0.0f) < glm::epsilon<float>() ? 0 : 1;

		// Diagonal matrix
		glm::mat3 m3(2.0f, 0.0f, 0.0f,
					 0.0f, 3.0f, 0.0f,
					 0.0f, 0.0f, 4.0f);
		float det3 = glm::determinant(m3);
		Error += glm::abs(det3 - 24.0f) < glm::epsilon<float>() ? 0 : 1;
	}

	// Test 4x4 matrices
	{
		// Identity matrix
		glm::mat4 m0(1.0f);
		float det0 = glm::determinant(m0);
		Error += glm::abs(det0 - 1.0f) < glm::epsilon<float>() ? 0 : 1;

		// Diagonal matrix
		glm::mat4 m1(2.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 3.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 4.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 5.0f);
		float det1 = glm::determinant(m1);
		Error += glm::abs(det1 - 120.0f) < glm::epsilon<float>() ? 0 : 1;

		// Upper triangular matrix - determinant should be product of diagonal
		glm::mat4 m2(2.0f, 0.0f, 0.0f, 0.0f,
					 1.0f, 3.0f, 0.0f, 0.0f,
					 4.0f, 5.0f, 2.0f, 0.0f,
					 6.0f, 7.0f, 8.0f, 4.0f);
		float det2 = glm::determinant(m2);
		Error += glm::abs(det2 - 48.0f) < glm::epsilon<float>() ? 0 : 1;

		// Known 4x4 case - using a matrix with a simple, calculable determinant
		// This matrix has determinant = 17
		glm::mat4 m3(1.0f, 0.0f, 2.0f, 0.0f,
					 2.0f, 1.0f, 0.0f, 1.0f,
					 1.0f, 2.0f, 1.0f, 0.0f,
					 0.0f, 1.0f, 1.0f, 2.0f);
		float det3 = glm::determinant(m3);
		Error += glm::abs(det3 - 17.0f) < glm::epsilon<float>() ? 0 : 1;

		// Singular 4x4 matrix (rank deficient)
		glm::mat4 m4(1.0f, 2.0f, 3.0f, 6.0f,    // columns are linearly dependent
					 2.0f, 4.0f, 6.0f, 12.0f,   // column 4 = 2 * column 3
					 1.0f, 2.0f, 3.0f, 6.0f,
					 0.0f, 0.0f, 0.0f, 0.0f);
		float det4 = glm::determinant(m4);
		Error += glm::abs(det4 - 0.0f) < glm::epsilon<float>() ? 0 : 1;
	}

	// Test edge cases
	{
		// Matrix with negative values
		glm::mat2 m0(-1.0f, 0.0f, 0.0f, -1.0f);
		float det0 = glm::determinant(m0);
		Error += glm::abs(det0 - 1.0f) < glm::epsilon<float>() ? 0 : 1;

		// Matrix with very small values (testing numerical stability)
		glm::mat2 m1(1e-6f, 0.0f, 0.0f, 1e-6f);
		float det1 = glm::determinant(m1);
		Error += glm::abs(det1 - 1e-12f) < 1e-15f ? 0 : 1;

		// Matrix with larger values
		glm::mat2 m2(100.0f, 0.0f, 0.0f, 100.0f);
		float det2 = glm::determinant(m2);
		Error += glm::abs(det2 - 10000.0f) < glm::epsilon<float>() ? 0 : 1;
	}

	// Output debug info on failures
	if (Error > 0) {
		printf("test_determinant failed with %d errors\n", Error);
	}

	return Error;
}

static int test_inverse()
{
	int Error = 0;

	{
		glm::mat4x4 A4x4(
			glm::vec4(1, 0, 1, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 0, 0, 1));
		glm::mat4x4 B4x4 = inverse(A4x4);
		glm::mat4x4 I4x4 = A4x4 * B4x4;
		glm::mat4x4 Identity(1);
		Error += all(equal(I4x4, Identity, epsilon<float>())) ? 0 : 1;
	}

	{
		glm::mat3x3 A3x3(
			glm::vec3(1, 0, 1),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1));
		glm::mat3x3 B3x3 = glm::inverse(A3x3);
		glm::mat3x3 I3x3 = A3x3 * B3x3;
		glm::mat3x3 Identity(1);
		Error += all(equal(I3x3, Identity, epsilon<float>())) ? 0 : 1;
	}

	{
		glm::mat2x2 A2x2(
			glm::vec2(1, 1),
			glm::vec2(0, 1));
		glm::mat2x2 B2x2 = glm::inverse(A2x2);
		glm::mat2x2 I2x2 = A2x2 * B2x2;
		glm::mat2x2 Identity(1);
		Error += all(equal(I2x2, Identity, epsilon<float>())) ? 0 : 1;
	}

	return Error;
}

static int test_inverse_simd()
{
	int Error = 0;

	glm::mat4x4 const Identity(1);

	glm::mat4x4 const A4x4(
		glm::vec4(1, 0, 1, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1));
	glm::mat4x4 const B4x4 = glm::inverse(A4x4);
	glm::mat4x4 const I4x4 = A4x4 * B4x4;

	Error += glm::all(glm::equal(I4x4, Identity, 0.001f)) ? 0 : 1;

	return Error;
}

static int test_shearing()
{
    int Error = 0;

    {
        glm::vec3 const center(0, 0, 0);
        glm::vec2 const l_x(2, 0);
        glm::vec2 const l_y(0, 0);
        glm::vec2 const l_z(0, 0);
        glm::mat4x4 const A4x4(
                glm::vec4(0, 0, 1, 1),
                glm::vec4(0, 1, 1, 0),
                glm::vec4(1, 1, 1, 0),
                glm::vec4(1, 1, 0, 1));
        glm::mat4x4 const B4x4 = glm::shear(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const C4x4 = glm::shear_slow(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const expected(
                glm::vec4(0, 0, 1, 1),
                glm::vec4(0, 1, 3, 2),
                glm::vec4(1, 1, 1, 0),
                glm::vec4(1, 1, 0, 1));
        Error += all(equal(B4x4, expected, epsilon<float>())) ? 0 : 1;
        Error += all(equal(C4x4, expected, epsilon<float>())) ? 0 : 1;
    }

    {
        glm::vec3 const center(0, 0, 0);
        glm::vec2 const l_x(1, 0);
        glm::vec2 const l_y(0, 1);
        glm::vec2 const l_z(1, 0);
        glm::mat4x4 const A4x4(
                glm::vec4(0, 0, 1, 0),
                glm::vec4(0, 1, 1, 0),
                glm::vec4(1, 1, 1, 0),
                glm::vec4(1, 0, 0, 0));
        glm::mat4x4 const B4x4 = glm::shear(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const C4x4 = glm::shear_slow(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const expected(
                glm::vec4(1, 1, 2, 0),
                glm::vec4(0, 1, 2, 0),
                glm::vec4(1, 2, 2, 0),
                glm::vec4(1, 0, 0, 0));
        Error += all(equal(B4x4, expected, epsilon<float>())) ? 0 : 1;
        Error += all(equal(C4x4, expected, epsilon<float>())) ? 0 : 1;
    }

    {
        glm::vec3 const center(3, 2, 1);
        glm::vec2 const l_x(1, 2);
        glm::vec2 const l_y(3, 1);
        glm::vec2 const l_z(4, 5);
        glm::mat4x4 const A4x4(1);
        glm::mat4x4 const B4x4 = glm::shear(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const C4x4 = glm::shear_slow(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const expected(
                glm::vec4(1, 3, 4, 0),
                glm::vec4(1, 1, 5, 0),
                glm::vec4(2, 1, 1, 0),
                glm::vec4(-9, -8, -9, 1));
        Error += all(equal(B4x4, expected, epsilon<float>())) ? 0 : 1;
        Error += all(equal(C4x4, expected, epsilon<float>())) ? 0 : 1;
    }

    {
        glm::vec3 const center(3, 2, 1);
        glm::vec2 const l_x(1, 2);
        glm::vec2 const l_y(3, 1);
        glm::vec2 const l_z(4, 5);
        glm::mat4x4 const A4x4(
                glm::vec4(-3, 2, 1, 0),
                glm::vec4(3, 2, 1, 0),
                glm::vec4(4, -8, 0, 0),
                glm::vec4(7, 1, -2, 0));
        glm::mat4x4 const B4x4 = glm::shear(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const C4x4 = glm::shear_slow(A4x4, center, l_x, l_y, l_z);
        glm::mat4x4 const expected(
                glm::vec4(22, -24, 4, 0),
                glm::vec4(20, -36, 2, 0),
                glm::vec4(1, -2, 3, 0),
                glm::vec4(-26, 39, -19, 0));
        Error += all(equal(B4x4, expected, epsilon<float>())) ? 0 : 1;
        Error += all(equal(C4x4, expected, epsilon<float>())) ? 0 : 1;
    }

    return Error;
}

template<typename VEC3, typename MAT4>
static int test_inverse_perf(std::size_t Count, std::size_t Instance, char const * Message)
{
	std::vector<MAT4> TestInputs;
	TestInputs.resize(Count);
	std::vector<MAT4> TestOutputs;
	TestOutputs.resize(TestInputs.size());

	VEC3 Axis(glm::normalize(VEC3(1.0f, 2.0f, 3.0f)));

	for(std::size_t i = 0; i < TestInputs.size(); ++i)
	{
		typename MAT4::value_type f = static_cast<typename MAT4::value_type>(i + Instance) * typename MAT4::value_type(0.1) + typename MAT4::value_type(0.1);
		TestInputs[i] = glm::rotate(glm::translate(MAT4(1), Axis * f), f, Axis);
		//TestInputs[i] = glm::translate(MAT4(1), Axis * f);
	}

	std::clock_t StartTime = std::clock();

	for(std::size_t i = 0; i < TestInputs.size(); ++i)
		TestOutputs[i] = glm::inverse(TestInputs[i]);

	std::clock_t EndTime = std::clock();

	for(std::size_t i = 0; i < TestInputs.size(); ++i)
		TestOutputs[i] = TestOutputs[i] * TestInputs[i];

	typename MAT4::value_type Diff(0);
	for(std::size_t Entry = 0; Entry < TestOutputs.size(); ++Entry)
	{
		MAT4 i(1.0);
		MAT4 m(TestOutputs[Entry]);
		for(glm::length_t y = 0; y < m.length(); ++y)
		for(glm::length_t x = 0; x < m[y].length(); ++x)
			Diff = glm::max(m[y][x], i[y][x]);
	}

	//glm::uint Ulp = 0;
	//Ulp = glm::max(glm::float_distance(*Dst, *Src), Ulp);

	std::printf("inverse<%s>(%f): %lu\n", Message, static_cast<double>(Diff), static_cast<unsigned long>(EndTime - StartTime));

	return 0;
}

int main()
{
	int Error = 0;

	Error += test_matrixCompMult();
	Error += test_outerProduct();
	Error += test_transpose();
	Error += test_determinant();
	Error += test_inverse();
	Error += test_inverse_simd();
	Error += test_shearing();

#ifdef NDEBUG
	std::size_t const Samples = 1000;
#else
	std::size_t const Samples = 1;
#endif//NDEBUG

	for(std::size_t i = 0; i < 1; ++i)
	{
		Error += test_inverse_perf<glm::vec3, glm::mat4>(Samples, i, "mat4");
		Error += test_inverse_perf<glm::dvec3, glm::dmat4>(Samples, i, "dmat4");
	}

	return Error;
}

