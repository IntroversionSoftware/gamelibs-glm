#define GLM_FORCE_SWIZZLE
#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/vector_relational.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdio>
#include <ctime>
#include <vector>

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wglobal-constructors"
#	pragma clang diagnostic ignored "-Wunused-variable"
#endif

static glm::vec4 g1;
static glm::vec4 g2(1);
static glm::vec4 g3(1, 1, 1, 1);

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

template <int Value>
struct mask
{
	enum{value = Value};
};

enum comp
{
	X,
	Y,
	Z,
	W
};

//template<comp X, comp Y, comp Z, comp W>
//__m128 swizzle(glm::vec4 const& v)
//{
//	__m128 Src = _mm_set_ps(v.w, v.z, v.y, v.x);
//	return _mm_shuffle_ps(Src, Src, mask<(int(W) << 6) | (int(Z) << 4) | (int(Y) << 2) | (int(X) << 0)>::value);
//}

static int test_vec4_ctor()
{
	int Error = 0;

	{
		glm::ivec4 A(1, 2, 3, 4);
		glm::ivec4 B(A);
		Error += GLM_CHECK(glm::all(glm::equal(A, B)));
	}
	
	{
		glm::vec4 a{ 0, 1, 2, 3 };
		std::vector<glm::vec4> v = {
			a,
			{4, 5, 6, 7},
			{8, 9, 0, 1}};
	}

	{
		glm::dvec4 a{ 0, 1, 2, 3 };
		std::vector<glm::dvec4> v = {
			a,
			{4, 5, 6, 7},
			{8, 9, 0, 1}};
	}

	{
		glm::ivec4 A(1);
		glm::ivec4 B(1, 1, 1, 1);

		Error += GLM_CHECK_EQ(A, B);
	}

	{
		std::vector<glm::ivec4> Tests;
		Tests.push_back(glm::ivec4(glm::ivec2(1, 2), 3, 4));
		Tests.push_back(glm::ivec4(1, glm::ivec2(2, 3), 4));
		Tests.push_back(glm::ivec4(1, 2, glm::ivec2(3, 4)));
		Tests.push_back(glm::ivec4(glm::ivec3(1, 2, 3), 4));
		Tests.push_back(glm::ivec4(1, glm::ivec3(2, 3, 4)));
		Tests.push_back(glm::ivec4(glm::ivec2(1, 2), glm::ivec2(3, 4)));
		Tests.push_back(glm::ivec4(1, 2, 3, 4));
		Tests.push_back(glm::ivec4(glm::ivec4(1, 2, 3, 4)));

		for(std::size_t i = 0; i < Tests.size(); ++i)
			Error += GLM_CHECK_EQ(Tests[i], glm::ivec4(1, 2, 3, 4));
	}

	{
		glm::vec1 R(1.0f);
		glm::vec1 S(2.0f);
		glm::vec1 T(3.0f);
		glm::vec1 U(4.0f);
		glm::vec4 O(1.0f, 2.0f, 3.0f, 4.0f);

		glm::vec4 A(R);
		glm::vec4 B(1.0f);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());

		glm::vec4 C(R, S, T, U);
		Error += GLM_CHECK_NEAR(C, O, glm::epsilon<float>());

		glm::vec4 D(R, 2.0f, 3.0f, 4.0f);
		Error += GLM_CHECK_NEAR(D, O, glm::epsilon<float>());

		glm::vec4 E(1.0f, S, 3.0f, 4.0f);
		Error += GLM_CHECK_NEAR(E, O, glm::epsilon<float>());

		glm::vec4 F(R, S, 3.0f, 4.0f);
		Error += GLM_CHECK_NEAR(F, O, glm::epsilon<float>());

		glm::vec4 G(1.0f, 2.0f, T, 4.0f);
		Error += GLM_CHECK_NEAR(G, O, glm::epsilon<float>());

		glm::vec4 H(R, 2.0f, T, 4.0f);
		Error += GLM_CHECK_NEAR(H, O, glm::epsilon<float>());

		glm::vec4 I(1.0f, S, T, 4.0f);
		Error += GLM_CHECK_NEAR(I, O, glm::epsilon<float>());

		glm::vec4 J(R, S, T, 4.0f);
		Error += GLM_CHECK_NEAR(J, O, glm::epsilon<float>());

		glm::vec4 K(R, 2.0f, 3.0f, U);
		Error += GLM_CHECK_NEAR(K, O, glm::epsilon<float>());

		glm::vec4 L(1.0f, S, 3.0f, U);
		Error += GLM_CHECK_NEAR(L, O, glm::epsilon<float>());

		glm::vec4 M(R, S, 3.0f, U);
		Error += GLM_CHECK_NEAR(M, O, glm::epsilon<float>());

		glm::vec4 N(1.0f, 2.0f, T, U);
		Error += GLM_CHECK_NEAR(N, O, glm::epsilon<float>());

		glm::vec4 P(R, 2.0f, T, U);
		Error += GLM_CHECK_NEAR(P, O, glm::epsilon<float>());

		glm::vec4 Q(1.0f, S, T, U);
		Error += GLM_CHECK_NEAR(Q, O, glm::epsilon<float>());

		glm::vec4 V(R, S, T, U);
		Error += GLM_CHECK_NEAR(V, O, glm::epsilon<float>());
	}

	{
		glm::vec1 const R(1.0f);
		glm::dvec1 const S(2.0);
		glm::vec1 const T(3.0);
		glm::dvec1 const U(4.0);
		glm::vec4 const O(1.0f, 2.0, 3.0f, 4.0);

		glm::vec4 const A(R);
		glm::vec4 const B(1.0);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());

		glm::vec4 const C(R, S, T, U);
		Error += GLM_CHECK_NEAR(C, O, glm::epsilon<float>());

		glm::vec4 const D(R, 2.0f, 3.0, 4.0f);
		Error += GLM_CHECK_NEAR(D, O, glm::epsilon<float>());

		glm::vec4 const E(1.0, S, 3.0f, 4.0);
		Error += GLM_CHECK_NEAR(E, O, glm::epsilon<float>());

		glm::vec4 const F(R, S, 3.0, 4.0f);
		Error += GLM_CHECK_NEAR(F, O, glm::epsilon<float>());

		glm::vec4 const G(1.0f, 2.0, T, 4.0);
		Error += GLM_CHECK_NEAR(G, O, glm::epsilon<float>());

		glm::vec4 const H(R, 2.0, T, 4.0);
		Error += GLM_CHECK_NEAR(H, O, glm::epsilon<float>());

		glm::vec4 const I(1.0, S, T, 4.0f);
		Error += GLM_CHECK_NEAR(I, O, glm::epsilon<float>());

		glm::vec4 const J(R, S, T, 4.0f);
		Error += GLM_CHECK_NEAR(J, O, glm::epsilon<float>());

		glm::vec4 const K(R, 2.0f, 3.0, U);
		Error += GLM_CHECK_NEAR(K, O, glm::epsilon<float>());

		glm::vec4 const L(1.0f, S, 3.0, U);
		Error += GLM_CHECK_NEAR(L, O, glm::epsilon<float>());

		glm::vec4 const M(R, S, 3.0, U);
		Error += GLM_CHECK_NEAR(M, O, glm::epsilon<float>());

		glm::vec4 const N(1.0f, 2.0, T, U);
		Error += GLM_CHECK_NEAR(N, O, glm::epsilon<float>());

		glm::vec4 const P(R, 2.0, T, U);
		Error += GLM_CHECK_NEAR(P, O, glm::epsilon<float>());

		glm::vec4 const Q(1.0f, S, T, U);
		Error += GLM_CHECK_NEAR(Q, O, glm::epsilon<float>());

		glm::vec4 const V(R, S, T, U);
		Error += GLM_CHECK_NEAR(V, O, glm::epsilon<float>());
	}

	{
		glm::vec1 v1_0(1.0f);
		glm::vec1 v1_1(2.0f);
		glm::vec1 v1_2(3.0f);
		glm::vec1 v1_3(4.0f);

		glm::vec2 v2_0(1.0f, 2.0f);
		glm::vec2 v2_1(2.0f, 3.0f);
		glm::vec2 v2_2(3.0f, 4.0f);

		glm::vec3 v3_0(1.0f, 2.0f, 3.0f);
		glm::vec3 v3_1(2.0f, 3.0f, 4.0f);

		glm::vec4 O(1.0f, 2.0, 3.0f, 4.0);

		glm::vec4 A(v1_0, v1_1, v2_2);
		Error += GLM_CHECK_NEAR(A, O, glm::epsilon<float>());

		glm::vec4 B(1.0f, 2.0f, v2_2);
		Error += GLM_CHECK_NEAR(B, O, glm::epsilon<float>());

		glm::vec4 C(v1_0, 2.0f, v2_2);
		Error += GLM_CHECK_NEAR(C, O, glm::epsilon<float>());

		glm::vec4 D(1.0f, v1_1, v2_2);
		Error += GLM_CHECK_NEAR(D, O, glm::epsilon<float>());

		glm::vec4 E(v2_0, v1_2, v1_3);
		Error += GLM_CHECK_NEAR(E, O, glm::epsilon<float>());

		glm::vec4 F(v2_0, 3.0, v1_3);
		Error += GLM_CHECK_NEAR(F, O, glm::epsilon<float>());

		glm::vec4 G(v2_0, v1_2, 4.0);
		Error += GLM_CHECK_NEAR(G, O, glm::epsilon<float>());

		glm::vec4 H(v2_0, 3.0f, 4.0);
		Error += GLM_CHECK_NEAR(H, O, glm::epsilon<float>());

		glm::vec4 I(v3_0, 4.0);
		Error += GLM_CHECK_NEAR(I, O, glm::epsilon<float>());

		glm::vec4 J(1.0f, v3_1);
		Error += GLM_CHECK_NEAR(J, O, glm::epsilon<float>());

		glm::vec4 K(1.0f, v2_1, 4.0f);
		Error += GLM_CHECK_NEAR(J, O, glm::epsilon<float>());
	}

	return Error;
}

static int test_bvec4_ctor()
{
	int Error = 0;

	glm::bvec4 A(true);
	glm::bvec4 B(true);
	glm::bvec4 C(false);
	glm::bvec4 D = A && B;
	glm::bvec4 E = A && C;
	glm::bvec4 F = A || C;

	Error += GLM_CHECK_EQ(D, glm::bvec4(true));
	Error += GLM_CHECK_EQ(E, glm::bvec4(false));
	Error += GLM_CHECK_EQ(F, glm::bvec4(true));

	bool G = A == C;
	bool H = A != C;
	Error += GLM_CHECK(!G);
	Error += GLM_CHECK(H);

	return Error;
}

static int test_operators()
{
	int Error = 0;

	{
		glm::ivec4 A(1);
		glm::ivec4 B(1);
		bool R = A != B;
		bool S = A == B;

		Error += GLM_CHECK((S && !R));
	}

	{
		glm::vec4 const A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::vec4 const B(4.0f, 5.0f, 6.0f, 7.0f);

		glm::vec4 const C = A + B;
		Error += GLM_CHECK_NEAR(C, glm::vec4(5, 7, 9, 11), glm::epsilon<float>());

		glm::vec4 const D = B - A;
		Error += GLM_CHECK_NEAR(D, glm::vec4(3, 3, 3, 3), glm::epsilon<float>());

		glm::vec4 const E = A * B;
		Error += GLM_CHECK_NEAR(E, glm::vec4(4, 10, 18, 28), glm::epsilon<float>());

		glm::vec4 const F = B / A;
		Error += GLM_CHECK_NEAR(F, glm::vec4(4, 2.5, 2, 7.0f / 4.0f), glm::epsilon<float>());

		glm::vec4 const G = A + 1.0f;
		Error += GLM_CHECK_NEAR(G, glm::vec4(2, 3, 4, 5), glm::epsilon<float>());

		glm::vec4 const H = B - 1.0f;
		Error += GLM_CHECK_NEAR(H, glm::vec4(3, 4, 5, 6), glm::epsilon<float>());

		glm::vec4 const I = A * 2.0f;
		Error += GLM_CHECK_NEAR(I, glm::vec4(2, 4, 6, 8), glm::epsilon<float>());

		glm::vec4 const J = B / 2.0f;
		Error += GLM_CHECK_NEAR(J, glm::vec4(2, 2.5, 3, 3.5), glm::epsilon<float>());

		glm::vec4 const K = 1.0f + A;
		Error += GLM_CHECK_NEAR(K, glm::vec4(2, 3, 4, 5), glm::epsilon<float>());

		glm::vec4 const L = 1.0f - B;
		Error += GLM_CHECK_NEAR(L, glm::vec4(-3, -4, -5, -6), glm::epsilon<float>());

		glm::vec4 const M = 2.0f * A;
		Error += GLM_CHECK_NEAR(M, glm::vec4(2, 4, 6, 8), glm::epsilon<float>());

		glm::vec4 const N = 2.0f / B;
		Error += GLM_CHECK_NEAR(N, glm::vec4(0.5, 2.0 / 5.0, 2.0 / 6.0, 2.0 / 7.0), glm::epsilon<float>());
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		A += B;
		Error += GLM_CHECK_EQ(A, glm::ivec4(5, 7, 9, 11));

		A += 1;
		Error += GLM_CHECK_EQ(A, glm::ivec4(6, 8, 10, 12));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		B -= A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(3, 3, 3, 3));

		B -= 1;
		Error += GLM_CHECK_EQ(B, glm::ivec4(2, 2, 2, 2));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		A *= B;
		Error += GLM_CHECK_EQ(A, glm::ivec4(4, 10, 18, 28));

		A *= 2;
		Error += GLM_CHECK_EQ(A, glm::ivec4(8, 20, 36, 56));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 2.0f, 4.0f);
		glm::ivec4 B(4.0f, 4.0f, 8.0f, 8.0f);

		B /= A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(4, 2, 4, 2));

		B /= 2;
		Error += GLM_CHECK_EQ(B, glm::ivec4(2, 1, 2, 1));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 2.0f, 4.0f);
		glm::ivec4 B(4.0f, 4.0f, 8.0f, 8.0f);

		B = B / A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(4, 2, 4, 2));

		B = B / 2;
		Error += GLM_CHECK_EQ(B, glm::ivec4(2, 1, 2, 1));
	}
	{
		glm::ivec4 B(2);

		B /= B.y;
		Error += GLM_CHECK_EQ(B, glm::ivec4(1));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = -A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(-1.0f, -2.0f, -3.0f, -4.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = --A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(0.0f, 1.0f, 2.0f, 3.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A--;
		Error += GLM_CHECK_EQ(B, glm::ivec4(1.0f, 2.0f, 3.0f, 4.0f));
		Error += GLM_CHECK_EQ(A, glm::ivec4(0.0f, 1.0f, 2.0f, 3.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = ++A;
		Error += GLM_CHECK_EQ(B, glm::ivec4(2.0f, 3.0f, 4.0f, 5.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A++;
		Error += GLM_CHECK_EQ(B, glm::ivec4(1.0f, 2.0f, 3.0f, 4.0f));
		Error += GLM_CHECK_EQ(A, glm::ivec4(2.0f, 3.0f, 4.0f, 5.0f));
	}

	return Error;
}

static int test_equal()
{
	int Error = 0;

	{
		glm::uvec4 A(1, 2, 3, 4);
		glm::uvec4 B(1, 2, 3, 4);
		Error += GLM_CHECK_EQ(A, B);
		Error += GLM_CHECK(!(A != B));
	}

	{
		glm::ivec4 A(1, 2, 3, 4);
		glm::ivec4 B(1, 2, 3, 4);
		Error += GLM_CHECK_EQ(A, B);
		Error += GLM_CHECK(!(A != B));
	}

	return Error;
}

static int test_size()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(sizeof(glm::vec4), sizeof(glm::lowp_vec4));
	Error += GLM_CHECK_EQ(sizeof(glm::vec4), sizeof(glm::mediump_vec4));
	Error += GLM_CHECK_EQ(sizeof(glm::vec4), sizeof(glm::highp_vec4));
	Error += GLM_CHECK_EQ(16, sizeof(glm::mediump_vec4));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec4), sizeof(glm::lowp_dvec4));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec4), sizeof(glm::mediump_dvec4));
	Error += GLM_CHECK_EQ(sizeof(glm::dvec4), sizeof(glm::highp_dvec4));
	Error += GLM_CHECK_EQ(32, sizeof(glm::highp_dvec4));
	Error += GLM_CHECK_EQ(glm::vec4().length(), 4);
	Error += GLM_CHECK_EQ(glm::dvec4().length(), 4);
	Error += GLM_CHECK_EQ(glm::vec4::length(), 4);
	Error += GLM_CHECK_EQ(glm::dvec4::length(), 4);

	return Error;
}

static int test_swizzle_partial()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	glm::vec4 const A(1, 2, 3, 4);

	{
		glm::vec4 B(A.xy, A.zw);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}
	{
		glm::vec4 B(A.xy, 3.0f, 4.0f);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}
	{
		glm::vec4 B(1.0f, A.yz, 4.0f);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}
	{
		glm::vec4 B(1.0f, 2.0f, A.zw);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}

	{
		glm::vec4 B(A.xyz, 4.0f);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}
	{
		glm::vec4 B(1.0f, A.yzw);
		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
	}
#	endif//GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION

	return Error;
}

static int test_swizzle()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	{
		glm::ivec4 A = glm::ivec4(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A.xyzw;
		glm::ivec4 C(A.xyzw);
		glm::ivec4 D(A.xyzw());
		glm::ivec4 E(A.x, A.yzw);
		glm::ivec4 F(A.x, A.yzw());
		glm::ivec4 G(A.xyz, A.w);
		glm::ivec4 H(A.xyz(), A.w);
		glm::ivec4 I(A.xy, A.zw);
		glm::ivec4 J(A.xy(), A.zw());
		glm::ivec4 K(A.x, A.y, A.zw);
		glm::ivec4 L(A.x, A.yz, A.w);
		glm::ivec4 M(A.xy, A.z, A.w);

		Error += GLM_CHECK(glm::all(glm::equal(A, B)));
		Error += GLM_CHECK(glm::all(glm::equal(A, C)));
		Error += GLM_CHECK(glm::all(glm::equal(A, D)));
		Error += GLM_CHECK(glm::all(glm::equal(A, E)));
		Error += GLM_CHECK(glm::all(glm::equal(A, F)));
		Error += GLM_CHECK(glm::all(glm::equal(A, G)));
		Error += GLM_CHECK(glm::all(glm::equal(A, H)));
		Error += GLM_CHECK(glm::all(glm::equal(A, I)));
		Error += GLM_CHECK(glm::all(glm::equal(A, J)));
		Error += GLM_CHECK(glm::all(glm::equal(A, K)));
		Error += GLM_CHECK(glm::all(glm::equal(A, L)));
		Error += GLM_CHECK(glm::all(glm::equal(A, M)));
	}
#	endif//GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
	{
		glm::vec4 A = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		glm::vec4 B = A.xyzw();
		glm::vec4 C(A.xyzw());
		glm::vec4 D(A.xyzw());
		glm::vec4 E(A.x, A.yzw());
		glm::vec4 F(A.x, A.yzw());
		glm::vec4 G(A.xyz(), A.w);
		glm::vec4 H(A.xyz(), A.w);
		glm::vec4 I(A.xy(), A.zw());
		glm::vec4 J(A.xy(), A.zw());
		glm::vec4 K(A.x, A.y, A.zw());
		glm::vec4 L(A.x, A.yz(), A.w);
		glm::vec4 M(A.xy(), A.z, A.w);

		Error += GLM_CHECK_NEAR(A, B, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, C, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, D, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, E, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, F, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, G, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, H, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, I, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, J, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, K, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, L, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(A, M, glm::epsilon<float>());
	}
#	endif//GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION

	return Error;
}

static int test_operator_increment()
{
	int Error = 0;

	glm::ivec4 v0(1);
	glm::ivec4 v1(v0);
	glm::ivec4 v2(v0);
	glm::ivec4 v3 = ++v1;
	glm::ivec4 v4 = v2++;

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

struct AoS
{
	glm::vec4 A;
	glm::vec3 B;
	glm::vec3 C;
	glm::vec2 D;
};

static int test_perf_AoS(std::size_t Size)
{
	int Error = 0;

	std::vector<AoS> In;
	std::vector<AoS> Out;
	In.resize(Size);
	Out.resize(Size);

	std::clock_t StartTime = std::clock();

	for(std::size_t i = 0; i < In.size(); ++i)
		Out[i] = In[i];

	std::clock_t EndTime = std::clock();

	glm_test::info("AoS: %d\n", static_cast<int>(EndTime - StartTime));

	return Error;
}

static int test_perf_SoA(std::size_t Size)
{
	int Error = 0;

	std::vector<glm::vec4> InA;
	std::vector<glm::vec3> InB;
	std::vector<glm::vec3> InC;
	std::vector<glm::vec2> InD;
	std::vector<glm::vec4> OutA;
	std::vector<glm::vec3> OutB;
	std::vector<glm::vec3> OutC;
	std::vector<glm::vec2> OutD;

	InA.resize(Size);
	InB.resize(Size);
	InC.resize(Size);
	InD.resize(Size);
	OutA.resize(Size);
	OutB.resize(Size);
	OutC.resize(Size);
	OutD.resize(Size);

	std::clock_t StartTime = std::clock();

	for(std::size_t i = 0; i < InA.size(); ++i)
	{
		OutA[i] = InA[i];
		OutB[i] = InB[i];
		OutC[i] = InC[i];
		OutD[i] = InD[i];
	}

	std::clock_t EndTime = std::clock();

	glm_test::info("SoA: %d\n", static_cast<int>(EndTime - StartTime));

	return Error;
}

namespace heap
{
	struct A
	{
		float f;
	};

	struct B : public A
	{
		float g;
		glm::vec4 v;
	};

	static int test()
	{
		int Error = 0;

		A* p = new B;
		p->f = 0.0f;
		delete p;

		// An aligned vec4 member is placed on a 16-byte boundary after the two floats.
		Error += sizeof(B) == (glm::detail::is_aligned<glm::defaultp>::value ? sizeof(glm::vec4) * 2 : sizeof(glm::vec4) + sizeof(float) * 2) ? 0 : 1;

		return Error;
	}
}//namespace heap

static int test_simd()
{
	int Error = 0;

	glm::vec4 const a(std::clock(), std::clock(), std::clock(), std::clock());
	glm::vec4 const b(std::clock(), std::clock(), std::clock(), std::clock());

	glm::vec4 const c(b * a);
	glm::vec4 const d(a + c);

	Error += GLM_CHECK(glm::all(glm::greaterThanEqual(d, glm::vec4(0))));

	return Error;
}

static int test_inheritance()
{
	struct my_vec4 : public glm::vec4
	{
		my_vec4()
			: glm::vec4(76.f, 75.f, 74.f, 73.f)
			, member(82)
		{}

		int member;
	};

	int Error = 0;

	my_vec4 v;

	Error += GLM_CHECK_EQ(v.member, 82);
	Error += GLM_CHECK_NEAR(v.x, 76.f, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(v.y, 75.f, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(v.z, 74.f, glm::epsilon<float>());
	Error += GLM_CHECK_NEAR(v.w, 73.f, glm::epsilon<float>());

	return Error;
}

/*
static int test_simd_gen()
{
	int Error = 0;

	int const C = static_cast<int>(std::clock());
	int const D = static_cast<int>(std::clock());

	glm::ivec4 const A(C);
	glm::ivec4 const B(D);

	Error += GLM_CHECK_NE(A, B);

	return Error;
}
*/
int main()
{
	// Suppress unused variable warnings
	(void)g1;
	(void)g2;
	(void)g3;

	int Error = 0;

	//Error += test_simd_gen();

/*
	{
		glm::ivec4 const a1(2);
		glm::ivec4 const b1 = a1 >> 1;

		__m128i const e1 = _mm_set1_epi32(2);
		__m128i const f1 = _mm_srli_epi32(e1, 1);

		glm::ivec4 const g1 = *reinterpret_cast<glm::ivec4 const* const>(&f1);

		glm::ivec4 const a2(-2);
		glm::ivec4 const b2 = a2 >> 1;

		__m128i const e2 = _mm_set1_epi32(-1);
		__m128i const f2 = _mm_srli_epi32(e2, 1);

		glm::ivec4 const g2 = *reinterpret_cast<glm::ivec4 const* const>(&f2);

		std::printf("GNI\n");
	}

	{
		glm::uvec4 const a1(2);
		glm::uvec4 const b1 = a1 >> 1u;

		__m128i const e1 = _mm_set1_epi32(2);
		__m128i const f1 = _mm_srli_epi32(e1, 1);

		glm::uvec4 const g1 = *reinterpret_cast<glm::uvec4 const* const>(&f1);

		glm::uvec4 const a2(-1);
		glm::uvec4 const b2 = a2 >> 1u;

		__m128i const e2 = _mm_set1_epi32(-1);
		__m128i const f2 = _mm_srli_epi32(e2, 1);

		glm::uvec4 const g2 = *reinterpret_cast<glm::uvec4 const* const>(&f2);

		std::printf("GNI\n");
	}
*/

#	ifdef NDEBUG
	std::size_t const Size(1000000);
#	else
	std::size_t const Size(1);
#	endif//NDEBUG

	Error += GLM_TEST(test_perf_AoS(Size));
	Error += GLM_TEST(test_perf_SoA(Size));

	Error += GLM_TEST(test_vec4_ctor());
	Error += GLM_TEST(test_bvec4_ctor());
	Error += GLM_TEST(test_size());
	Error += GLM_TEST(test_operators());
	Error += GLM_TEST(test_equal());
	Error += GLM_TEST(test_swizzle());
	Error += GLM_TEST(test_swizzle_partial());
	Error += GLM_TEST(test_simd());
	Error += GLM_TEST(test_operator_increment());
	Error += GLM_TEST(heap::test());
	Error += GLM_TEST(test_inheritance());

	return GLM_TEST_FINISH(Error);
}

