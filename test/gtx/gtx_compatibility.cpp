#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/compatibility.hpp>

// This file has divisions by zero to test isnan
#if GLM_COMPILER & GLM_COMPILER_VC
#	pragma warning(push)
#	pragma warning(disable : 4723)
#endif

static int test_isfinite()
{
	int Error(0);

	Error += GLM_CHECK(glm::isfinite(1.0f));
	Error += GLM_CHECK(glm::isfinite(1.0));
	Error += GLM_CHECK(glm::isfinite(-1.0f));
	Error += GLM_CHECK(glm::isfinite(-1.0));

	Error += GLM_CHECK(glm::all(glm::isfinite(glm::vec4(1.0f))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::dvec4(1.0))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::vec4(-1.0f))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::dvec4(-1.0))));

	Error += GLM_CHECK(glm::all(glm::isfinite(glm::quat(1.0f, 1.0f, 1.0f, 1.0f))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::dquat(1.0, 1.0, 1.0, 1.0))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::quat(-1.0f, -1.0f, -1.0f, -1.0f))));
	Error += GLM_CHECK(glm::all(glm::isfinite(glm::dquat(-1.0, -1.0, -1.0, -1.0))));

	return Error;
}

static int test_isfinite_specials()
{
	GLM_TEST_REQUIRE_IEEE_SPECIALS();

	int Error(0);

	float Zero_f = 0.0;
	double Zero_d = 0.0;

	Error += GLM_CHECK(!glm::isfinite(0.0f/Zero_f));
	Error += GLM_CHECK(!glm::isfinite(0.0/Zero_d));
	Error += GLM_CHECK(!glm::isfinite(1.0f/Zero_f));
	Error += GLM_CHECK(!glm::isfinite(1.0/Zero_d));
	Error += GLM_CHECK(!glm::isfinite(-1.0f/Zero_f));
	Error += GLM_CHECK(!glm::isfinite(-1.0/Zero_d));

	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::vec4(0.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dvec4(0.0/Zero_d))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::vec4(1.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dvec4(1.0/Zero_d))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::vec4(-1.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dvec4(-1.0/Zero_d))));

	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::quat(0.0f/Zero_f, 0.0f/Zero_f, 0.0f/Zero_f, 0.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dquat(0.0/Zero_d, 0.0/Zero_d, 0.0/Zero_d, 0.0/Zero_d))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::quat(1.0f/Zero_f, 1.0f/Zero_f, 1.0f/Zero_f, 1.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dquat(1.0/Zero_d, 1.0/Zero_d, 1.0/Zero_d, 1.0/Zero_d))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::quat(-1.0f/Zero_f, -1.0f/Zero_f, -1.0f/Zero_f, -1.0f/Zero_f))));
	Error += GLM_CHECK(!glm::any(glm::isfinite(glm::dquat(-1.0/Zero_d, -1.0/Zero_d, -1.0/Zero_d, -1.0/Zero_d))));

	return Error;
}

int main()
{
	int Error(0);

	Error += GLM_TEST(test_isfinite());
	Error += GLM_TEST(test_isfinite_specials());

	return GLM_TEST_FINISH(Error);
}

#if(GLM_COMPILER & GLM_COMPILER_VC)
#	pragma warning(pop)
#endif
