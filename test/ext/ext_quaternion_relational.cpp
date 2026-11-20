#include "glm_test.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_float_precision.hpp>
#include <glm/ext/quaternion_double.hpp>
#include <glm/ext/quaternion_double_precision.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float3_precision.hpp>
#include <glm/ext/vector_double3.hpp>
#include <glm/ext/vector_double3_precision.hpp>

// Helper function to create quaternions consistently regardless of layout
template <typename T>
static glm::qua<T, glm::defaultp> make_quat(T w, T x, T y, T z) {
	return glm::qua<T, glm::defaultp>::wxyz(w, x, y, z);
}

template <typename quaType>
static int test_equal()
{
	typedef typename quaType::value_type T;
	T const Epsilon = static_cast<T>(0.0001);

	int Error = 0;

	// Test 1: Equal quaternions
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, 0, 0, 0);

		Error += GLM_CHECK_NEAR(Q, P, Epsilon); // Identical quaternions should be equal
	}

	// Test 2: Nearly equal quaternions (within epsilon)
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon / T(2), Epsilon / T(2), Epsilon / T(2));

		Error += GLM_CHECK_NEAR(Q, P, Epsilon); // Quaternions within epsilon should be equal
	}

	// Test 3: Not equal quaternions (outside epsilon)
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon * T(2), 0, 0);

		Error += glm_test::context(GLM_CHECK(!glm::all(glm::equal(Q, P, Epsilon))), "Q", Q, "P", P); // Quaternions outside epsilon should not be equal
	}

	// Test 4: Equal non-normalized quaternions
	{
		quaType const Q = make_quat<T>(2, 3, 4, 5);
		quaType const P = make_quat<T>(2, 3, 4, 5);

		Error += GLM_CHECK_NEAR(Q, P, Epsilon); // Identical non-normalized quaternions should be equal
	}

	return Error;
}

template <typename quaType>
static int test_notEqual()
{
	typedef typename quaType::value_type T;
	T const Epsilon = static_cast<T>(0.0001);

	int Error = 0;

	// Test 1: Equal quaternions should not be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, 0, 0, 0);

		Error += glm_test::context(GLM_CHECK(!glm::any(glm::notEqual(Q, P, Epsilon))), "Q", Q, "P", P); // Identical quaternions should not be notEqual
	}

	// Test 2: Nearly equal quaternions (within epsilon) should not be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon / T(2), Epsilon / T(2), Epsilon / T(2));

		Error += glm_test::context(GLM_CHECK(!glm::any(glm::notEqual(Q, P, Epsilon))), "Q", Q, "P", P); // Quaternions within epsilon should not be notEqual
	}

	// Test 3: Different quaternions should be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon * T(2), 0, 0);

		Error += glm_test::context(GLM_CHECK(glm::any(glm::notEqual(Q, P, Epsilon))), "Q", Q, "P", P); // Different quaternions should be notEqual
	}

	// Test 4: Very different quaternions should be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(0, 1, 0, 0);

		Error += glm_test::context(GLM_CHECK(glm::any(glm::notEqual(Q, P, Epsilon))), "Q", Q, "P", P); // Very different quaternions should be notEqual
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_equal<glm::quat>());
	Error += GLM_TEST(test_equal<glm::lowp_quat>());
	Error += GLM_TEST(test_equal<glm::mediump_quat>());
	Error += GLM_TEST(test_equal<glm::highp_quat>());
	Error += GLM_TEST(test_notEqual<glm::quat>());
	Error += GLM_TEST(test_notEqual<glm::lowp_quat>());
	Error += GLM_TEST(test_notEqual<glm::mediump_quat>());
	Error += GLM_TEST(test_notEqual<glm::highp_quat>());

	return GLM_TEST_FINISH(Error);
}
