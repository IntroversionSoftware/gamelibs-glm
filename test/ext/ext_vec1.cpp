#define GLM_FORCE_SWIZZLE
#include "glm_test.hpp"
#include <glm/vector_relational.hpp>
#include <glm/gtc/vec1.hpp>
#include <vector>

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wglobal-constructors"
#	pragma clang diagnostic ignored "-Wunused-variable"
#endif

static glm::vec1 g1;
static glm::vec1 g2(1);

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

static int test_vec1_operators()
{
	int Error(0);

	glm::ivec1 A(1);
	glm::ivec1 B(1);
	{
		bool R = A != B;
		bool S = A == B;

		Error += GLM_CHECK((S && !R));
	}

	{
		A *= 1;
		B *= 1;
		A += 1;
		B += 1;

		bool R = A != B;
		bool S = A == B;

		Error += GLM_CHECK((S && !R));
	}

	return Error;
}

static int test_vec1_ctor()
{
	int Error = 0;

	{
	//	Error += std::is_trivially_default_constructible<glm::vec1>::value ? 0 : 1;
	//	Error += std::is_trivially_copy_assignable<glm::vec1>::value ? 0 : 1;
		Error += GLM_CHECK(std::is_trivially_copyable<glm::vec1>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::dvec1>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::ivec1>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::uvec1>::value);

		Error += GLM_CHECK(std::is_copy_constructible<glm::vec1>::value);
	}

	{
		glm::ivec1 A = glm::vec1(2.0f);

		glm::ivec1 E(glm::dvec1(2.0));
		Error += GLM_CHECK_EQ(A, E);

		glm::ivec1 F(glm::ivec1(2));
		Error += GLM_CHECK_EQ(A, F);
	}

	return Error;
}

static int test_vec1_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::vec1), sizeof(glm::mediump_vec1));
	Error += GLM_CHECK_EQ(4, sizeof(glm::mediump_vec1));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec1), sizeof(glm::highp_dvec1));
	Error += GLM_CHECK_EQ(8, sizeof(glm::highp_dvec1));
	Error += GLM_CHECK_EQ(glm::vec1().length(), 1);
	Error += GLM_CHECK_EQ(glm::dvec1().length(), 1);
	Error += GLM_CHECK_EQ(glm::vec1::length(), 1);
	Error += GLM_CHECK_EQ(glm::dvec1::length(), 1);

	GLM_CONSTEXPR glm::length_t Length = glm::vec1::length();
	Error += GLM_CHECK_EQ(Length, 1);

	return Error;
}

static int test_vec1_operator_increment()
{
	int Error(0);

	glm::ivec1 v0(1);
	glm::ivec1 v1(v0);
	glm::ivec1 v2(v0);
	glm::ivec1 v3 = ++v1;
	glm::ivec1 v4 = v2++;

	Error += GLM_CHECK(glm::all(glm::equal(v0, v4)));
	Error += GLM_CHECK(glm::all(glm::equal(v1, v2)));
	Error += GLM_CHECK(glm::all(glm::equal(v1, v3)));

	int i0(1);
	int i1(i0);
	int i2(i0);
	int i3 = ++i1;
	int i4 = i2++;

	Error += GLM_CHECK_EQ(i0, i4);
	Error += GLM_CHECK_EQ(i1, i2);
	Error += GLM_CHECK_EQ(i1, i3);

	return Error;
}

static int test_bvec1_ctor()
{
	int Error = 0;

	glm::bvec1 A(true);
	glm::bvec1 B(true);
	glm::bvec1 C(false);
	glm::bvec1 D = A && B;
	glm::bvec1 E = A && C;
	glm::bvec1 F = A || C;

	Error += GLM_CHECK_EQ(D, glm::bvec1(true));
	Error += GLM_CHECK_EQ(E, glm::bvec1(false));
	Error += GLM_CHECK_EQ(F, glm::bvec1(true));

	bool G = A == C;
	bool H = A != C;
	Error += GLM_CHECK(!G);
	Error += GLM_CHECK(H);

	return Error;
}

int main()
{
	// Suppress unused variable warnings
	(void)g1;
	(void)g2;

	int Error = 0;

	Error += GLM_TEST(test_vec1_size());
	Error += GLM_TEST(test_vec1_ctor());
	Error += GLM_TEST(test_bvec1_ctor());
	Error += GLM_TEST(test_vec1_operators());
	Error += GLM_TEST(test_vec1_operator_increment());

	return GLM_TEST_FINISH(Error);
}
