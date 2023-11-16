#include <glm/ext/quaternion_double.hpp>
#include <glm/ext/quaternion_double_precision.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_float_precision.hpp>
#include <glm/ext/quaternion_relational.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <iostream>

// Helper function to create quaternions consistently regardless of layout
// This handles both storage and constructor layout macros
template <typename T = float, glm::qualifier Q = glm::defaultp>
static glm::qua<T, Q> make_quat(T w, T x, T y, T z) {
#ifdef GLM_FORCE_QUAT_DATA_XYZW
	// When GLM_FORCE_QUAT_DATA_XYZW is defined, constructor expects (x,y,z,w)
	return glm::qua<T, Q>(x, y, z, w);
#else
	// Default behavior with constructor expecting (w,x,y,z)
	return glm::qua<T, Q>(w, x, y, z);
#endif
}

// Print quaternion components for debugging
template <typename T, glm::qualifier Q>
static void print_quat(const std::string& label, const glm::qua<T, Q>& q) {
	std::cout << label << ": w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;
}

static int test_ctr()
{
	int Error(0);

	std::cout << "Testing constructors and C++ type traits:" << std::endl;

#   if GLM_HAS_TRIVIAL_QUERIES
	//  Error += std::is_trivially_default_constructible<glm::quat>::value ? 0 : 1;
	//  Error += std::is_trivially_default_constructible<glm::dquat>::value ? 0 : 1;
	//  Error += std::is_trivially_copy_assignable<glm::quat>::value ? 0 : 1;
	//  Error += std::is_trivially_copy_assignable<glm::dquat>::value ? 0 : 1;
	Error += std::is_trivially_copyable<glm::quat>::value ? 0 : 1;
	Error += std::is_trivially_copyable<glm::dquat>::value ? 0 : 1;

	Error += std::is_copy_constructible<glm::quat>::value ? 0 : 1;
	Error += std::is_copy_constructible<glm::dquat>::value ? 0 : 1;

	if (Error > 0) {
		std::cout << "Error: One or more type traits checks failed" << std::endl;
	}
#   endif

#   if GLM_HAS_INITIALIZER_LISTS
	{
		// For initializer lists, the order matches the constructor parameter order:
		// - If GLM_FORCE_QUAT_DATA_XYZW is defined: constructor takes (x,y,z,w)
		// - Otherwise (default): constructor takes (w,x,y,z)
		glm::quat A{ 0, 1, 2, 3 };

		// Check based on constructor parameter order
#       ifdef GLM_FORCE_QUAT_DATA_XYZW
		// With XYZW constructor order: {0,1,2,3} means {x=0,y=1,z=2,w=3}
		if (A.x != 0 || A.y != 1 || A.z != 2 || A.w != 3) {
			std::cout << "Error: Initializer list doesn't match expected XYZW constructor order" << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
#       else
		// With WXYZ constructor order (default): {0,1,2,3} means {w=0,x=1,y=2,z=3}
		if (A.w != 0 || A.x != 1 || A.y != 2 || A.z != 3) {
			std::cout << "Error: Initializer list doesn't match expected WXYZ constructor order" << std::endl;
			print_quat("Quaternion", A);
			Error++;
		}
#       endif

		// Test vector of quaternions
		std::vector<glm::quat> B{
			A,
			{0, 1, 2, 3} };

		if (B.size() != 2 || !(B[0] == A)) {
			std::cout << "Error: Vector of quaternions with initializer list failed" << std::endl;
			Error++;
		}
	}
#   endif//GLM_HAS_INITIALIZER_LISTS

	// Test additional constructors
	{
		// IMPORTANT: The default constructor in GLM does NOT initialize to identity!
		// It leaves values uninitialized for performance reasons
		// Instead, we should use explicit construction for identity quaternion
		glm::quat identity = make_quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat explicit_identity(1.0f, 0.0f, 0.0f, 0.0f);

		if (explicit_identity != identity) {
			std::cout << "Error: Explicit identity constructor failed" << std::endl;
			print_quat("Explicit", explicit_identity);
			print_quat("Expected identity", identity);
			Error++;
		}

		// Test scalar + vector constructor
		glm::vec3 v(1.0f, 2.0f, 3.0f);
		glm::quat q2(4.0f, v);

#       ifdef GLM_FORCE_QUAT_DATA_WXYZ
		if (q2.w != 4.0f || q2.x != 1.0f || q2.y != 2.0f || q2.z != 3.0f) {
			std::cout << "Error: Scalar + vector constructor failed for WXYZ layout" << std::endl;
			print_quat("Result", q2);
			Error++;
		}
#       else
		if (q2.x != 1.0f || q2.y != 2.0f || q2.z != 3.0f || q2.w != 4.0f) {
			std::cout << "Error: Scalar + vector constructor failed for XYZW layout" << std::endl;
			print_quat("Result", q2);
			Error++;
		}
#       endif
	}

	return Error;
}

static int test_two_axis_ctr()
{
	int Error = 0;

	std::cout << "\nTesting two-axis constructor:" << std::endl;

	// Test 1: Rotation from X to Y axis
	{
		glm::quat const q1(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
		glm::vec3 const v1 = q1 * glm::vec3(1, 0, 0);

		if (!glm::all(glm::equal(v1, glm::vec3(0, 1, 0), 0.0001f))) {
			std::cout << "Error: Rotation from X to Y failed" << std::endl;
			std::cout << "Rotated vector: " << v1.x << ", " << v1.y << ", " << v1.z << std::endl;
			Error++;
		}
	}

	// Test 2: Double rotation should rotate by 180 degrees
	{
		glm::quat const q1(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
		glm::quat const q2 = q1 * q1;
		glm::vec3 const v2 = q2 * glm::vec3(1, 0, 0);

		if (!glm::all(glm::equal(v2, glm::vec3(-1, 0, 0), 0.0001f))) {
			std::cout << "Error: Double rotation failed" << std::endl;
			std::cout << "Rotated vector: " << v2.x << ", " << v2.y << ", " << v2.z << std::endl;
			print_quat("Double rotation quaternion", q2);
			Error++;
		}
	}

	// Test 3: Rotation from X to -X (180 degrees)
	{
		glm::quat const q3(glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0));
		glm::vec3 const v3 = q3 * glm::vec3(1, 0, 0);

		if (!glm::all(glm::equal(v3, glm::vec3(-1, 0, 0), 0.0001f))) {
			std::cout << "Error: Rotation from X to -X failed" << std::endl;
			std::cout << "Rotated vector: " << v3.x << ", " << v3.y << ", " << v3.z << std::endl;
			Error++;
		}
	}

	// Test 4: Rotation from Y to -Y (180 degrees)
	{
		glm::quat const q4(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0));
		glm::vec3 const v4 = q4 * glm::vec3(0, 1, 0);

		if (!glm::all(glm::equal(v4, glm::vec3(0, -1, 0), 0.0001f))) {
			std::cout << "Error: Rotation from Y to -Y failed" << std::endl;
			std::cout << "Rotated vector: " << v4.x << ", " << v4.y << ", " << v4.z << std::endl;
			Error++;
		}
	}

	// Test 5: Rotation from Z to -Z (180 degrees)
	{
		glm::quat const q5(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1));
		glm::vec3 const v5 = q5 * glm::vec3(0, 0, 1);

		if (!glm::all(glm::equal(v5, glm::vec3(0, 0, -1), 0.0001f))) {
			std::cout << "Error: Rotation from Z to -Z failed" << std::endl;
			std::cout << "Rotated vector: " << v5.x << ", " << v5.y << ", " << v5.z << std::endl;
			Error++;
		}
	}

	return Error;
}

static int test_accessors()
{
	int Error = 0;

	std::cout << "\nTesting accessors:" << std::endl;

	// Test individual component accessors
	{
		glm::quat q = make_quat(1.0f, 2.0f, 3.0f, 4.0f);

		if (q.w != 1.0f || q.x != 2.0f || q.y != 3.0f || q.z != 4.0f) {
			std::cout << "Error: Component accessors not working correctly" << std::endl;
			print_quat("Quaternion", q);
			Error++;
		}
	}

	// Test operator[] accessor
	{
		glm::quat q = make_quat(1.0f, 2.0f, 3.0f, 4.0f);

#       ifdef GLM_FORCE_QUAT_DATA_WXYZ
		if (q[0] != 1.0f || q[1] != 2.0f || q[2] != 3.0f || q[3] != 4.0f) {
			std::cout << "Error: operator[] not working correctly for WXYZ layout" << std::endl;
			std::cout << "Values: " << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << std::endl;
			Error++;
		}
#       else
		if (q[0] != 2.0f || q[1] != 3.0f || q[2] != 4.0f || q[3] != 1.0f) {
			std::cout << "Error: operator[] not working correctly for XYZW layout" << std::endl;
			std::cout << "Values: " << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << std::endl;
			Error++;
		}
#       endif
	}

	return Error;
}

static int test_size()
{
	int Error = 0;

	std::cout << "\nTesting size and length:" << std::endl;

	// Test size of quaternion types
	std::size_t const A = sizeof(glm::quat);
	if (16 != A) {
		std::cout << "Error: sizeof(glm::quat) = " << A << ", expected 16" << std::endl;
		Error++;
	}

	std::size_t const B = sizeof(glm::dquat);
	if (32 != B) {
		std::cout << "Error: sizeof(glm::dquat) = " << B << ", expected 32" << std::endl;
		Error++;
	}

	// Test length() function
	if (glm::quat().length() != 4) {
		std::cout << "Error: glm::quat().length() != 4" << std::endl;
		Error++;
	}

	if (glm::dquat().length() != 4) {
		std::cout << "Error: glm::dquat().length() != 4" << std::endl;
		Error++;
	}

	if (glm::quat::length() != 4) {
		std::cout << "Error: glm::quat::length() != 4" << std::endl;
		Error++;
	}

	if (glm::dquat::length() != 4) {
		std::cout << "Error: glm::dquat::length() != 4" << std::endl;
		Error++;
	}

	return Error;
}

static int test_precision()
{
	int Error = 0;

	std::cout << "\nTesting precision hierarchy:" << std::endl;

	if (sizeof(glm::lowp_quat) > sizeof(glm::mediump_quat)) {
		std::cout << "Error: sizeof(glm::lowp_quat) > sizeof(glm::mediump_quat)" << std::endl;
		std::cout << "  lowp: " << sizeof(glm::lowp_quat) << " bytes" << std::endl;
		std::cout << "  mediump: " << sizeof(glm::mediump_quat) << " bytes" << std::endl;
		Error++;
	}

	if (sizeof(glm::mediump_quat) > sizeof(glm::highp_quat)) {
		std::cout << "Error: sizeof(glm::mediump_quat) > sizeof(glm::highp_quat)" << std::endl;
		std::cout << "  mediump: " << sizeof(glm::mediump_quat) << " bytes" << std::endl;
		std::cout << "  highp: " << sizeof(glm::highp_quat) << " bytes" << std::endl;
		Error++;
	}

	return Error;
}

static int test_constexpr()
{
	int Error = 0;

	std::cout << "\nTesting constexpr capabilities:" << std::endl;

#if GLM_HAS_CONSTEXPR
	static_assert(glm::quat::length() == 4, "GLM: Failed constexpr");
	static_assert(glm::quat(1.0f, glm::vec3(0.0f)).w > 0.0f, "GLM: Failed constexpr");
	std::cout << "Constexpr tests pass" << std::endl;
#else
	std::cout << "Constexpr not supported by compiler" << std::endl;
#endif

	return Error;
}

int main()
{
	int Error = 0;

	std::cout << "=== Quaternion Type Extension Tests ===" << std::endl;

	// Determine the quaternion storage layout
	std::cout << "Quaternion memory layout: ";
#ifdef GLM_FORCE_QUAT_DATA_WXYZ
	std::cout << "WXYZ (storage)";
#else
	std::cout << "XYZW (storage)";
#endif

	std::cout << " / ";

#ifdef GLM_FORCE_QUAT_DATA_XYZW
	std::cout << "XYZW (constructor)" << std::endl;
#else
	std::cout << "WXYZ (constructor)" << std::endl;
#endif
	std::cout << std::endl;

	Error += test_ctr();
	Error += test_two_axis_ctr();
	Error += test_accessors();
	Error += test_size();
	Error += test_precision();
	Error += test_constexpr();

	if (Error == 0)
		std::cout << "\nAll tests passed!" << std::endl;
	else
		std::cout << "\nFailed tests: " << Error << std::endl;

	return Error;
}
