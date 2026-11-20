#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <limits>

// adjugate(m) * m = m * adjugate(m) = det(m) * I, for invertible and singular matrices
template<glm::length_t D, typename T>
static int test_adjugate_identity()
{
	int Error = 0;

	typedef glm::mat<D, D, T> mat;
	mat Matrices[3];
	for (glm::length_t c = 0; c < D; ++c)
		for (glm::length_t r = 0; r < D; ++r)
		{
			Matrices[0][c][r] = static_cast<T>((c * 7 + r * 3) % 5) - T(1.5);
			Matrices[1][c][r] = static_cast<T>(c == r ? 2 : 0) + static_cast<T>(c) * T(0.25);
			Matrices[2][c][r] = static_cast<T>(c + 1) * static_cast<T>(r + 1); // rank 1
		}

	for (mat const& m : Matrices)
	{
		mat const DetI = mat(glm::determinant(m));
		Error += glm_test::context(GLM_CHECK_NEAR(glm::adjugate(m) * m, DetI, T(1e-4)), "m", m);
		Error += glm_test::context(GLM_CHECK_NEAR(m * glm::adjugate(m), DetI, T(1e-4)), "m", m);
	}

	return Error;
}

static int test_adjugate()
{
	int Error = 0;

	const float epsilon = std::numeric_limits<float>::epsilon();

	// mat2
	const glm::mat2 m2(
		2, 3,
		1, 5
	);

	const glm::mat2 eam2(
		5, -3,
		-1, 2
	);

	const glm::mat2 am2 = glm::adjugate(m2);

	Error += GLM_CHECK_NEAR(am2, eam2, epsilon);

	// mat3
	const glm::mat3 m3(
		2, 3, 3,
		1, 5, 4,
		4, 6, 8
	);

	const glm::mat3 eam3(
		16, -6, -3,
		8, 4, -5,
		-14, 0, 7
	);

	const glm::mat3 am3 = glm::adjugate(m3);

	Error += GLM_CHECK_NEAR(am3, eam3, epsilon);

	// mat4
	const glm::mat4 m4(
		2, 3, 3, 1,
		1, 5, 4, 3,
		4, 6, 8, 5,
		-2, -3, -3, 4
	);

	const glm::mat4 eam4(
		97, -30, -15, 17,
		45, 20, -25, 5,
		-91, 0, 35, -21,
		14, 0, 0, 14
	);

	const glm::mat4 am4 = glm::adjugate(m4);

	Error += GLM_CHECK_NEAR(am4, eam4, epsilon);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_adjugate());
	Error += GLM_TEST(test_adjugate_identity<2, float>());
	Error += GLM_TEST(test_adjugate_identity<3, float>());
	Error += GLM_TEST(test_adjugate_identity<4, float>());
	Error += GLM_TEST(test_adjugate_identity<3, double>());

	return GLM_TEST_FINISH(Error);
}
