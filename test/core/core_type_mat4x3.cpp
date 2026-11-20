#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
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

static int test_operators()
{
	int Error = 0;

	glm::mat4x3 l(1.0f);
	glm::mat4x3 m(1.0f);
	glm::vec4 u(1.0f);
	glm::vec3 v(1.0f);

	float x = 1.0f;
	glm::vec3 a = m * u;
	Error += GLM_CHECK_NEAR(a, glm::vec3(1.0f), glm::epsilon<float>());

	glm::vec4 b = v * m;
	Error += GLM_CHECK_NEAR(b, glm::vec4(v, 0.0f), glm::epsilon<float>());

	glm::mat4x3 n0(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4x3 n = x / n0;
	Error += GLM_CHECK_NEAR(n, n0, glm::epsilon<float>());

	glm::mat4x3 o = m / x;
	Error += GLM_CHECK_NEAR(o, m, glm::epsilon<float>());

	glm::mat4x3 p = x * m;
	Error += GLM_CHECK_NEAR(p, m, glm::epsilon<float>());

	glm::mat4x3 q = m * x;
	bool R = glm::any(glm::notEqual(m, q, glm::epsilon<float>()));
	bool S = glm::all(glm::equal(m, l, glm::epsilon<float>()));

	Error += GLM_CHECK((S && !R));

	return Error;
}

static int test_ctr()
{
	int Error = 0;

	glm::mat4x3 m0(
		glm::vec3(0, 1, 2),
		glm::vec3(3, 4, 5),
		glm::vec3(6, 7, 8),
		glm::vec3(9, 10, 11));

	glm::mat4x3 m1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	glm::mat4x3 m2{
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{9, 10, 11}};

	Error += GLM_CHECK_NEAR(m0, m2, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(m1, m2, glm::epsilon<float>());

	std::vector<glm::mat4x3> v1{
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
	};

	std::vector<glm::mat4x3> v2{
		{
			{ 0, 1, 2 },
			{ 4, 5, 6 },
			{ 8, 9, 10 },
			{ 12, 13, 14 }
		},
		{
			{ 0, 1, 2 },
			{ 4, 5, 6 },
			{ 8, 9, 10 },
			{ 12, 13, 14 }
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
		glm::mat4x3 B(A);
		glm::mat4x3 Identity(1.0f);

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
	Error += GLM_CHECK_EQ(48, sizeof(glm::mat<4, 3, float, glm::packed_highp>));
	Error += GLM_CHECK_EQ(96, sizeof(glm::mat<4, 3, double, glm::packed_highp>));
#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
		Error += GLM_CHECK_EQ(64, sizeof(glm::mat<4, 3, float, glm::aligned_highp>));
		Error += GLM_CHECK_EQ(128, sizeof(glm::mat<4, 3, double, glm::aligned_highp>));
#	endif
	Error += GLM_CHECK_EQ(glm::mat4x3().length(), 4);
	Error += GLM_CHECK_EQ(glm::dmat4x3().length(), 4);
	Error += GLM_CHECK_EQ(glm::mat4x3::length(), 4);
	Error += GLM_CHECK_EQ(glm::dmat4x3::length(), 4);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(cast::test());
	Error += GLM_TEST(test_ctr());
	Error += GLM_TEST(test_operators());
	Error += GLM_TEST(test_size());

	return GLM_TEST_FINISH(Error);
}


