#define GLM_ENABLE_EXPERIMENTAL

#include "glm_test.hpp"
#include <glm/gtx/extended_min_max.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>

// This file has divisions by zero to test isnan
#if GLM_COMPILER & GLM_COMPILER_VC
#	pragma warning(disable : 4723)
#endif

namespace fmin_
{
	static int test()
	{
		int Error = 0;

		glm::vec2 B0 = glm::fmin(glm::vec2(1), glm::vec2(1));
		glm::vec2 B1 = glm::fmin(glm::vec2(1), 1.0f);
		bool B2 = glm::all(glm::equal(B0, B1, glm::epsilon<float>()));
		Error += GLM_CHECK(B2);

		glm::vec3 C0 = glm::fmin(glm::vec3(1), glm::vec3(1));
		glm::vec3 C1 = glm::fmin(glm::vec3(1), 1.0f);
		bool C2 = glm::all(glm::equal(C0, C1, glm::epsilon<float>()));
		Error += GLM_CHECK(C2);

		glm::vec4 D0 = glm::fmin(glm::vec4(1), glm::vec4(1));
		glm::vec4 D1 = glm::fmin(glm::vec4(1), 1.0f);
		bool D2 = glm::all(glm::equal(D0, D1, glm::epsilon<float>()));
		Error += GLM_CHECK(D2);

		return Error;
	}

	static int test_nan()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();

		int Error = 0;

		float Zero_f = 0.0f;
		glm::vec1 A0 = glm::fmin(glm::vec1(1), glm::vec1(Zero_f / 0.0f));
		Error += GLM_CHECK_NEAR(A0.x, 1.0f, glm::epsilon<float>());

		glm::vec1 A1 = glm::fmin(glm::vec1(Zero_f / 0.0f), glm::vec1(1));
		Error += GLM_CHECK_NEAR(A1.x, 1.0f, glm::epsilon<float>());

		return Error;
	}
}//namespace fmin_

namespace fmax_
{
	static int test()
	{
		int Error = 0;

		glm::vec2 B0 = glm::fmax(glm::vec2(1), glm::vec2(1));
		glm::vec2 B1 = glm::fmax(glm::vec2(1), 1.0f);
		bool B2 = glm::all(glm::equal(B0, B1, glm::epsilon<float>()));
		Error += GLM_CHECK(B2);

		glm::vec3 C0 = glm::fmax(glm::vec3(1), glm::vec3(1));
		glm::vec3 C1 = glm::fmax(glm::vec3(1), 1.0f);
		bool C2 = glm::all(glm::equal(C0, C1, glm::epsilon<float>()));
		Error += GLM_CHECK(C2);

		glm::vec4 D0 = glm::fmax(glm::vec4(1), glm::vec4(1));
		glm::vec4 D1 = glm::fmax(glm::vec4(1), 1.0f);
		bool D2 = glm::all(glm::equal(D0, D1, glm::epsilon<float>()));
		Error += GLM_CHECK(D2);

		return Error;
	}

	static int test_nan()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();

		int Error = 0;

		float Zero_f = 0.0f;
		glm::vec1 A0 = glm::fmax(glm::vec1(1), glm::vec1(Zero_f / 0.0f));
		Error += GLM_CHECK_NEAR(A0.x, 1.0f, glm::epsilon<float>());

		glm::vec1 A1 = glm::fmax(glm::vec1(Zero_f / 0.0f), glm::vec1(1));
		Error += GLM_CHECK_NEAR(A1.x, 1.0f, glm::epsilon<float>());

		return Error;
	}
}//namespace fmax_

namespace fclamp_
{
	static int test_nan()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();

		int Error = 0;

		float Zero_f = 0.0f;
		glm::vec1 A0 = glm::fclamp(glm::vec1(1), glm::vec1(Zero_f / 0.0f), glm::vec1(2.0f));
		Error += GLM_CHECK_NEAR(A0.x, 1.0f, glm::epsilon<float>());

		return Error;
	}
}//namespace fclamp_

int main()
{
	int Error = 0;

	Error += GLM_TEST(fmin_::test());
	Error += GLM_TEST(fmin_::test_nan());
	Error += GLM_TEST(fmax_::test());
	Error += GLM_TEST(fmax_::test_nan());
	Error += GLM_TEST(fclamp_::test_nan());

	return GLM_TEST_FINISH(Error);
}
