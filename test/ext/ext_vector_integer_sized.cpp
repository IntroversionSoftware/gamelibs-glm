#include "glm_test.hpp"
#include <glm/ext/vector_integer.hpp>
#include <glm/ext/vector_int1.hpp>
#include <glm/ext/vector_int1_sized.hpp>
#include <glm/ext/vector_uint1.hpp>
#include <glm/ext/vector_uint1_sized.hpp>

template <typename genType>
static int test_operators()
{
	int Error = 0;

	{
		genType const A(1);
		genType const B(1);

		bool const R = A != B;
		bool const S = A == B;
		Error += GLM_CHECK((S && !R));
	}

	{
		genType const A(1);
		genType const B(1);

		genType const C = A + B;
		Error += GLM_CHECK_EQ(C, genType(2));

		genType const D = A - B;
		Error += GLM_CHECK_EQ(D, genType(0));

		genType const E = A * B;
		Error += GLM_CHECK_EQ(E, genType(1));

		genType const F = A / B;
		Error += GLM_CHECK_EQ(F, genType(1));
	}

	{
		genType const A(3);
		genType const B(2);

		genType const C = A % B;
		Error += GLM_CHECK_EQ(C, genType(1));
	}

	{
		genType const A(1);
		genType const B(1);
		genType const C(0);

		genType const I = A & B;
		Error += GLM_CHECK_EQ(I, genType(1));
		genType const D = A & C;
		Error += GLM_CHECK_EQ(D, genType(0));

		genType const E = A | B;
		Error += GLM_CHECK_EQ(E, genType(1));
		genType const F = A | C;
		Error += GLM_CHECK_EQ(F, genType(1));

		genType const G = A ^ B;
		Error += GLM_CHECK_EQ(G, genType(0));
		genType const H = A ^ C;
		Error += GLM_CHECK_EQ(H, genType(1));
	}

	{
		genType B(1);
		genType C(2);

		genType D = B << B;
		Error += GLM_CHECK_EQ(D, genType(2));
		genType E = C >> B;
		Error += GLM_CHECK_EQ(E, genType(1));
	}

	return Error;
}

template <typename genType>
static int test_ctor()
{
	typedef typename genType::value_type T;

	int Error = 0;

	genType const A = genType(1);

	genType const E(genType(1));
	Error += GLM_CHECK_EQ(A, E);

	genType const F(E);
	Error += GLM_CHECK_EQ(A, F);

	genType const B = genType(1);
	genType const G(glm::vec<2, T>(1));
	Error += GLM_CHECK_EQ(B, G);

	genType const H(glm::vec<3, T>(1));
	Error += GLM_CHECK_EQ(B, H);

	genType const I(glm::vec<4, T>(1));
	Error += GLM_CHECK_EQ(B, I);

	return Error;
}

template <typename genType>
static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(typename genType::value_type), sizeof(genType));
	Error += GLM_CHECK_EQ(genType().length(), 1);
	Error += GLM_CHECK_EQ(genType::length(), 1);

	return Error;
}

template <typename genType>
static int test_relational()
{
	int Error = 0;

	genType const A(1);
	genType const B(1);
	genType const C(0);

	Error += GLM_CHECK_EQ(A, B);
	Error += GLM_CHECK_NE(A, C);
	Error += GLM_CHECK(all(equal(A, B)));
	Error += GLM_CHECK(any(notEqual(A, C)));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_operators<glm::ivec1>());
	Error += GLM_TEST(test_operators<glm::i8vec1>());
	Error += GLM_TEST(test_operators<glm::i16vec1>());
	Error += GLM_TEST(test_operators<glm::i32vec1>());
	Error += GLM_TEST(test_operators<glm::i64vec1>());

	Error += GLM_TEST(test_ctor<glm::ivec1>());
	Error += GLM_TEST(test_ctor<glm::i8vec1>());
	Error += GLM_TEST(test_ctor<glm::i16vec1>());
	Error += GLM_TEST(test_ctor<glm::i32vec1>());
	Error += GLM_TEST(test_ctor<glm::i64vec1>());

	Error += GLM_TEST(test_size<glm::ivec1>());
	Error += GLM_TEST(test_size<glm::i8vec1>());
	Error += GLM_TEST(test_size<glm::i16vec1>());
	Error += GLM_TEST(test_size<glm::i32vec1>());
	Error += GLM_TEST(test_size<glm::i64vec1>());

	Error += GLM_TEST(test_relational<glm::ivec1>());
	Error += GLM_TEST(test_relational<glm::i8vec1>());
	Error += GLM_TEST(test_relational<glm::i16vec1>());
	Error += GLM_TEST(test_relational<glm::i32vec1>());
	Error += GLM_TEST(test_relational<glm::i64vec1>());

	Error += GLM_TEST(test_operators<glm::uvec1>());
	Error += GLM_TEST(test_operators<glm::u8vec1>());
	Error += GLM_TEST(test_operators<glm::u16vec1>());
	Error += GLM_TEST(test_operators<glm::u32vec1>());
	Error += GLM_TEST(test_operators<glm::u64vec1>());

	Error += GLM_TEST(test_ctor<glm::uvec1>());
	Error += GLM_TEST(test_ctor<glm::u8vec1>());
	Error += GLM_TEST(test_ctor<glm::u16vec1>());
	Error += GLM_TEST(test_ctor<glm::u32vec1>());
	Error += GLM_TEST(test_ctor<glm::u64vec1>());

	Error += GLM_TEST(test_size<glm::uvec1>());
	Error += GLM_TEST(test_size<glm::u8vec1>());
	Error += GLM_TEST(test_size<glm::u16vec1>());
	Error += GLM_TEST(test_size<glm::u32vec1>());
	Error += GLM_TEST(test_size<glm::u64vec1>());

	Error += GLM_TEST(test_relational<glm::uvec1>());
	Error += GLM_TEST(test_relational<glm::u8vec1>());
	Error += GLM_TEST(test_relational<glm::u16vec1>());
	Error += GLM_TEST(test_relational<glm::u32vec1>());
	Error += GLM_TEST(test_relational<glm::u64vec1>());

	return GLM_TEST_FINISH(Error);
}
