#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/wrap.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>

namespace clamp
{
	static int test()
	{
		int Error(0);

		float A = glm::clamp(0.5f);
		Error += GLM_CHECK(glm::equal(A, 0.5f, 0.00001f));

		float B = glm::clamp(0.0f);
		Error += GLM_CHECK(glm::equal(B, 0.0f, 0.00001f));

		float C = glm::clamp(1.0f);
		Error += GLM_CHECK(glm::equal(C, 1.0f, 0.00001f));

		float D = glm::clamp(-0.5f);
		Error += GLM_CHECK(glm::equal(D, 0.0f, 0.00001f));

		float E = glm::clamp(1.5f);
		Error += GLM_CHECK(glm::equal(E, 1.0f, 0.00001f));

		glm::vec2 K = glm::clamp(glm::vec2(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(K, glm::vec2(0.5f), glm::vec2(0.00001f))));

		glm::vec3 L = glm::clamp(glm::vec3(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(L, glm::vec3(0.5f), glm::vec3(0.00001f))));

		glm::vec4 M = glm::clamp(glm::vec4(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(M, glm::vec4(0.5f), glm::vec4(0.00001f))));

		glm::vec1 N = glm::clamp(glm::vec1(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(N, glm::vec1(0.5f), glm::vec1(0.00001f))));

		return Error;
	}
}//namespace clamp

namespace repeat
{
	static int test()
	{
		int Error(0);

		float A = glm::repeat(0.5f);
		Error += GLM_CHECK(glm::equal(A, 0.5f, 0.00001f));

		float B = glm::repeat(0.0f);
		Error += GLM_CHECK(glm::equal(B, 0.0f, 0.00001f));

		float C = glm::repeat(1.0f);
		Error += GLM_CHECK(glm::equal(C, 0.0f, 0.00001f));

		float D = glm::repeat(-0.5f);
		Error += GLM_CHECK(glm::equal(D, 0.5f, 0.00001f));

		float E = glm::repeat(1.5f);
		Error += GLM_CHECK(glm::equal(E, 0.5f, 0.00001f));

		float F = glm::repeat(0.9f);
		Error += GLM_CHECK(glm::equal(F, 0.9f, 0.00001f));

		glm::vec2 K = glm::repeat(glm::vec2(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(K, glm::vec2(0.5f), glm::vec2(0.00001f))));

		glm::vec3 L = glm::repeat(glm::vec3(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(L, glm::vec3(0.5f), glm::vec3(0.00001f))));

		glm::vec4 M = glm::repeat(glm::vec4(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(M, glm::vec4(0.5f), glm::vec4(0.00001f))));

		glm::vec1 N = glm::repeat(glm::vec1(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(N, glm::vec1(0.5f), glm::vec1(0.00001f))));

		return Error;
	}
}//namespace repeat

namespace mirrorClamp
{
	static int test()
	{
		int Error(0);

		float A = glm::mirrorClamp(0.5f);
		Error += GLM_CHECK(glm::equal(A, 0.5f, 0.00001f));

		float B = glm::mirrorClamp(0.0f);
		Error += GLM_CHECK(glm::equal(B, 0.0f, 0.00001f));

		float C = glm::mirrorClamp(1.1f);
		Error += GLM_CHECK(glm::equal(C, 0.1f, 0.00001f));

		float D = glm::mirrorClamp(-0.5f);
		Error += GLM_CHECK(glm::equal(D, 0.5f, 0.00001f));

		float E = glm::mirrorClamp(1.5f);
		Error += GLM_CHECK(glm::equal(E, 0.5f, 0.00001f));

		float F = glm::mirrorClamp(0.9f);
		Error += GLM_CHECK(glm::equal(F, 0.9f, 0.00001f));

		float G = glm::mirrorClamp(3.1f);
		Error += GLM_CHECK(glm::equal(G, 0.1f, 0.00001f));

		float H = glm::mirrorClamp(-3.1f);
		Error += GLM_CHECK(glm::equal(H, 0.1f, 0.00001f));

		float I = glm::mirrorClamp(-0.9f);
		Error += GLM_CHECK(glm::equal(I, 0.9f, 0.00001f));

		glm::vec2 K = glm::mirrorClamp(glm::vec2(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(K, glm::vec2(0.5f), glm::vec2(0.00001f))));

		glm::vec3 L = glm::mirrorClamp(glm::vec3(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(L, glm::vec3(0.5f), glm::vec3(0.00001f))));

		glm::vec4 M = glm::mirrorClamp(glm::vec4(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(M, glm::vec4(0.5f), glm::vec4(0.00001f))));

		glm::vec1 N = glm::mirrorClamp(glm::vec1(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(N, glm::vec1(0.5f), glm::vec1(0.00001f))));

		return Error;
	}
}//namespace mirrorClamp

namespace mirrorRepeat
{
	static int test()
	{
		int Error(0);

		float A = glm::mirrorRepeat(0.5f);
		Error += GLM_CHECK(glm::equal(A, 0.5f, 0.00001f));

		float B = glm::mirrorRepeat(0.0f);
		Error += GLM_CHECK(glm::equal(B, 0.0f, 0.00001f));

		float C = glm::mirrorRepeat(1.0f);
		Error += GLM_CHECK(glm::equal(C, 1.0f, 0.00001f));

		float D = glm::mirrorRepeat(-0.5f);
		Error += GLM_CHECK(glm::equal(D, 0.5f, 0.00001f));

		float E = glm::mirrorRepeat(1.5f);
		Error += GLM_CHECK(glm::equal(E, 0.5f, 0.00001f));

		float F = glm::mirrorRepeat(0.9f);
		Error += GLM_CHECK(glm::equal(F, 0.9f, 0.00001f));

		float G = glm::mirrorRepeat(3.0f);
		Error += GLM_CHECK(glm::equal(G, 1.0f, 0.00001f));

		float H = glm::mirrorRepeat(-3.0f);
		Error += GLM_CHECK(glm::equal(H, 1.0f, 0.00001f));

		float I = glm::mirrorRepeat(-1.0f);
		Error += GLM_CHECK(glm::equal(I, 1.0f, 0.00001f));

		glm::vec2 K = glm::mirrorRepeat(glm::vec2(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(K, glm::vec2(0.5f), glm::vec2(0.00001f))));

		glm::vec3 L = glm::mirrorRepeat(glm::vec3(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(L, glm::vec3(0.5f), glm::vec3(0.00001f))));

		glm::vec4 M = glm::mirrorRepeat(glm::vec4(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(M, glm::vec4(0.5f), glm::vec4(0.00001f))));

		glm::vec1 N = glm::mirrorRepeat(glm::vec1(0.5f));
		Error += GLM_CHECK(glm::all(glm::equal(N, glm::vec1(0.5f), glm::vec1(0.00001f))));

		return Error;
	}
}//namespace mirrorRepeat

int main()
{
	int Error(0);

	Error += GLM_TEST(clamp::test());
	Error += GLM_TEST(repeat::test());
	Error += GLM_TEST(mirrorClamp::test());
	Error += GLM_TEST(mirrorRepeat::test());

	return GLM_TEST_FINISH(Error);
}
