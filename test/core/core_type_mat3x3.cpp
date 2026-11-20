#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/matrix.hpp>
#include <glm/vector_relational.hpp>
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

static int test_mat3x3()
{
	glm::dmat3 Mat0(
		glm::dvec3(0.6f, 0.2f, 0.3f),
		glm::dvec3(0.2f, 0.7f, 0.5f),
		glm::dvec3(0.3f, 0.5f, 0.7f));
	glm::dmat3 Inv0 = glm::inverse(Mat0);
	glm::dmat3 Res0 = Mat0 * Inv0;

	return glm::all(glm::equal(Res0, glm::dmat3(1.0), 0.01)) ? 0 : 1;
}

static int test_operators()
{
	int Error = 0;

	glm::mat3x3 l(1.0f);
	glm::mat3x3 m(1.0f);
	glm::vec3 u(1.0f);
	glm::vec3 v(1.0f);

	float x = 1.0f;
	glm::vec3 a = m * u;
	Error += GLM_CHECK_NEAR(a, glm::vec3(1.0f), glm::epsilon<float>());

	glm::vec3 b = v * m;
	Error += GLM_CHECK_NEAR(b, glm::vec3(1.0f), glm::epsilon<float>());

	glm::mat3x3 n0(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat3x3 n = x / n0;
	Error += GLM_CHECK_NEAR(n, n0, glm::epsilon<float>());

	glm::mat3x3 o = m / x;
	Error += GLM_CHECK_NEAR(o, m, glm::epsilon<float>());

	glm::mat3x3 p = x * m;
	Error += GLM_CHECK_NEAR(p, m, glm::epsilon<float>());

	glm::mat3x3 q = m * x;
	bool R = glm::any(glm::notEqual(m, q, glm::epsilon<float>()));
	bool S = glm::all(glm::equal(m, l, glm::epsilon<float>()));

	Error += GLM_CHECK((S && !R));

	return Error;
}

static int test_inverse()
{
	int Error(0);

	{
		glm::mat3 const Matrix(
			glm::vec3(0.6f, 0.2f, 0.3f),
			glm::vec3(0.2f, 0.7f, 0.5f),
			glm::vec3(0.3f, 0.5f, 0.7f));
		glm::mat3 const Inverse = glm::inverse(Matrix);
		glm::mat3 const Identity = Matrix * Inverse;

		Error += GLM_CHECK_NEAR(Identity[0], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.01f));
		Error += GLM_CHECK_NEAR(Identity[1], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.01f));
		Error += GLM_CHECK_NEAR(Identity[2], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.01f));
	}

	{
		glm::mat3 const Matrix(
			glm::vec3(0.6f, 0.2f, 0.3f),
			glm::vec3(0.2f, 0.7f, 0.5f),
			glm::vec3(0.3f, 0.5f, 0.7f));
		glm::mat3 const Identity = Matrix / Matrix;

		Error += GLM_CHECK_NEAR(Identity[0], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.01f));
		Error += GLM_CHECK_NEAR(Identity[1], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.01f));
		Error += GLM_CHECK_NEAR(Identity[2], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.01f));
	}

	return Error;
}

static int test_ctr()
{
	int Error = 0;

	glm::mat3x3 m0(
		glm::vec3(0, 1, 2),
		glm::vec3(3, 4, 5),
		glm::vec3(6, 7, 8));

	glm::mat3x3 m1{0, 1, 2, 3, 4, 5, 6, 7, 8};

	glm::mat3x3 m2{
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8}};

	Error += GLM_CHECK_NEAR(m0, m2, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(m1, m2, glm::epsilon<float>());

	std::vector<glm::mat3x3> v1{
		{0, 1, 2, 3, 4, 5, 6, 7, 8},
		{0, 1, 2, 3, 4, 5, 6, 7, 8}
	};

	std::vector<glm::mat3x3> v2{
		{
			{ 0, 1, 2},
			{ 3, 4, 5},
			{ 6, 7, 8}
		},
		{
			{ 0, 1, 2},
			{ 3, 4, 5},
			{ 6, 7, 8}
		}
	};

	return Error;
}

namespace cast
{
	template<typename genType>
	static int entry()
	{
		int Error = 0;

		genType A(1.0f);
		glm::mat3x3 B(A);
		glm::mat3x3 Identity(1.0f);

		Error += GLM_CHECK_NEAR(B, Identity, glm::epsilon<float>());

		return Error;
	}

	static int test()
	{
		int Error = 0;

		Error += entry<glm::mat2x2>();
		Error += entry<glm::mat2x3>();
		Error += entry<glm::mat2x4>();
		Error += entry<glm::mat3x2>();
		Error += entry<glm::mat3x3>();
		Error += entry<glm::mat3x4>();
		Error += entry<glm::mat4x2>();
		Error += entry<glm::mat4x3>();
		Error += entry<glm::mat4x4>();

		return Error;
	}
}//namespace cast

static int test_size()
{
	int Error = 0;

	// Packed matrices are tightly packed; aligned ones pad each 3-row column to 4 components.
	Error += GLM_CHECK_EQ(36, sizeof(glm::mat<3, 3, float, glm::packed_highp>));
	Error += GLM_CHECK_EQ(72, sizeof(glm::mat<3, 3, double, glm::packed_highp>));
#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
		Error += GLM_CHECK_EQ(48, sizeof(glm::mat<3, 3, float, glm::aligned_highp>));
		Error += GLM_CHECK_EQ(96, sizeof(glm::mat<3, 3, double, glm::aligned_highp>));
#	endif
	Error += GLM_CHECK_EQ(glm::mat3x3().length(), 3);
	Error += GLM_CHECK_EQ(glm::dmat3x3().length(), 3);
	Error += GLM_CHECK_EQ(glm::mat3x3::length(), 3);
	Error += GLM_CHECK_EQ(glm::dmat3x3::length(), 3);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(cast::test());
	Error += GLM_TEST(test_ctr());
	Error += GLM_TEST(test_mat3x3());
	Error += GLM_TEST(test_operators());
	Error += GLM_TEST(test_inverse());
	Error += GLM_TEST(test_size());

	return GLM_TEST_FINISH(Error);
}

