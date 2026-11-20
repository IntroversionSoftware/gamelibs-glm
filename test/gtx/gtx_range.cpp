#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/range.hpp>

static int test_vec()
{
	int Error = 0;

	{
		glm::ivec3 const v(1, 2, 3);

		int count = 0;
		glm::ivec3 Result(0);
		for(int x : v)
		{
			Result[count] = x;
			count++;
		}
		Error += GLM_CHECK_EQ(count, 3);
		Error += GLM_CHECK_EQ(v, Result);
	}

	{
		glm::ivec3 v(1, 2, 3);
		for(int& x : v)
			x = 0;
		Error += GLM_CHECK(glm::all(glm::equal(v, glm::ivec3(0))));
	}

	return Error;
}

static int test_mat()
{
	int Error = 0;

	// Flat iteration needs a packed matrix: an aligned mat4x3 pads each column to 4 components.
	{
		glm::packed_t<glm::mat4x3> m(1.0f);

		int count = 0;
		float Sum = 0.0f;
		for(float x : m)
		{
			count++;
			Sum += x;
		}
		Error += GLM_CHECK_EQ(count, 12);
		Error += GLM_CHECK_NEAR(Sum, 3.0f, 0.001f);
	}

	{
		glm::packed_t<glm::mat4x3> m(1.0f);

		for (float& x : m) { x = 0; }
		glm::vec4 v(1, 1, 1, 1);
		Error += GLM_CHECK_NEAR(glm::mat4x3(m) * v, glm::vec3(0, 0, 0), glm::epsilon<float>());
	}

	// A matrix of any qualifier can be read through a packed copy.
	{
		glm::mat4x3 const m(1.0f);

		int count = 0;
		float Sum = 0.0f;
		for(float x : glm::to_packed(m))
		{
			count++;
			Sum += x;
		}
		Error += GLM_CHECK_EQ(count, 12);
		Error += GLM_CHECK_NEAR(Sum, 3.0f, 0.001f);
	}

	return Error;
}

int main()
{
	int Error = 0;
	Error += GLM_TEST(test_vec());
	Error += GLM_TEST(test_mat());
	return GLM_TEST_FINISH(Error);
}

