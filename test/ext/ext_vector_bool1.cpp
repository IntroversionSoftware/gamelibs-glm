#include "glm_test.hpp"
#include <glm/ext/vector_bool1.hpp>
#include <glm/ext/vector_bool1_precision.hpp>

template <typename genType>
static int test_operators()
{
	int Error = 0;

	genType const A(true);
	genType const B(true);
	{
		bool const R = A != B;
		bool const S = A == B;
		Error += GLM_CHECK((S && !R));
	}

	return Error;
}

template <typename genType>
static int test_ctor()
{
	int Error = 0;

	glm::bvec1 const A = genType(true);

	glm::bvec1 const E(genType(true));
	Error += GLM_CHECK_EQ(A, E);

	glm::bvec1 const F(E);
	Error += GLM_CHECK_EQ(A, F);

	return Error;
}

template <typename genType>
static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::bvec1), sizeof(genType));
	Error += GLM_CHECK_EQ(genType().length(), 1);
	Error += GLM_CHECK_EQ(genType::length(), 1);

	return Error;
}

template <typename genType>
static int test_relational()
{
	int Error = 0;

	genType const A(true);
	genType const B(true);
	genType const C(false);

	Error += GLM_CHECK_EQ(A, B);
	Error += GLM_CHECK_EQ((A && B), A);
	Error += GLM_CHECK_EQ((A || C), A);

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

	Error += GLM_TEST(test_operators<glm::bvec1>());
	Error += GLM_TEST(test_operators<glm::lowp_bvec1>());
	Error += GLM_TEST(test_operators<glm::mediump_bvec1>());
	Error += GLM_TEST(test_operators<glm::highp_bvec1>());

	Error += GLM_TEST(test_ctor<glm::bvec1>());
	Error += GLM_TEST(test_ctor<glm::lowp_bvec1>());
	Error += GLM_TEST(test_ctor<glm::mediump_bvec1>());
	Error += GLM_TEST(test_ctor<glm::highp_bvec1>());

	//Error += test_size<glm::bvec1>();
	//Error += test_size<glm::lowp_bvec1>();
	//Error += test_size<glm::mediump_bvec1>();
	//Error += test_size<glm::highp_bvec1>();

	Error += GLM_TEST(test_relational<glm::bvec1>());
	Error += GLM_TEST(test_relational<glm::lowp_bvec1>());
	Error += GLM_TEST(test_relational<glm::mediump_bvec1>());
	Error += GLM_TEST(test_relational<glm::highp_bvec1>());

	Error += GLM_TEST(test_constexpr<glm::bvec1>());
	Error += GLM_TEST(test_constexpr<glm::lowp_bvec1>());
	Error += GLM_TEST(test_constexpr<glm::mediump_bvec1>());
	Error += GLM_TEST(test_constexpr<glm::highp_bvec1>());

	return GLM_TEST_FINISH(Error);
}
