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
#include <iostream>

// Helper function to print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

template <typename quaType, typename vecType>
static int test_log()
{
	typedef typename quaType::value_type T;

	T const Epsilon = static_cast<T>(0.001f);

	int Error = 0;

	quaType const Q(vecType(1, 0, 0), vecType(0, 1, 0));
	quaType const P = glm::log(Q);

	// Q and log(Q) should be different
	if (glm::all(glm::equal(Q, P, Epsilon))) {
		std::cout << "Error: quaternion and its logarithm should not be equal" << std::endl;
		print_quat("Original", Q);
		print_quat("Log", P);
		Error++;
	}

	// exp(log(Q)) should equal Q
	quaType const R = glm::exp(P);
	if (!glm::all(glm::equal(Q, R, Epsilon))) {
		std::cout << "Error: exp(log(Q)) should equal Q" << std::endl;
		print_quat("Original", Q);
		print_quat("Log", P);
		print_quat("Exp(Log)", R);
		Error++;
	}

	// Test with identity quaternion
	quaType const Identity(static_cast<T>(1), vecType(0, 0, 0));
	quaType const LogIdentity = glm::log(Identity);

	// Log of identity should be approximately (0,0,0,0)
	vecType const ZeroVec(0, 0, 0);
	quaType const ZeroQuat(static_cast<T>(0), ZeroVec);

	if (!glm::all(glm::equal(LogIdentity, ZeroQuat, Epsilon))) {
		std::cout << "Error: log of identity quaternion should be zero quaternion" << std::endl;
		print_quat("Log(Identity)", LogIdentity);
		Error++;
	}

	return Error;
}

template <typename quaType, typename vecType>
static int test_pow()
{
	typedef typename quaType::value_type T;

	T const Epsilon = static_cast<T>(0.001f);

	int Error = 0;

	quaType const Q(vecType(1, 0, 0), vecType(0, 1, 0));

	// Test pow with exponent 1
	{
		T const One = static_cast<T>(1.0f);
		quaType const P = glm::pow(Q, One);

		if (!glm::all(glm::equal(Q, P, Epsilon))) {
			std::cout << "Error: Q^1 should equal Q" << std::endl;
			print_quat("Original", Q);
			print_quat("Q^1", P);
			Error++;
		}
	}

	// Test pow with exponent 2 and sqrt
	{
		T const Two = static_cast<T>(2.0f);
		quaType const P = glm::pow(Q, Two);
		quaType const R = Q * Q;

		if (!glm::all(glm::equal(P, R, Epsilon))) {
			std::cout << "Error: Q^2 should equal Q*Q" << std::endl;
			print_quat("Q^2", P);
			print_quat("Q*Q", R);
			Error++;
		}

		quaType const U = glm::sqrt(P);
		if (!glm::all(glm::equal(Q, U, Epsilon))) {
			std::cout << "Error: sqrt(Q^2) should equal Q" << std::endl;
			print_quat("Original", Q);
			print_quat("sqrt(Q^2)", U);
			Error++;
		}
	}

	// Additional test: fractional power
	{
		T const Half = static_cast<T>(0.5f);
		quaType const P = glm::pow(Q, Half);
		quaType const R = glm::sqrt(Q);

		if (!glm::all(glm::equal(P, R, Epsilon))) {
			std::cout << "Error: Q^0.5 should equal sqrt(Q)" << std::endl;
			print_quat("Q^0.5", P);
			print_quat("sqrt(Q)", R);
			Error++;
		}
	}

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Exponential Extension Tests ===" << std::endl;
	std::cout << "Quaternion memory layout: " <<
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
		"WXYZ"
#else
		"XYZW"
#endif
		<< std::endl;

	std::cout << "\nTesting log/exp functions:" << std::endl;
	Error += test_log<glm::quat, glm::vec3>();
	Error += test_log<glm::lowp_quat, glm::lowp_vec3>();
	Error += test_log<glm::mediump_quat, glm::mediump_vec3>();
	Error += test_log<glm::highp_quat, glm::highp_vec3>();

	Error += test_log<glm::dquat, glm::dvec3>();
	Error += test_log<glm::lowp_dquat, glm::lowp_dvec3>();
	Error += test_log<glm::mediump_dquat, glm::mediump_dvec3>();
	Error += test_log<glm::highp_dquat, glm::highp_dvec3>();

	std::cout << "\nTesting pow/sqrt functions:" << std::endl;
	Error += test_pow<glm::quat, glm::vec3>();
	Error += test_pow<glm::lowp_quat, glm::lowp_vec3>();
	Error += test_pow<glm::mediump_quat, glm::mediump_vec3>();
	Error += test_pow<glm::highp_quat, glm::highp_vec3>();

	Error += test_pow<glm::dquat, glm::dvec3>();
	Error += test_pow<glm::lowp_dquat, glm::lowp_dvec3>();
	Error += test_pow<glm::mediump_dquat, glm::mediump_dvec3>();
	Error += test_pow<glm::highp_dquat, glm::highp_dvec3>();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
