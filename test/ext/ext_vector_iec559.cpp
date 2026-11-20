#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/vector_double1.hpp>
#include <glm/ext/vector_double1_precision.hpp>
#include <glm/ext/vector_double2.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/ext/vector_double4.hpp>
#include <glm/ext/vector_float1.hpp>
#include <glm/ext/vector_float1_precision.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

template <typename genType>
static int test_operators()
{
	typedef typename genType::value_type valType;

	int Error = 0;

	{
		genType const A(1);
		genType const B(1);

		genType const C = A + B;
		Error += GLM_CHECK_NEAR(C, genType(2), glm::epsilon<valType>());

		genType const D = A - B;
		Error += GLM_CHECK_NEAR(D, genType(0), glm::epsilon<valType>());

		genType const E = A * B;
		Error += GLM_CHECK_NEAR(E, genType(1), glm::epsilon<valType>());

		genType const F = A / B;
		Error += GLM_CHECK_NEAR(F, genType(1), glm::epsilon<valType>());
	}

	return Error;
}

template <typename genType>
static int test_ctor()
{
	typedef typename genType::value_type T;

	int Error = 0;

	glm::vec<1, T> const A = genType(1);

	glm::vec<1, T> const E(genType(1));
	Error += GLM_CHECK_NEAR(A, E, glm::epsilon<T>());

	glm::vec<1, T> const F(E);
	Error += GLM_CHECK_NEAR(A, F, glm::epsilon<T>());

	genType const B = genType(1);
	genType const G(glm::vec<2, T>(1));
	Error += GLM_CHECK_NEAR(B, G, glm::epsilon<T>());

	genType const H(glm::vec<3, T>(1));
	Error += GLM_CHECK_NEAR(B, H, glm::epsilon<T>());

	genType const I(glm::vec<4, T>(1));
	Error += GLM_CHECK_NEAR(B, I, glm::epsilon<T>());

	return Error;
}

template <typename genType>
static int test_size()
{
	typedef typename genType::value_type T;

	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::vec<1, T>), sizeof(genType));
	Error += GLM_CHECK_EQ(genType().length(), 1);
	Error += GLM_CHECK_EQ(genType::length(), 1);

	return Error;
}

template <typename genType>
static int test_relational()
{
	typedef typename genType::value_type valType;

	int Error = 0;

	genType const A(1);
	genType const B(1);
	genType const C(0);

	Error += GLM_CHECK(all(equal(A, B, glm::epsilon<valType>())));
	Error += GLM_CHECK(any(notEqual(A, C, glm::epsilon<valType>())));

	return Error;
}

template <typename genType>
static int test_constexpr()
{
	static_assert(genType::length() == 1, "GLM: Failed constexpr");

	return 0;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_operators<glm::dvec1>());
	Error += GLM_TEST(test_operators<glm::lowp_dvec1>());
	Error += GLM_TEST(test_operators<glm::mediump_dvec1>());
	Error += GLM_TEST(test_operators<glm::highp_dvec1>());

	Error += GLM_TEST(test_ctor<glm::dvec1>());
	Error += GLM_TEST(test_ctor<glm::lowp_dvec1>());
	Error += GLM_TEST(test_ctor<glm::mediump_dvec1>());
	Error += GLM_TEST(test_ctor<glm::highp_dvec1>());

	Error += GLM_TEST(test_size<glm::dvec1>());
	Error += GLM_TEST(test_size<glm::lowp_dvec1>());
	Error += GLM_TEST(test_size<glm::mediump_dvec1>());
	Error += GLM_TEST(test_size<glm::highp_dvec1>());

	Error += GLM_TEST(test_relational<glm::dvec1>());
	Error += GLM_TEST(test_relational<glm::lowp_dvec1>());
	Error += GLM_TEST(test_relational<glm::mediump_dvec1>());
	Error += GLM_TEST(test_relational<glm::highp_dvec1>());

	Error += GLM_TEST(test_constexpr<glm::dvec1>());
	Error += GLM_TEST(test_constexpr<glm::lowp_dvec1>());
	Error += GLM_TEST(test_constexpr<glm::mediump_dvec1>());
	Error += GLM_TEST(test_constexpr<glm::highp_dvec1>());

	Error += GLM_TEST(test_operators<glm::vec1>());
	Error += GLM_TEST(test_operators<glm::lowp_vec1>());
	Error += GLM_TEST(test_operators<glm::mediump_vec1>());
	Error += GLM_TEST(test_operators<glm::highp_vec1>());

	Error += GLM_TEST(test_ctor<glm::vec1>());
	Error += GLM_TEST(test_ctor<glm::lowp_vec1>());
	Error += GLM_TEST(test_ctor<glm::mediump_vec1>());
	Error += GLM_TEST(test_ctor<glm::highp_vec1>());

	Error += GLM_TEST(test_size<glm::vec1>());
	Error += GLM_TEST(test_size<glm::lowp_vec1>());
	Error += GLM_TEST(test_size<glm::mediump_vec1>());
	Error += GLM_TEST(test_size<glm::highp_vec1>());

	Error += GLM_TEST(test_relational<glm::vec1>());
	Error += GLM_TEST(test_relational<glm::lowp_vec1>());
	Error += GLM_TEST(test_relational<glm::mediump_vec1>());
	Error += GLM_TEST(test_relational<glm::highp_vec1>());

	Error += GLM_TEST(test_constexpr<glm::vec1>());
	Error += GLM_TEST(test_constexpr<glm::lowp_vec1>());
	Error += GLM_TEST(test_constexpr<glm::mediump_vec1>());
	Error += GLM_TEST(test_constexpr<glm::highp_vec1>());

	return GLM_TEST_FINISH(Error);
}
