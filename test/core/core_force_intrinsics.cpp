#ifndef GLM_FORCE_INTRINSICS
#	define GLM_FORCE_INTRINSICS
#endif//GLM_FORCE_INTRINSICS
#define GLM_FORCE_SWIZZLE
#include "glm_test.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/vector_relational.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <ctime>
#include <vector>

static int test_vec4_ctor()
{
	int Error = 0;

	{
		glm::ivec4 A(1, 2, 3, 4);
		glm::ivec4 B(A);
		Error += GLM_CHECK(glm::all(glm::equal(A, B)));
	}

	{
	//	Error += std::is_trivially_default_constructible<glm::vec4>::value ? 0 : 1;
	//	Error += std::is_trivially_copy_assignable<glm::vec4>::value ? 0 : 1;
		Error += GLM_CHECK(std::is_trivially_copyable<glm::vec4>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::dvec4>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::ivec4>::value);
		Error += GLM_CHECK(std::is_trivially_copyable<glm::uvec4>::value);

		Error += GLM_CHECK(std::is_copy_constructible<glm::vec4>::value);
	}

	{
		glm::vec4 a{ 0, 1, 2, 3 };
		Error += GLM_CHECK(glm::all(glm::equal(a, glm::vec4(0, 1, 2, 3), glm::epsilon<float>())));

		std::vector<glm::vec4> v = {
			{0, 1, 2, 3},
			{4, 5, 6, 7},
			{8, 9, 0, 1}};
		Error += GLM_CHECK(glm::all(glm::equal(v[0], glm::vec4(0, 1, 2, 3), glm::epsilon<float>())));
		Error += GLM_CHECK(glm::all(glm::equal(v[1], glm::vec4(4, 5, 6, 7), glm::epsilon<float>())));
		Error += GLM_CHECK(glm::all(glm::equal(v[2], glm::vec4(8, 9, 0, 1), glm::epsilon<float>())));
	}

	{
		glm::dvec4 a{ 0, 1, 2, 3 };
		Error += GLM_CHECK(glm::all(glm::equal(a, glm::dvec4(0, 1, 2, 3), glm::epsilon<double>())));

		std::vector<glm::dvec4> v = {
			{0, 1, 2, 3},
			{4, 5, 6, 7},
			{8, 9, 0, 1}};
		Error += GLM_CHECK(glm::all(glm::equal(v[0], glm::dvec4(0, 1, 2, 3), glm::epsilon<double>())));
		Error += GLM_CHECK(glm::all(glm::equal(v[1], glm::dvec4(4, 5, 6, 7), glm::epsilon<double>())));
		Error += GLM_CHECK(glm::all(glm::equal(v[2], glm::dvec4(8, 9, 0, 1), glm::epsilon<double>())));
	}

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	{
		glm::ivec4 A = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
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
#	endif

#	if GLM_CONFIG_SWIZZLE
	{
		glm::ivec4 A = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A.xyzw();
		glm::ivec4 C(A.xyzw());
		glm::ivec4 D(A.xyzw());
		glm::ivec4 E(A.x, A.yzw());
		glm::ivec4 F(A.x, A.yzw());
		glm::ivec4 G(A.xyz(), A.w);
		glm::ivec4 H(A.xyz(), A.w);
		glm::ivec4 I(A.xy(), A.zw());
		glm::ivec4 J(A.xy(), A.zw());
		glm::ivec4 K(A.x, A.y, A.zw());
		glm::ivec4 L(A.x, A.yz(), A.w);
		glm::ivec4 M(A.xy(), A.z, A.w);

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
#	endif//GLM_CONFIG_SWIZZLE

	{
		glm::ivec4 A(1);
		glm::ivec4 B(1, 1, 1, 1);

		Error += GLM_CHECK(A == B);
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
			Error += GLM_CHECK(Tests[i] == glm::ivec4(1, 2, 3, 4));
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

	Error += GLM_CHECK((D == A));
	Error += GLM_CHECK((E == C));
	Error += GLM_CHECK((F == A));

	bool G = A == C;
	bool H = A != C;

	Error += GLM_CHECK(!G);
	Error += GLM_CHECK(H);

	return Error;
}

static int test_vec4_operators()
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
		Error += GLM_CHECK(glm::all(glm::equal(C, glm::vec4(5, 7, 9, 11), 0.001f)));

		glm::vec4 D = B - A;
		Error += GLM_CHECK(glm::all(glm::equal(D, glm::vec4(3, 3, 3, 3), 0.001f)));

		glm::vec4 E = A * B;
		Error += GLM_CHECK(glm::all(glm::equal(E, glm::vec4(4, 10, 18, 28), 0.001f)));

		glm::vec4 F = B / A;
		Error += GLM_CHECK(glm::all(glm::equal(F, glm::vec4(4, 2.5, 2, 7.0f / 4.0f), 0.001f)));

		glm::vec4 G = A + 1.0f;
		Error += GLM_CHECK(glm::all(glm::equal(G, glm::vec4(2, 3, 4, 5), 0.001f)));

		glm::vec4 H = B - 1.0f;
		Error += GLM_CHECK(glm::all(glm::equal(H, glm::vec4(3, 4, 5, 6), 0.001f)));

		glm::vec4 I = A * 2.0f;
		Error += GLM_CHECK(glm::all(glm::equal(I, glm::vec4(2, 4, 6, 8), 0.001f)));

		glm::vec4 J = B / 2.0f;
		Error += GLM_CHECK(glm::all(glm::equal(J, glm::vec4(2, 2.5, 3, 3.5), 0.001f)));

		glm::vec4 K = 1.0f + A;
		Error += GLM_CHECK(glm::all(glm::equal(K, glm::vec4(2, 3, 4, 5), 0.001f)));

		glm::vec4 L = 1.0f - B;
		Error += GLM_CHECK(glm::all(glm::equal(L, glm::vec4(-3, -4, -5, -6), 0.001f)));

		glm::vec4 M = 2.0f * A;
		Error += GLM_CHECK(glm::all(glm::equal(M, glm::vec4(2, 4, 6, 8), 0.001f)));

		glm::vec4 const N = 2.0f / B;
		Error += GLM_CHECK(glm::all(glm::equal(N, glm::vec4(0.5, 2.0 / 5.0, 2.0 / 6.0, 2.0 / 7.0), 0.0001f)));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		A += B;
		Error += GLM_CHECK(A == glm::ivec4(5, 7, 9, 11));

		A += 1;
		Error += GLM_CHECK(A == glm::ivec4(6, 8, 10, 12));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		B -= A;
		Error += GLM_CHECK(B == glm::ivec4(3, 3, 3, 3));

		B -= 1;
		Error += GLM_CHECK(B == glm::ivec4(2, 2, 2, 2));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 5.0f, 6.0f, 7.0f);

		A *= B;
		Error += GLM_CHECK(A == glm::ivec4(4, 10, 18, 28));

		A *= 2;
		Error += GLM_CHECK(A == glm::ivec4(8, 20, 36, 56));
	}
	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B(4.0f, 4.0f, 6.0f, 8.0f);

		B /= A;
		Error += GLM_CHECK(B == glm::ivec4(4, 2, 2, 2));

		B /= 2;
		Error += GLM_CHECK(B == glm::ivec4(2, 1, 1, 1));
	}
	{
		glm::ivec4 B(2);

		B /= B.y;
		Error += GLM_CHECK(B == glm::ivec4(1));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = -A;
		Error += GLM_CHECK(B == glm::ivec4(-1.0f, -2.0f, -3.0f, -4.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = --A;
		Error += GLM_CHECK(B == glm::ivec4(0.0f, 1.0f, 2.0f, 3.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A--;
		Error += GLM_CHECK(B == glm::ivec4(1.0f, 2.0f, 3.0f, 4.0f));
		Error += GLM_CHECK(A == glm::ivec4(0.0f, 1.0f, 2.0f, 3.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = ++A;
		Error += GLM_CHECK(B == glm::ivec4(2.0f, 3.0f, 4.0f, 5.0f));
	}

	{
		glm::ivec4 A(1.0f, 2.0f, 3.0f, 4.0f);
		glm::ivec4 B = A++;
		Error += GLM_CHECK(B == glm::ivec4(1.0f, 2.0f, 3.0f, 4.0f));
		Error += GLM_CHECK(A == glm::ivec4(2.0f, 3.0f, 4.0f, 5.0f));
	}

	return Error;
}

static int test_vec4_equal()
{
	int Error = 0;

	{
		glm::uvec4 A(1, 2, 3, 4);
		Error += GLM_CHECK((A == glm::uvec4(1, 2, 3, 4)));
		Error += GLM_CHECK(!((A != glm::uvec4(1, 2, 3, 4))));
	}

	{
		glm::ivec4 A(1, 2, 3, 4);
		Error += GLM_CHECK((A == glm::ivec4(1, 2, 3, 4)));
		Error += GLM_CHECK(!((A != glm::ivec4(1, 2, 3, 4))));
	}

	return Error;
}

static int test_vec4_size()
{
	int Error = 0;

	Error += GLM_CHECK(sizeof(glm::vec4) == sizeof(glm::lowp_vec4));
	Error += GLM_CHECK(sizeof(glm::vec4) == sizeof(glm::mediump_vec4));
	Error += GLM_CHECK(sizeof(glm::vec4) == sizeof(glm::highp_vec4));
	Error += GLM_CHECK(16 == sizeof(glm::mediump_vec4));
	Error += GLM_CHECK(sizeof(glm::dvec4) == sizeof(glm::lowp_dvec4));
	Error += GLM_CHECK(sizeof(glm::dvec4) == sizeof(glm::mediump_dvec4));
	Error += GLM_CHECK(sizeof(glm::dvec4) == sizeof(glm::highp_dvec4));
	Error += GLM_CHECK(32 == sizeof(glm::highp_dvec4));
	Error += GLM_CHECK(glm::vec4().length() == 4);
	Error += GLM_CHECK(glm::dvec4().length() == 4);

	return Error;
}

static int test_vec4_swizzle_partial()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR

	glm::ivec4 A(1, 2, 3, 4);

	{
		glm::ivec4 B(A.xy, A.zw);
		Error += GLM_CHECK(A == B);
	}
	{
		glm::ivec4 B(A.xy, 3, 4);
		Error += GLM_CHECK(A == B);
	}
	{
		glm::ivec4 B(1, A.yz, 4);
		Error += GLM_CHECK(A == B);
	}
	{
		glm::ivec4 B(1, 2, A.zw);
		Error += GLM_CHECK(A == B);
	}

	{
		glm::ivec4 B(A.xyz, 4);
		Error += GLM_CHECK(A == B);
	}
	{
		glm::ivec4 B(1, A.yzw);
		Error += GLM_CHECK(A == B);
	}
#	endif

	return Error;
}

static int test_operator_increment()
{
	int Error(0);

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

	Error += GLM_CHECK(i0 == i4);
	Error += GLM_CHECK(i1 == i2);
	Error += GLM_CHECK(i1 == i3);

	return Error;
}

static int test_vec4_simd()
{
	int Error = 0;

	glm::vec4 const a(std::clock(), std::clock(), std::clock(), std::clock());
	glm::vec4 const b(std::clock(), std::clock(), std::clock(), std::clock());

	glm::vec4 const c(b * a);
	glm::vec4 const d(a + c);

	Error += GLM_CHECK(glm::all(glm::greaterThanEqual(d, glm::vec4(0))));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_vec4_ctor());
	Error += GLM_TEST(test_bvec4_ctor());
	Error += GLM_TEST(test_vec4_size());
	Error += GLM_TEST(test_vec4_operators());
	Error += GLM_TEST(test_vec4_equal());
	Error += GLM_TEST(test_vec4_swizzle_partial());
	Error += GLM_TEST(test_vec4_simd());
	Error += GLM_TEST(test_operator_increment());

	return GLM_TEST_FINISH(Error);
}

