#include "glm_test.hpp"
#include <glm/ext/quaternion_exponential.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_float_precision.hpp>
#include <glm/ext/quaternion_double.hpp>
#include <glm/ext/quaternion_double_precision.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float3_precision.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/ext/vector_double3_precision.hpp>
#include <glm/ext/scalar_constants.hpp>

template <typename quaType, typename vecType>
static int test_log()
{
	typedef typename quaType::value_type T;

	T const Epsilon = static_cast<T>(0.001f);

	int Error = 0;

	quaType const Q(vecType(1, 0, 0), vecType(0, 1, 0));
	quaType const P = glm::log(Q);

	// Q and log(Q) should be different
	Error += GLM_CHECK(!glm::all(glm::equal(Q, P, Epsilon)));

	// exp(log(Q)) should equal Q
	quaType const R = glm::exp(P);
	Error += GLM_CHECK_NEAR(R, Q, Epsilon);

	// Log of identity should be approximately (0,0,0,0)
	quaType const Identity(static_cast<T>(1), vecType(0, 0, 0));
	quaType const LogIdentity = glm::log(Identity);
	quaType const ZeroQuat(static_cast<T>(0), vecType(0, 0, 0));
	Error += GLM_CHECK_NEAR(LogIdentity, ZeroQuat, Epsilon);

	return Error;
}

template <typename quaType, typename vecType>
static int test_pow()
{
	typedef typename quaType::value_type T;

	T const Epsilon = static_cast<T>(0.001f);

	int Error = 0;

	quaType const Q(vecType(1, 0, 0), vecType(0, 1, 0));

	// Q^1 == Q
	{
		quaType const P = glm::pow(Q, static_cast<T>(1.0f));
		Error += GLM_CHECK_NEAR(P, Q, Epsilon);
	}

	// Q^2 == Q*Q, and sqrt(Q^2) == Q
	{
		quaType const P = glm::pow(Q, static_cast<T>(2.0f));
		Error += GLM_CHECK_NEAR(P, Q * Q, Epsilon);
		Error += GLM_CHECK_NEAR(glm::sqrt(P), Q, Epsilon);
	}

	// Q^0.5 == sqrt(Q)
	{
		quaType const P = glm::pow(Q, static_cast<T>(0.5f));
		Error += GLM_CHECK_NEAR(P, glm::sqrt(Q), Epsilon);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_log<glm::quat, glm::vec3>());
	Error += GLM_TEST(test_log<glm::lowp_quat, glm::lowp_vec3>());
	Error += GLM_TEST(test_log<glm::mediump_quat, glm::mediump_vec3>());
	Error += GLM_TEST(test_log<glm::highp_quat, glm::highp_vec3>());

	Error += GLM_TEST(test_log<glm::dquat, glm::dvec3>());
	Error += GLM_TEST(test_log<glm::lowp_dquat, glm::lowp_dvec3>());
	Error += GLM_TEST(test_log<glm::mediump_dquat, glm::mediump_dvec3>());
	Error += GLM_TEST(test_log<glm::highp_dquat, glm::highp_dvec3>());

	Error += GLM_TEST(test_pow<glm::quat, glm::vec3>());
	Error += GLM_TEST(test_pow<glm::lowp_quat, glm::lowp_vec3>());
	Error += GLM_TEST(test_pow<glm::mediump_quat, glm::mediump_vec3>());
	Error += GLM_TEST(test_pow<glm::highp_quat, glm::highp_vec3>());

	Error += GLM_TEST(test_pow<glm::dquat, glm::dvec3>());
	Error += GLM_TEST(test_pow<glm::lowp_dquat, glm::lowp_dvec3>());
	Error += GLM_TEST(test_pow<glm::mediump_dquat, glm::mediump_dvec3>());
	Error += GLM_TEST(test_pow<glm::highp_dquat, glm::highp_dvec3>());

	return GLM_TEST_FINISH(Error);
}
