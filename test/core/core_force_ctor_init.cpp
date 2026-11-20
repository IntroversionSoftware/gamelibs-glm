#define GLM_FORCE_CTOR_INIT

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

static int test_vec()
{
	int Error = 0;

	glm::vec1 V1;
	Error += GLM_CHECK_NEAR(V1, glm::vec1(0), glm::epsilon<float>());

	glm::dvec1 U1;
	Error += GLM_CHECK_NEAR(U1, glm::dvec1(0), glm::epsilon<double>());

	glm::vec2 V2;
	Error += GLM_CHECK_NEAR(V2, glm::vec2(0, 0), glm::epsilon<float>());

	glm::dvec2 U2;
	Error += GLM_CHECK_NEAR(U2, glm::dvec2(0, 0), glm::epsilon<double>());

	glm::vec3 V3;
	Error += GLM_CHECK_NEAR(V3, glm::vec3(0, 0, 0), glm::epsilon<float>());

	glm::dvec3 U3;
	Error += GLM_CHECK_NEAR(U3, glm::dvec3(0, 0, 0), glm::epsilon<double>());

	glm::vec4 V4;
	Error += GLM_CHECK_NEAR(V4, glm::vec4(0, 0, 0, 0), glm::epsilon<float>());

	glm::dvec4 U4;
	Error += GLM_CHECK_NEAR(U4, glm::dvec4(0, 0, 0, 0), glm::epsilon<double>());

	return Error;
}

static int test_mat()
{
	int Error = 0;

	{
		glm::mat2x2 F;
		Error += GLM_CHECK_NEAR(F, glm::mat2x2(1), glm::epsilon<float>());

		glm::dmat2x2 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat2x2(1), glm::epsilon<double>());
	}

	{
		glm::mat2x3 F;
		Error += GLM_CHECK_NEAR(F, glm::mat2x3(1), glm::epsilon<float>());

		glm::dmat2x3 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat2x3(1), glm::epsilon<double>());
	}

	{
		glm::mat2x4 F;
		Error += GLM_CHECK_NEAR(F, glm::mat2x4(1), glm::epsilon<float>());

		glm::dmat2x4 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat2x4(1), glm::epsilon<double>());
	}

	{
		glm::mat3x2 F;
		Error += GLM_CHECK_NEAR(F, glm::mat3x2(1), glm::epsilon<float>());

		glm::dmat3x2 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat3x2(1), glm::epsilon<double>());
	}

	{
		glm::mat3x3 F;
		Error += GLM_CHECK_NEAR(F, glm::mat3x3(1), glm::epsilon<float>());

		glm::dmat3x3 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat3x3(1), glm::epsilon<double>());
	}

	{
		glm::mat3x4 F;
		Error += GLM_CHECK_NEAR(F, glm::mat3x4(1), glm::epsilon<float>());

		glm::dmat3x4 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat3x4(1), glm::epsilon<double>());
	}

	{
		glm::mat4x2 F;
		Error += GLM_CHECK_NEAR(F, glm::mat4x2(1), glm::epsilon<float>());

		glm::dmat4x2 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat4x2(1), glm::epsilon<double>());
	}

	{
		glm::mat4x3 F;
		Error += GLM_CHECK_NEAR(F, glm::mat4x3(1), glm::epsilon<float>());

		glm::dmat4x3 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat4x3(1), glm::epsilon<double>());
	}

	{
		glm::mat4x4 F;
		Error += GLM_CHECK_NEAR(F, glm::mat4x4(1), glm::epsilon<float>());

		glm::dmat4x4 D;
		Error += GLM_CHECK_NEAR(D, glm::dmat4x4(1), glm::epsilon<double>());
	}

	return Error;
}

static int test_qua()
{
	int Error = 0;

	glm::quat F;
	Error += GLM_CHECK_NEAR(F, glm::quat(1, 0, 0, 0), glm::epsilon<float>());

	glm::dquat D;
	Error += GLM_CHECK_NEAR(D, glm::dquat(1, 0, 0, 0), glm::epsilon<double>());

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_vec());
	Error += GLM_TEST(test_mat());
	Error += GLM_TEST(test_qua());

	return GLM_TEST_FINISH(Error);
}

