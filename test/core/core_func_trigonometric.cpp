#define GLM_ENABLE_EXPERIMENTAL 1

#include "glm_test.hpp"
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <glm/vector_relational.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>

#include <cmath>
#include <limits>
#include <string>
#include <vector>


// Function to generate appropriate test angles based on function type
template<typename T>
std::vector<T> generate_test_angles(const std::string& func_name) {
	const T PI = static_cast<T>(3.14159265358979323846);

	// Common angles for most trig functions
	std::vector<T> common_angles = {
		-PI, -3 * PI / 4, -2 * PI / 3, -PI / 2, -PI / 3, -PI / 4, -PI / 6,
		0,
		PI / 6, PI / 4, PI / 3, PI / 2, 2 * PI / 3, 3 * PI / 4, PI,
		5 * PI / 4, 4 * PI / 3, 3 * PI / 2, 5 * PI / 3, 7 * PI / 4, 2 * PI
	};

	// Add function-specific test values
	if (func_name == "asin" || func_name == "acos") {
		// For asin/acos, input must be in [-1, 1]
		return {
			static_cast<T>(-1.0), static_cast<T>(-0.8), static_cast<T>(-0.6),
			static_cast<T>(-0.4), static_cast<T>(-0.2), static_cast<T>(0.0),
			static_cast<T>(0.2), static_cast<T>(0.4), static_cast<T>(0.6),
			static_cast<T>(0.8), static_cast<T>(1.0)
		};
	}
	else if (func_name == "tan" || func_name == "sec") {
		// Remove values near asymptotes for tan
		std::vector<T> tan_angles;
		for (const auto& angle : common_angles) {
			T mod_angle = std::fmod(std::abs(angle), PI);
			// Avoid angles too close to (n+0.5)*PI
			if (std::abs(mod_angle - PI / 2) > static_cast<T>(0.1)) {
				tan_angles.push_back(angle);
			}
		}
		return tan_angles;
	}
	else if (func_name == "acosh") {
		// For acosh, input must be >= 1
		return {
			static_cast<T>(1.0), static_cast<T>(1.2), static_cast<T>(1.5),
			static_cast<T>(2.0), static_cast<T>(3.0), static_cast<T>(5.0),
			static_cast<T>(10.0)
		};
	}
	else if (func_name == "atanh") {
		// For atanh, input must be in (-1, 1)
		return {
			static_cast<T>(-0.99), static_cast<T>(-0.8), static_cast<T>(-0.6),
			static_cast<T>(-0.4), static_cast<T>(-0.2), static_cast<T>(0.0),
			static_cast<T>(0.2), static_cast<T>(0.4), static_cast<T>(0.6),
			static_cast<T>(0.8), static_cast<T>(0.99)
		};
	}

	// For other functions, use common angles
	return common_angles;
}

// Structure to define function-specific test characteristics
template<typename T>
struct FunctionTestInfo {
	// Special edge cases to test
	std::vector<T> edge_cases;

	// Expected results for edge cases (must match size of edge_cases)
	std::vector<T> edge_case_results;

	// Function to check if an input is valid (return true if valid)
	std::function<bool(T)> validate_input;

	// Function name for logging
	std::string name;
};

// Create test info for different trigonometric functions
template<typename T>
FunctionTestInfo<T> get_function_test_info(const std::string& func_name) {
	FunctionTestInfo<T> info;
	info.name = func_name;

	// Default input validator (all inputs valid)
	info.validate_input = [](T) { return true; };

	if (func_name == "sin") {
		info.edge_cases = { static_cast<T>(0.0), static_cast<T>(3.14159265358979323846) };
		info.edge_case_results = { static_cast<T>(0.0), static_cast<T>(0.0) }; // sin(0)=0, sin(π)=0
	}
	else if (func_name == "cos") {
		info.edge_cases = { static_cast<T>(0.0), static_cast<T>(3.14159265358979323846 / 2) };
		info.edge_case_results = { static_cast<T>(1.0), static_cast<T>(0.0) }; // cos(0)=1, cos(π/2)=0
	}
	else if (func_name == "tan") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // tan(0)=0

		// Avoid values near asymptotes
		info.validate_input = [](T x) {
			const T PI = static_cast<T>(3.14159265358979323846);
			T mod_x = std::fmod(std::abs(x), PI);
			return std::abs(mod_x - PI / 2) > static_cast<T>(0.1);
			};
	}
	else if (func_name == "asin") {
		info.edge_cases = { static_cast<T>(-1.0), static_cast<T>(0.0), static_cast<T>(1.0) };
		info.edge_case_results = {
			static_cast<T>(-3.14159265358979323846 / 2), // asin(-1) = -π/2
			static_cast<T>(0.0),                       // asin(0) = 0
			static_cast<T>(3.14159265358979323846 / 2)   // asin(1) = π/2
		};

		// Input must be in [-1, 1]
		info.validate_input = [](T x) { return x >= -1.0 && x <= 1.0; };
	}
	else if (func_name == "acos") {
		info.edge_cases = { static_cast<T>(-1.0), static_cast<T>(0.0), static_cast<T>(1.0) };
		info.edge_case_results = {
			static_cast<T>(3.14159265358979323846),    // acos(-1) = π
			static_cast<T>(3.14159265358979323846 / 2),  // acos(0) = π/2
			static_cast<T>(0.0)                        // acos(1) = 0
		};

		// Input must be in [-1, 1]
		info.validate_input = [](T x) { return x >= -1.0 && x <= 1.0; };
	}
	else if (func_name == "atan") {
		info.edge_cases = { static_cast<T>(-1.0), static_cast<T>(0.0), static_cast<T>(1.0) };
		info.edge_case_results = {
			static_cast<T>(-3.14159265358979323846 / 4), // atan(-1) = -π/4
			static_cast<T>(0.0),                       // atan(0) = 0
			static_cast<T>(3.14159265358979323846 / 4)   // atan(1) = π/4
		};
	}
	else if (func_name == "sinh") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // sinh(0) = 0
	}
	else if (func_name == "cosh") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(1.0) }; // cosh(0) = 1
	}
	else if (func_name == "tanh") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // tanh(0) = 0
	}
	else if (func_name == "asinh") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // asinh(0) = 0
	}
	else if (func_name == "acosh") {
		info.edge_cases = { static_cast<T>(1.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // acosh(1) = 0

		// Input must be >= 1
		info.validate_input = [](T x) { return x >= 1.0; };
	}
	else if (func_name == "atanh") {
		info.edge_cases = { static_cast<T>(0.0) };
		info.edge_case_results = { static_cast<T>(0.0) }; // atanh(0) = 0

		// Input must be in (-1, 1)
		info.validate_input = [](T x) { return x > -1.0 && x < 1.0; };
	}

	return info;
}

namespace trig_test {

	// Epsilon by precision qualifier: an absolute error up to magnitude 1, relative above it.
	template<typename T, glm::qualifier Q>
	T epsilon()
	{
		if constexpr (Q == glm::highp)
			return static_cast<T>(1e-6);
		else if constexpr (Q == glm::mediump)
			return static_cast<T>(1e-3);
		else
			return static_cast<T>(1e-1);
	}

	template<typename T>
	T tolerance(T eps, T expected)
	{
		return eps * glm::max(static_cast<T>(1), std::abs(expected));
	}

	template<typename T, glm::qualifier Q>
	glm::tvec4<T, Q> tolerance(T eps, glm::tvec4<T, Q> const& expected)
	{
		return eps * glm::max(glm::tvec4<T, Q>(1), glm::abs(expected));
	}

	// Test one trig function against its std:: reference
	template<typename T, glm::qualifier Q, typename StdFunc, typename GlmScalarFunc, typename GlmVectorFunc>
	int test_function(
		const std::string& func_name,
		StdFunc std_func,         // Reference std:: function (e.g., std::sin)
		GlmScalarFunc glm_scalar, // GLM scalar function (e.g., glm::sin<T, Q>)
		GlmVectorFunc glm_vector  // GLM vector function (e.g., glm::sin for vectors)
	) {
		int Error = 0;

		auto test_info = get_function_test_info<T>(func_name);
		auto test_angles = generate_test_angles<T>(func_name);
		T const eps = epsilon<T, Q>();

		// Scalar tests
		for (T a : test_angles) {
			if (!test_info.validate_input(a))
				continue;

			T expected = std_func(a);
			T actual = glm_scalar(a);

			// Both non-finite (NaN or Inf) is fine; one of them is a mismatch
			bool expected_is_finite = std::isfinite(expected);
			bool actual_is_finite = std::isfinite(actual);
			if (!expected_is_finite && !actual_is_finite)
				continue;
			Error += glm_test::context(GLM_CHECK_EQ(actual_is_finite, expected_is_finite), "a", a, "actual", actual, "expected", expected);
			if (expected_is_finite != actual_is_finite)
				continue;

			Error += glm_test::context(GLM_CHECK_LT(std::abs(actual - expected), tolerance(eps, expected)), "a", a, "actual", actual, "expected", expected);
		}

		// Vector tests
		for (T a : test_angles) {
			if (!test_info.validate_input(a))
				continue;

			glm::tvec4<T, Q> v(a);
			glm::tvec4<T, Q> expected_v(std_func(a));
			glm::tvec4<T, Q> actual_v = glm_vector(v);

			bool expected_is_finite = std::isfinite(expected_v.x);
			bool actual_is_finite = std::isfinite(actual_v.x);
			if (!expected_is_finite && !actual_is_finite)
				continue;
			Error += glm_test::context(GLM_CHECK_EQ(actual_is_finite, expected_is_finite), "a", a, "actual_v", actual_v, "expected_v", expected_v);
			if (expected_is_finite != actual_is_finite)
				continue;

			Error += glm_test::context(GLM_CHECK(!glm::any(glm::greaterThan(glm::abs(actual_v - expected_v), tolerance(eps, expected_v)))),
				"a", a, "actual_v", actual_v, "expected_v", expected_v);
		}

		// Mixed vector test: the first 4 valid inputs, one per component
		std::vector<T> valid_angles;
		for (T a : test_angles) {
			if (test_info.validate_input(a))
				valid_angles.push_back(a);
			if (valid_angles.size() == 4)
				break;
		}
		if (valid_angles.size() == 4) {
			glm::tvec4<T, Q> vec_angles(valid_angles[0], valid_angles[1], valid_angles[2], valid_angles[3]);
			glm::tvec4<T, Q> expected_vec(
				std_func(valid_angles[0]),
				std_func(valid_angles[1]),
				std_func(valid_angles[2]),
				std_func(valid_angles[3])
			);
			glm::tvec4<T, Q> actual_vec = glm_vector(vec_angles);

			Error += glm_test::context(GLM_CHECK(!glm::any(glm::greaterThan(glm::abs(actual_vec - expected_vec), tolerance(eps, expected_vec)))),
				"vec_angles", vec_angles, "actual_vec", actual_vec, "expected_vec", expected_vec);
		}

		// Edge cases
		for (size_t i = 0; i < test_info.edge_cases.size(); ++i) {
			T input = test_info.edge_cases[i];
			T expected = test_info.edge_case_results[i];
			T actual = glm_scalar(input);

			Error += glm_test::context(GLM_CHECK_LT(std::abs(actual - expected), tolerance(eps, expected)), "input", input, "actual", actual, "expected", expected);
		}

		return Error;
	}

	// Test a two-argument function (atan2) against its std:: reference
	template<typename T, glm::qualifier Q, typename StdFunc, typename GlmScalarFunc, typename GlmVectorFunc>
	int test_function2(
		StdFunc std_func,         // Reference std:: function (e.g., std::atan2)
		GlmScalarFunc glm_scalar, // GLM scalar function (e.g., glm::atan<T, Q>)
		GlmVectorFunc glm_vector  // GLM vector function (e.g., glm::atan for vectors)
	) {
		int Error = 0;

		std::vector<T> test_values = {
			static_cast<T>(-1.0), static_cast<T>(-0.5), static_cast<T>(0.0),
			static_cast<T>(0.5), static_cast<T>(1.0)
		};
		T const eps = epsilon<T, Q>();

		// Scalar and vector tests; (0, 0) is skipped
		for (T y : test_values) {
			for (T x : test_values) {
				if (y == 0 && x == 0)
					continue;

				T expected = std_func(y, x);
				T actual = glm_scalar(y, x);
				Error += glm_test::context(GLM_CHECK_LT(std::abs(actual - expected), tolerance(eps, expected)), "y", y, "x", x, "actual", actual, "expected", expected);

				glm::tvec4<T, Q> expected_v(expected);
				glm::tvec4<T, Q> actual_v = glm_vector(glm::tvec4<T, Q>(y), glm::tvec4<T, Q>(x));
				Error += glm_test::context(GLM_CHECK(!glm::any(glm::greaterThan(glm::abs(actual_v - expected_v), tolerance(eps, expected_v)))),
					"y", y, "x", x, "actual_v", actual_v, "expected_v", expected_v);
			}
		}

		// Mixed vector test
		{
			glm::tvec4<T, Q> y_vec(test_values[0], test_values[1], test_values[2], test_values[3]);
			glm::tvec4<T, Q> x_vec(test_values[1], test_values[2], test_values[3], test_values[4]);

			glm::tvec4<T, Q> expected_vec(
				std_func(test_values[0], test_values[1]),
				std_func(test_values[1], test_values[2]),
				std_func(test_values[2], test_values[3]),
				std_func(test_values[3], test_values[4])
			);
			glm::tvec4<T, Q> actual_vec = glm_vector(y_vec, x_vec);

			Error += glm_test::context(GLM_CHECK(!glm::any(glm::greaterThan(glm::abs(actual_vec - expected_vec), tolerance(eps, expected_vec)))),
				"y_vec", y_vec, "x_vec", x_vec, "actual_vec", actual_vec, "expected_vec", expected_vec);
		}

		return Error;
	}

	// Specialized test runners for each trig function

	// Sin function
	template<typename T, glm::qualifier Q>
	int test_sin() {
		return test_function<T, Q>(
			"sin",
			[](T x) { return std::sin(x); },
			[](T x) { return glm::sin<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::sin(v); }
		);
	}

	// Cos function
	template<typename T, glm::qualifier Q>
	int test_cos() {
		return test_function<T, Q>(
			"cos",
			[](T x) { return std::cos(x); },
			[](T x) { return glm::cos<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::cos(v); }
		);
	}

	// Tan function
	template<typename T, glm::qualifier Q>
	int test_tan() {
		return test_function<T, Q>(
			"tan",
			[](T x) { return std::tan(x); },
			[](T x) { return glm::tan<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::tan(v); }
		);
	}

	// Asin function
	template<typename T, glm::qualifier Q>
	int test_asin() {
		return test_function<T, Q>(
			"asin",
			[](T x) { return std::asin(x); },
			[](T x) { return glm::asin<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::asin(v); }
		);
	}

	// Acos function
	template<typename T, glm::qualifier Q>
	int test_acos() {
		return test_function<T, Q>(
			"acos",
			[](T x) { return std::acos(x); },
			[](T x) { return glm::acos<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::acos(v); }
		);
	}

	// Atan function (single argument)
	template<typename T, glm::qualifier Q>
	int test_atan() {
		return test_function<T, Q>(
			"atan",
			[](T x) { return std::atan(x); },
			[](T x) { return glm::atan<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::atan(v); }
		);
	}

	// Atan2 function (two arguments)
	template<typename T, glm::qualifier Q>
	int test_atan2() {
		return test_function2<T, Q>(
			[](T y, T x) { return std::atan2(y, x); },
			[](T y, T x) { return glm::atan<T, Q>(y, x); },
			[](glm::tvec4<T, Q> y, glm::tvec4<T, Q> x) { return glm::atan(y, x); }
		);
	}

	// Sinh function
	template<typename T, glm::qualifier Q>
	int test_sinh() {
		return test_function<T, Q>(
			"sinh",
			[](T x) { return std::sinh(x); },
			[](T x) { return glm::sinh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::sinh(v); }
		);
	}

	// Cosh function
	template<typename T, glm::qualifier Q>
	int test_cosh() {
		return test_function<T, Q>(
			"cosh",
			[](T x) { return std::cosh(x); },
			[](T x) { return glm::cosh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::cosh(v); }
		);
	}

	// Tanh function
	template<typename T, glm::qualifier Q>
	int test_tanh() {
		return test_function<T, Q>(
			"tanh",
			[](T x) { return std::tanh(x); },
			[](T x) { return glm::tanh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::tanh(v); }
		);
	}

	// Asinh function
	template<typename T, glm::qualifier Q>
	int test_asinh() {
		return test_function<T, Q>(
			"asinh",
			[](T x) { return std::asinh(x); },
			[](T x) { return glm::asinh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::asinh(v); }
		);
	}

	// Acosh function
	template<typename T, glm::qualifier Q>
	int test_acosh() {
		return test_function<T, Q>(
			"acosh",
			[](T x) { return std::acosh(x); },
			[](T x) { return glm::acosh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::acosh(v); }
		);
	}

	// Atanh function
	template<typename T, glm::qualifier Q>
	int test_atanh() {
		return test_function<T, Q>(
			"atanh",
			[](T x) { return std::atanh(x); },
			[](T x) { return glm::atanh<T, Q>(x); },
			[](glm::tvec4<T, Q> v) { return glm::atanh(v); }
		);
	}

	// Radians function
	template<typename T, glm::qualifier Q>
	int test_radians() {
		return test_function<T, Q>(
			"radians",
			[](T x) { return x * static_cast<T>(0.017453292519943295769236907684886); }, // std::radians doesn't exist
			[](T x) { return glm::radians(x); },
			[](glm::tvec4<T, Q> v) { return glm::radians(v); }
		);
	}

	// Degrees function
	template<typename T, glm::qualifier Q>
	int test_degrees() {
		return test_function<T, Q>(
			"degrees",
			[](T x) { return x * static_cast<T>(57.295779513082320876798154814105); }, // std::degrees doesn't exist
			[](T x) { return glm::degrees(x); },
			[](glm::tvec4<T, Q> v) { return glm::degrees(v); }
		);
	}

	// Outside their domains the functions return what the standard library does at every precision:
	// NaN, or +-inf at atanh's poles.
	template<typename T, glm::qualifier Q>
	int test_domain()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();
		int Error = 0;
		T const Inf = std::numeric_limits<T>::infinity();
		glm::tvec4<T, Q> const Outside(static_cast<T>(1.5), static_cast<T>(-1.5), static_cast<T>(2), static_cast<T>(-2));

		Error += GLM_CHECK(std::isnan(glm::asin<T, Q>(static_cast<T>(1.5))));
		Error += GLM_CHECK(std::isnan(glm::asin<T, Q>(static_cast<T>(-1.5))));
		Error += GLM_CHECK(std::isnan(glm::acos<T, Q>(static_cast<T>(1.5))));
		Error += GLM_CHECK(std::isnan(glm::acosh<T, Q>(static_cast<T>(0.5))));
		Error += GLM_CHECK(std::isnan(glm::atanh<T, Q>(static_cast<T>(2))));
		Error += GLM_CHECK(std::isnan(glm::atanh<T, Q>(static_cast<T>(-1.5))));
		Error += GLM_CHECK_EQ((glm::atanh<T, Q>(static_cast<T>(1))), Inf);
		Error += GLM_CHECK_EQ((glm::atanh<T, Q>(static_cast<T>(-1))), -Inf);

		Error += GLM_CHECK(glm::all(glm::isnan(glm::asin(Outside))));
		Error += GLM_CHECK(glm::all(glm::isnan(glm::acos(Outside))));
		Error += GLM_CHECK(glm::all(glm::isnan(glm::atanh(Outside))));
		Error += GLM_CHECK(glm::all(glm::isnan(glm::acosh(glm::tvec4<T, Q>(static_cast<T>(0.5), static_cast<T>(0), static_cast<T>(-1), static_cast<T>(-3))))));

		return Error;
	}

	// asin is accurate across the whole domain, the ends included, and odd: asin(0) is exactly 0.
	template<typename T, glm::qualifier Q>
	int test_asin_domain_sweep()
	{
		int Error = 0;
		T const eps = glm::min(epsilon<T, Q>(), static_cast<T>(1e-3));

		for(int i = -100; i <= 100; ++i)
		{
			T const x = static_cast<T>(i) / static_cast<T>(100);
			Error += glm_test::context(GLM_CHECK_NEAR((glm::asin<T, Q>(x)), std::asin(x), eps), "x", x);
		}
		Error += GLM_CHECK_EQ((glm::asin<T, Q>(static_cast<T>(0))), static_cast<T>(0));

		return Error;
	}

} // namespace trig_test

#define TRIG_TEST(fn) \
	Error += GLM_TEST(trig_test::fn<float, glm::highp>()); \
	Error += GLM_TEST(trig_test::fn<float, glm::mediump>()); \
	Error += GLM_TEST(trig_test::fn<float, glm::lowp>()); \
	Error += GLM_TEST(trig_test::fn<double, glm::highp>()); \
	Error += GLM_TEST(trig_test::fn<double, glm::mediump>()); \
	Error += GLM_TEST(trig_test::fn<double, glm::lowp>())

int main()
{
	int Error = 0;

	TRIG_TEST(test_sin);
	TRIG_TEST(test_cos);
	TRIG_TEST(test_tan);
	TRIG_TEST(test_asin);
	TRIG_TEST(test_acos);
	TRIG_TEST(test_atan);
	TRIG_TEST(test_atan2);
	TRIG_TEST(test_sinh);
	TRIG_TEST(test_cosh);
	TRIG_TEST(test_tanh);
	TRIG_TEST(test_asinh);
	TRIG_TEST(test_acosh);
	TRIG_TEST(test_atanh);
	TRIG_TEST(test_radians);
	TRIG_TEST(test_degrees);
	TRIG_TEST(test_domain);
	TRIG_TEST(test_asin_domain_sweep);

	return GLM_TEST_FINISH(Error);
}
