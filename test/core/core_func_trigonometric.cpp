#define GLM_ENABLE_EXPERIMENTAL 1

#include <glm/trigonometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/vector_relational.hpp>

#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>


// Helper function to get qualifier name as string
static std::string_view qualifier_name(glm::qualifier Q) {
	switch (Q) {
	case glm::qualifier::highp: return "highp";
	case glm::qualifier::mediump: return "mediump";
	case glm::qualifier::lowp: return "lowp";
	default: return "unknownp";
	}
}

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

	// Main test function template for any trig function
	template<typename T, glm::qualifier Q, typename StdFunc, typename GlmScalarFunc, typename GlmVectorFunc>
	int test_function(
		const std::string& func_name,
		StdFunc std_func,         // Reference std:: function (e.g., std::sin)
		GlmScalarFunc glm_scalar, // GLM scalar function (e.g., glm::sin<T, Q>)
		GlmVectorFunc glm_vector  // GLM vector function (e.g., glm::sin for vectors)
	) {
		int Error = 0;

		// Header with clear type and precision info
		std::cout << "===== " << func_name << " Test: " << typeid(T).name() << " with "
			<< qualifier_name(Q) << " precision =====" << std::endl;

		// Get test info for this function
		auto test_info = get_function_test_info<T>(func_name);

		// Generate test angles appropriate for this function
		auto test_angles = generate_test_angles<T>(func_name);

		// Set epsilon based on precision qualifier
		T eps;
		if constexpr (Q == glm::highp) {
			eps = static_cast<T>(1e-6);  // 0.0001% to 0.0002% error margin
		}
		else if constexpr (Q == glm::mediump) {
			eps = static_cast<T>(1e-3);  // 0.1% to 0.2% error margin
		}
		else {
			eps = static_cast<T>(1e-1);  // ~10% error margin
		}

		std::cout << "  Using epsilon: " << eps << std::endl;

		// Scalar tests
		std::cout << "  Testing scalar " << func_name << "..." << std::endl;
		for (T a : test_angles) {
			// Skip invalid inputs
			if (!test_info.validate_input(a)) {
				continue;
			}

			T expected = std_func(a);
			T actual = glm_scalar(a);

			// Check for NaN or Inf results
			bool expected_is_finite = std::isfinite(expected);
			bool actual_is_finite = std::isfinite(actual);

			if (!expected_is_finite && !actual_is_finite) {
				// Both are non-finite (NaN or Inf), which is fine
				continue;
			}

			if (expected_is_finite != actual_is_finite) {
				std::cout << "    FAIL: " << func_name << "(" << a << ") finiteness mismatch: "
					<< "std is " << (expected_is_finite ? "finite" : "non-finite")
					<< ", glm is " << (actual_is_finite ? "finite" : "non-finite") << std::endl;
				Error++;
				continue;
			}

			if (!glm::epsilonEqual(actual, expected, eps)) {
				std::cout << "    FAIL: " << func_name << "(" << a << ") = " << actual
					<< ", expected " << expected
					<< ", diff = " << std::abs(actual - expected) << std::endl;
				Error++;
			}
		}

		// Vector tests
		std::cout << "  Testing vector " << func_name << "..." << std::endl;
		for (T a : test_angles) {
			// Skip invalid inputs
			if (!test_info.validate_input(a)) {
				continue;
			}

			glm::tvec4<T, Q> v(a);
			glm::tvec4<T, Q> expected_v(std_func(a));
			glm::tvec4<T, Q> actual_v = glm_vector(v);

			// Check for NaN or Inf results
			bool expected_is_finite = std::isfinite(expected_v.x);
			bool actual_is_finite = std::isfinite(actual_v.x);

			if (!expected_is_finite && !actual_is_finite) {
				// Both are non-finite, which is fine
				continue;
			}

			if (expected_is_finite != actual_is_finite) {
				std::cout << "    FAIL: vec4(" << func_name << "(" << a << ")) finiteness mismatch" << std::endl;
				Error++;
				continue;
			}

			if (glm::any(glm::greaterThan(glm::abs(actual_v - expected_v), glm::tvec4<T, Q>(eps)))) {
				std::cout << "    FAIL: " << func_name << "(vec4(" << a << ")) = "
					<< glm::to_string(actual_v) << ", expected "
					<< glm::to_string(expected_v) << std::endl;
				Error++;
			}
		}

		// Mixed vector test (if we have enough valid inputs)
		if (test_angles.size() >= 4) {
			std::cout << "  Testing mixed vector " << func_name << "..." << std::endl;

			// Find 4 valid angles for the test
			std::vector<T> valid_angles;
			for (T a : test_angles) {
				if (test_info.validate_input(a) && valid_angles.size() < 4) {
					valid_angles.push_back(a);
				}
				if (valid_angles.size() == 4) break;
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

				if (glm::any(glm::greaterThan(glm::abs(actual_vec - expected_vec), glm::tvec4<T, Q>(eps)))) {
					std::cout << "    FAIL: " << func_name << "(mixed_vec) = "
						<< glm::to_string(actual_vec) << ", expected "
						<< glm::to_string(expected_vec) << std::endl;
					// Show individual components for easier debugging
					for (int i = 0; i < 4; i++) {
						std::cout << "      Component " << i << " (" << vec_angles[i] << "): " << actual_vec[i]
							<< " vs " << expected_vec[i]
							<< ", diff = " << std::abs(actual_vec[i] - expected_vec[i]) << std::endl;
					}
					Error++;
				}
			}
			else {
				std::cout << "    SKIP: Not enough valid inputs for mixed vector test" << std::endl;
			}
		}

		// Edge case tests
		if (!test_info.edge_cases.empty()) {
			std::cout << "  Testing " << func_name << " edge cases..." << std::endl;
			for (size_t i = 0; i < test_info.edge_cases.size(); ++i) {
				T input = test_info.edge_cases[i];
				T expected = test_info.edge_case_results[i];
				T actual = glm_scalar(input);

				if (!glm::epsilonEqual(actual, expected, eps)) {
					std::cout << "    FAIL: " << func_name << "(" << input << ") edge case = " << actual
						<< ", expected " << expected
						<< ", diff = " << std::abs(actual - expected) << std::endl;
					Error++;
				}
			}
		}

		// Summary
		if (Error == 0) {
			std::cout << "  PASSED: All " << func_name << " tests for " << typeid(T).name()
				<< " with " << qualifier_name(Q) << " precision" << std::endl;
		}
		else {
			std::cout << "  FAILED: " << Error << " error(s) in " << func_name << " tests for "
				<< typeid(T).name() << " with " << qualifier_name(Q)
				<< " precision" << std::endl;
		}
		std::cout << std::endl;  // Add some spacing between test sections

		return Error;
	}

	// Helper function for two-argument functions (e.g., atan2)
	template<typename T, glm::qualifier Q, typename StdFunc, typename GlmScalarFunc, typename GlmVectorFunc>
	int test_function2(
		const std::string& func_name,
		StdFunc std_func,         // Reference std:: function (e.g., std::atan2)
		GlmScalarFunc glm_scalar, // GLM scalar function (e.g., glm::atan<T, Q>)
		GlmVectorFunc glm_vector  // GLM vector function (e.g., glm::atan for vectors)
	) {
		int Error = 0;

		// Header with clear type and precision info
		std::cout << "===== " << func_name << " Test: " << typeid(T).name() << " with "
			<< qualifier_name(Q) << " precision =====" << std::endl;

		// Generate test values
		std::vector<T> test_values = {
			static_cast<T>(-1.0), static_cast<T>(-0.5), static_cast<T>(0.0),
			static_cast<T>(0.5), static_cast<T>(1.0)
		};

		// Set epsilon based on precision qualifier
		T eps;
		if constexpr (Q == glm::highp) {
			eps = static_cast<T>(1e-6);
		}
		else if constexpr (Q == glm::mediump) {
			eps = static_cast<T>(1e-3);
		}
		else {
			eps = static_cast<T>(1e-1);
		}

		std::cout << "  Using epsilon: " << eps << std::endl;

		// Scalar tests
		std::cout << "  Testing scalar " << func_name << "..." << std::endl;
		for (T y : test_values) {
			for (T x : test_values) {
				// Skip (0,0) for atan2
				if (func_name == "atan2" && y == 0 && x == 0) {
					continue;
				}

				T expected = std_func(y, x);
				T actual = glm_scalar(y, x);

				if (!glm::epsilonEqual(actual, expected, eps)) {
					std::cout << "    FAIL: " << func_name << "(" << y << ", " << x << ") = " << actual
						<< ", expected " << expected
						<< ", diff = " << std::abs(actual - expected) << std::endl;
					Error++;
				}
			}
		}

		// Vector tests
		std::cout << "  Testing vector " << func_name << "..." << std::endl;
		for (T y : test_values) {
			for (T x : test_values) {
				// Skip (0,0) for atan2
				if (func_name == "atan2" && y == 0 && x == 0) {
					continue;
				}

				glm::tvec4<T, Q> v_y(y);
				glm::tvec4<T, Q> v_x(x);
				glm::tvec4<T, Q> expected_v(std_func(y, x));
				glm::tvec4<T, Q> actual_v = glm_vector(v_y, v_x);

				if (glm::any(glm::greaterThan(glm::abs(actual_v - expected_v), glm::tvec4<T, Q>(eps)))) {
					std::cout << "    FAIL: " << func_name << "(vec4(" << y << "), vec4(" << x << ")) = "
						<< glm::to_string(actual_v) << ", expected "
						<< glm::to_string(expected_v) << std::endl;
					Error++;
				}
			}
		}

		// Mixed vector test
		std::cout << "  Testing mixed vector " << func_name << "..." << std::endl;
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

			if (glm::any(glm::greaterThan(glm::abs(actual_vec - expected_vec), glm::tvec4<T, Q>(eps)))) {
				std::cout << "    FAIL: " << func_name << "(mixed_vec_y, mixed_vec_x) = "
					<< glm::to_string(actual_vec) << ", expected "
					<< glm::to_string(expected_vec) << std::endl;
				for (int i = 0; i < 4; i++) {
					std::cout << "      Component " << i << " (" << y_vec[i] << ", " << x_vec[i] << "): "
						<< actual_vec[i] << " vs " << expected_vec[i]
						<< ", diff = " << std::abs(actual_vec[i] - expected_vec[i]) << std::endl;
				}
				Error++;
			}
		}

		// Summary
		if (Error == 0) {
			std::cout << "  PASSED: All " << func_name << " tests for " << typeid(T).name()
				<< " with " << qualifier_name(Q) << " precision" << std::endl;
		}
		else {
			std::cout << "  FAILED: " << Error << " error(s) in " << func_name << " tests for "
				<< typeid(T).name() << " with " << qualifier_name(Q)
				<< " precision" << std::endl;
		}
		std::cout << std::endl;

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
			"atan2",
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

} // namespace trig_test

int main() {
	auto start_time = std::chrono::high_resolution_clock::now();
	int Error = 0;

	std::cout << "\n=======================================================" << std::endl;
	std::cout << "= GLM TRIGONOMETRIC FUNCTIONS TEST SUITE =" << std::endl;
	std::cout << "=======================================================" << std::endl;

	// Define the list of tests to run
	struct TestInfo {
		std::string name;
		std::function<int()> test_float_highp;
		std::function<int()> test_float_mediump;
		std::function<int()> test_float_lowp;
		std::function<int()> test_double_highp;
		std::function<int()> test_double_mediump;
		std::function<int()> test_double_lowp;
	};

	std::vector<TestInfo> tests = {
		{
			"sin",
			[]() { return trig_test::test_sin<float, glm::highp>(); },
			[]() { return trig_test::test_sin<float, glm::mediump>(); },
			[]() { return trig_test::test_sin<float, glm::lowp>(); },
			[]() { return trig_test::test_sin<double, glm::highp>(); },
			[]() { return trig_test::test_sin<double, glm::mediump>(); },
			[]() { return trig_test::test_sin<double, glm::lowp>(); }
		},
		{
			"cos",
			[]() { return trig_test::test_cos<float, glm::highp>(); },
			[]() { return trig_test::test_cos<float, glm::mediump>(); },
			[]() { return trig_test::test_cos<float, glm::lowp>(); },
			[]() { return trig_test::test_cos<double, glm::highp>(); },
			[]() { return trig_test::test_cos<double, glm::mediump>(); },
			[]() { return trig_test::test_cos<double, glm::lowp>(); }
		},
		{
			"tan",
			[]() { return trig_test::test_tan<float, glm::highp>(); },
			[]() { return trig_test::test_tan<float, glm::mediump>(); },
			[]() { return trig_test::test_tan<float, glm::lowp>(); },
			[]() { return trig_test::test_tan<double, glm::highp>(); },
			[]() { return trig_test::test_tan<double, glm::mediump>(); },
			[]() { return trig_test::test_tan<double, glm::lowp>(); }
		},
		{
			"asin",
			[]() { return trig_test::test_asin<float, glm::highp>(); },
			[]() { return trig_test::test_asin<float, glm::mediump>(); },
			[]() { return trig_test::test_asin<float, glm::lowp>(); },
			[]() { return trig_test::test_asin<double, glm::highp>(); },
			[]() { return trig_test::test_asin<double, glm::mediump>(); },
			[]() { return trig_test::test_asin<double, glm::lowp>(); }
		},
		{
			"acos",
			[]() { return trig_test::test_acos<float, glm::highp>(); },
			[]() { return trig_test::test_acos<float, glm::mediump>(); },
			[]() { return trig_test::test_acos<float, glm::lowp>(); },
			[]() { return trig_test::test_acos<double, glm::highp>(); },
			[]() { return trig_test::test_acos<double, glm::mediump>(); },
			[]() { return trig_test::test_acos<double, glm::lowp>(); }
		},
		{
			"atan",
			[]() { return trig_test::test_atan<float, glm::highp>(); },
			[]() { return trig_test::test_atan<float, glm::mediump>(); },
			[]() { return trig_test::test_atan<float, glm::lowp>(); },
			[]() { return trig_test::test_atan<double, glm::highp>(); },
			[]() { return trig_test::test_atan<double, glm::mediump>(); },
			[]() { return trig_test::test_atan<double, glm::lowp>(); }
		},
		{
			"atan2",
			[]() { return trig_test::test_atan2<float, glm::highp>(); },
			[]() { return trig_test::test_atan2<float, glm::mediump>(); },
			[]() { return trig_test::test_atan2<float, glm::lowp>(); },
			[]() { return trig_test::test_atan2<double, glm::highp>(); },
			[]() { return trig_test::test_atan2<double, glm::mediump>(); },
			[]() { return trig_test::test_atan2<double, glm::lowp>(); }
		},
		{
			"sinh",
			[]() { return trig_test::test_sinh<float, glm::highp>(); },
			[]() { return trig_test::test_sinh<float, glm::mediump>(); },
			[]() { return trig_test::test_sinh<float, glm::lowp>(); },
			[]() { return trig_test::test_sinh<double, glm::highp>(); },
			[]() { return trig_test::test_sinh<double, glm::mediump>(); },
			[]() { return trig_test::test_sinh<double, glm::lowp>(); }
		},
		{
			"cosh",
			[]() { return trig_test::test_cosh<float, glm::highp>(); },
			[]() { return trig_test::test_cosh<float, glm::mediump>(); },
			[]() { return trig_test::test_cosh<float, glm::lowp>(); },
			[]() { return trig_test::test_cosh<double, glm::highp>(); },
			[]() { return trig_test::test_cosh<double, glm::mediump>(); },
			[]() { return trig_test::test_cosh<double, glm::lowp>(); }
		},
		{
			"tanh",
			[]() { return trig_test::test_tanh<float, glm::highp>(); },
			[]() { return trig_test::test_tanh<float, glm::mediump>(); },
			[]() { return trig_test::test_tanh<float, glm::lowp>(); },
			[]() { return trig_test::test_tanh<double, glm::highp>(); },
			[]() { return trig_test::test_tanh<double, glm::mediump>(); },
			[]() { return trig_test::test_tanh<double, glm::lowp>(); }
		},
		{
			"asinh",
			[]() { return trig_test::test_asinh<float, glm::highp>(); },
			[]() { return trig_test::test_asinh<float, glm::mediump>(); },
			[]() { return trig_test::test_asinh<float, glm::lowp>(); },
			[]() { return trig_test::test_asinh<double, glm::highp>(); },
			[]() { return trig_test::test_asinh<double, glm::mediump>(); },
			[]() { return trig_test::test_asinh<double, glm::lowp>(); }
		},
		{
			"acosh",
			[]() { return trig_test::test_acosh<float, glm::highp>(); },
			[]() { return trig_test::test_acosh<float, glm::mediump>(); },
			[]() { return trig_test::test_acosh<float, glm::lowp>(); },
			[]() { return trig_test::test_acosh<double, glm::highp>(); },
			[]() { return trig_test::test_acosh<double, glm::mediump>(); },
			[]() { return trig_test::test_acosh<double, glm::lowp>(); }
		},
		{
			"atanh",
			[]() { return trig_test::test_atanh<float, glm::highp>(); },
			[]() { return trig_test::test_atanh<float, glm::mediump>(); },
			[]() { return trig_test::test_atanh<float, glm::lowp>(); },
			[]() { return trig_test::test_atanh<double, glm::highp>(); },
			[]() { return trig_test::test_atanh<double, glm::mediump>(); },
			[]() { return trig_test::test_atanh<double, glm::lowp>(); }
		},
		{
			"radians",
			[]() { return trig_test::test_radians<float, glm::highp>(); },
			[]() { return trig_test::test_radians<float, glm::mediump>(); },
			[]() { return trig_test::test_radians<float, glm::lowp>(); },
			[]() { return trig_test::test_radians<double, glm::highp>(); },
			[]() { return trig_test::test_radians<double, glm::mediump>(); },
			[]() { return trig_test::test_radians<double, glm::lowp>(); }
		},
		{
			"degrees",
			[]() { return trig_test::test_degrees<float, glm::highp>(); },
			[]() { return trig_test::test_degrees<float, glm::mediump>(); },
			[]() { return trig_test::test_degrees<float, glm::lowp>(); },
			[]() { return trig_test::test_degrees<double, glm::highp>(); },
			[]() { return trig_test::test_degrees<double, glm::mediump>(); },
			[]() { return trig_test::test_degrees<double, glm::lowp>(); }
		}
	};

	// Track test results
	std::vector<std::pair<std::string, int>> test_results;

	// Run all tests
	for (const auto& test : tests) {
		std::cout << "\n\n=======================================================" << std::endl;
		std::cout << "TESTING: " << test.name << std::endl;
		std::cout << "=======================================================" << std::endl;

		int function_errors = 0;

		// Float tests
		std::cout << "\n--- FLOAT TESTS ---" << std::endl;
		function_errors += test.test_float_highp();
		function_errors += test.test_float_mediump();
		function_errors += test.test_float_lowp();

		// Double tests
		std::cout << "\n--- DOUBLE TESTS ---" << std::endl;
		function_errors += test.test_double_highp();
		function_errors += test.test_double_mediump();
		function_errors += test.test_double_lowp();

		// Update total error count
		Error += function_errors;

		// Store result for summary
		test_results.push_back({ test.name, function_errors });

		// Print function summary
		std::cout << "\n" << test.name << " SUMMARY: ";
		if (function_errors == 0) {
			std::cout << "ALL TESTS PASSED" << std::endl;
		}
		else {
			std::cout << function_errors << " ERROR(S) DETECTED" << std::endl;
		}
	}

	// Calculate and print execution time
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	// Print final summary
	std::cout << "\n\n=======================================================" << std::endl;
	std::cout << "TRIGONOMETRIC FUNCTIONS TEST SUMMARY" << std::endl;
	std::cout << "=======================================================" << std::endl;

	std::cout << std::left << std::setw(15) << "Function" << " | " << std::setw(12) << "Status" << std::endl;
	std::cout << "--------------------------------" << std::endl;

	for (const auto& result : test_results) {
		std::cout << std::left << std::setw(15) << result.first << " | "
			<< std::setw(12) << (result.second == 0 ? "PASS" : "FAIL (" + std::to_string(result.second) + ")")
			<< std::endl;
	}

	std::cout << "\n";
	if (Error == 0) {
		std::cout << "ALL TESTS PASSED SUCCESSFULLY!" << std::endl;
	}
	else {
		std::cout << "TESTS FAILED WITH " << Error << " ERROR(S)" << std::endl;
	}

	std::cout << "Total execution time: " << duration << " ms" << std::endl;

	return Error;
}
