#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/ext/matrix_relational.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform2.hpp>

static int test_reflect2D()
{
	int Error = 0;

	{
		const glm::mat3 m3(
			1, 0, 0,
			0, 1, 0,
			1, 2, 1
		);

		const glm::mat3 eam3(
			1, 0, 0,
			0, -1, 0,
			1, 2, 1
		);

		const glm::mat3 am3 = glm::reflect2D(
			m3,
			glm::vec2(0, 1),
			static_cast<glm::mat3::row_type::value_type>(0)
		);

		Error += GLM_CHECK_NEAR(eam3, am3, glm::epsilon<float>());
	}

	{
		const glm::mat3 m3(
			1, 0, 0,
			0, 1, 0,
			1, 2, 1
		);

		const glm::mat3 eam3(
			0, 1, 0,
			1, 0, 0,
			1, 2, 1
		);

		const glm::mat3 am3 = glm::reflect2D(
			m3,
			glm::vec2(-0.70710678, 0.70710678),
			static_cast<glm::mat3::row_type::value_type>(0)
		);

		Error += GLM_CHECK_NEAR(eam3, am3, glm::epsilon<float>());
	}
		
	return Error;
}

static int test_reflect3D()
{
	int Error = 0;

	{
		const glm::mat4 m4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		const glm::mat4 eam4(
			1, 0, 0, 0,
			0, -1, 0, 0,
			0, 0, 1, 0,
			0, -2, 0, 1
		);

		const glm::mat4 am4 = glm::reflect3D(
			m4,
			glm::vec3(0, 1, 0),
			static_cast<glm::mat4::row_type::value_type>(1)
		);

		Error += GLM_CHECK_NEAR(eam4, am4, glm::epsilon<float>());
	}

	{
		const glm::mat4 m4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		const glm::mat4 eam4(
			0, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		const glm::mat4 am4 = glm::reflect3D(
			m4,
			glm::vec3(-0.70710678, 0.70710678, 0.0),
			static_cast<glm::mat4::row_type::value_type>(0)
		);

		Error += GLM_CHECK_NEAR(eam4, am4, glm::epsilon<float>());
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_reflect2D());
	Error += GLM_TEST(test_reflect3D());

	return GLM_TEST_FINISH(Error);
}
