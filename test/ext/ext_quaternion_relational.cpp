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
#include <iostream>

// Helper function to create quaternions consistently regardless of layout
template <typename T>
static glm::qua<T, glm::defaultp> make_quat(T w, T x, T y, T z) {
	return glm::qua<T, glm::defaultp>::wxyz(w, x, y, z);
}

// Print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
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

		if (!glm::all(glm::equal(Q, P, Epsilon))) {
			std::cout << "Error: Identical quaternions should be equal" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 2: Nearly equal quaternions (within epsilon)
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon / T(2), Epsilon / T(2), Epsilon / T(2));

		if (!glm::all(glm::equal(Q, P, Epsilon))) {
			std::cout << "Error: Quaternions within epsilon should be equal" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 3: Not equal quaternions (outside epsilon)
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon * T(2), 0, 0);

		if (glm::all(glm::equal(Q, P, Epsilon))) {
			std::cout << "Error: Quaternions outside epsilon should not be equal" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 4: Equal non-normalized quaternions
	{
		quaType const Q = make_quat<T>(2, 3, 4, 5);
		quaType const P = make_quat<T>(2, 3, 4, 5);

		if (!glm::all(glm::equal(Q, P, Epsilon))) {
			std::cout << "Error: Identical non-normalized quaternions should be equal" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
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

		if (glm::any(glm::notEqual(Q, P, Epsilon))) {
			std::cout << "Error: Identical quaternions should not be notEqual" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 2: Nearly equal quaternions (within epsilon) should not be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon / T(2), Epsilon / T(2), Epsilon / T(2));

		if (glm::any(glm::notEqual(Q, P, Epsilon))) {
			std::cout << "Error: Quaternions within epsilon should not be notEqual" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 3: Different quaternions should be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(1, Epsilon * T(2), 0, 0);

		if (!glm::any(glm::notEqual(Q, P, Epsilon))) {
			std::cout << "Error: Different quaternions should be notEqual" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	// Test 4: Very different quaternions should be notEqual
	{
		quaType const Q = make_quat<T>(1, 0, 0, 0);
		quaType const P = make_quat<T>(0, 1, 0, 0);

		if (!glm::any(glm::notEqual(Q, P, Epsilon))) {
			std::cout << "Error: Very different quaternions should be notEqual" << std::endl;
			print_quat("Q", Q);
			print_quat("P", P);
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Relational Extension Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: " <<
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
		"WXYZ"
#else
		"XYZW"
#endif
		<< std::endl << std::endl;

	std::cout << "Testing equal function:" << std::endl;
	Error += test_equal<glm::quat>();
	Error += test_equal<glm::lowp_quat>();
	Error += test_equal<glm::mediump_quat>();
	Error += test_equal<glm::highp_quat>();

	std::cout << "\nTesting notEqual function:" << std::endl;
	Error += test_notEqual<glm::quat>();
	Error += test_notEqual<glm::lowp_quat>();
	Error += test_notEqual<glm::mediump_quat>();
	Error += test_notEqual<glm::highp_quat>();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
