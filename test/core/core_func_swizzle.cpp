#define GLM_FORCE_SWIZZLE
#include "glm_test.hpp"
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/glm.hpp>

static int test_ivec2_swizzle()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
	{
		glm::ivec2 A(1, 2);
		glm::ivec2 B = A.yx();
		glm::ivec2 C = B.yx();

		Error += GLM_CHECK_NE(A, B);
		Error += GLM_CHECK_EQ(A, C);
	}
#	endif//GLM_CONFIG_SWIZZLE

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	{
		glm::ivec2 A(1, 2);
		glm::ivec2 B = A.yx;
		glm::ivec2 C = A.yx;

		Error += GLM_CHECK_NE(A, B);
		Error += GLM_CHECK_EQ(B, C);

		B.xy = B.yx;
		C.xy = C.yx;

		Error += GLM_CHECK_EQ(B, C);

		glm::ivec2 D(0, 0);
		D.yx = A.xy;
		Error += GLM_CHECK_EQ(A.yx(), D);

		glm::ivec2 E = A.yx;
		Error += GLM_CHECK_EQ(E, D);
	}
#	endif//GLM_CONFIG_SWIZZLE

	return Error;
}

static int test_ivec3_swizzle()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
	{
		glm::ivec3 A(1, 2, 3);
		glm::ivec3 B = A.zyx();
		glm::ivec3 C = B.zyx();

		Error += GLM_CHECK_NE(A, B);
		Error += GLM_CHECK_EQ(A, C);
	}
#	endif

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	{
		glm::ivec3 const A(1, 2, 3);
		glm::ivec2 B = A.yx;
		glm::ivec2 C = A.yx;

		Error += GLM_CHECK_EQ(A.yx(), B);
		Error += GLM_CHECK_EQ(B, C);

		B.xy = B.yx;
		C.xy = C.yx;

		Error += GLM_CHECK_EQ(B, C);

		glm::ivec2 D(0, 0);
		D.yx = A.xy;

		Error += GLM_CHECK_EQ(A.yx(), D);

		glm::ivec2 E(0, 0);
		E.xy = A.xy();

		Error += GLM_CHECK_EQ(E, A.xy());
		Error += GLM_CHECK_EQ(E.xy(), A.xy());

		glm::ivec3 const F = A.xxx + A.xxx;
		Error += GLM_CHECK_EQ(F, glm::ivec3(2));

		glm::ivec3 const G = A.xxx - A.xxx;
		Error += GLM_CHECK_EQ(G, glm::ivec3(0));

		glm::ivec3 const H = A.xxx * A.xxx;
		Error += GLM_CHECK_EQ(H, glm::ivec3(1));

		glm::ivec3 const I = A.xxx / A.xxx;
		Error += GLM_CHECK_EQ(I, glm::ivec3(1));

		glm::ivec3 J(1, 2, 3);
		J.xyz += glm::ivec3(1);
		Error += GLM_CHECK_EQ(J, glm::ivec3(2, 3, 4));

		glm::ivec3 K(1, 2, 3);
		K.xyz += A.xyz;
		Error += GLM_CHECK_EQ(K, glm::ivec3(2, 4, 6));
	}
#	endif

	return Error;
}

static int test_ivec4_swizzle()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
	{
		glm::ivec4 A(1, 2, 3, 4);
		glm::ivec4 B = A.wzyx();
		glm::ivec4 C = B.wzyx();

		Error += GLM_CHECK_NE(A, B);
		Error += GLM_CHECK_EQ(A, C);
	}
#	endif

	return Error;
}

static int test_vec4_swizzle()
{
	int Error = 0;

#	if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR || GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_FUNCTION
	{
		glm::vec4 A(1, 2, 3, 4);
		glm::vec4 B = A.wzyx();
		glm::vec4 C = B.wzyx();

		Error += GLM_CHECK(glm::any(glm::notEqual(A, B, 0.0001f)));
		Error += GLM_CHECK_NEAR(A, C, 0.0001f);

		float D = glm::dot(C.wzyx(), C.xyzw());
		Error += GLM_CHECK_NEAR(D, 20.f, 0.001f);
	}
#	endif

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_ivec2_swizzle());
	Error += GLM_TEST(test_ivec3_swizzle());
	Error += GLM_TEST(test_ivec4_swizzle());
	Error += GLM_TEST(test_vec4_swizzle());

	return GLM_TEST_FINISH(Error);
}



