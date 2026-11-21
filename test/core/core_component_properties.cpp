// Components and swizzles as properties (GLM_CONFIG_COMPONENT_PROPERTIES): each use of a component
// or swizzle is a value, and writes go through the vector's storage. The value semantics also hold
// with plain members, so most of this runs either way; only the swizzle checks need properties.

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_ALIGNED_GENTYPES
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <type_traits>

template<glm::qualifier Q>
static int test_components()
{
	using v3 = glm::vec<3, float, Q>;
	int Error = 0;

	static_assert(std::is_trivially_copyable<v3>::value, "");
	static_assert(std::is_trivially_default_constructible<v3>::value, "");

	v3 A(1.0f, 2.0f, 3.0f);
	v3 const B(4.0f, 5.0f, 6.0f);

	// A component read is a value of the component type
	auto C = A.y;
	static_assert(std::is_same<decltype(C), float>::value, "auto c = v.y is a float");
	C = 100.0f;
	Error += GLM_CHECK_EQ(A.y, 2.0f);

	// Assignment of the same component of another vector assigns the value
	A.x = B.x;
	A.y = B.z;
	Error += GLM_CHECK_EQ(A, v3(4.0f, 6.0f, 3.0f));

	// Compound assignment, increment
	A.z += 1.0f;
	A.x *= 2.0f;
	A.y++;
	--A.y;
	Error += GLM_CHECK_EQ(A, v3(8.0f, 6.0f, 4.0f));

	// The other names for the same components
	A.g = 7.0f;
	A.p = 9.0f;
	Error += GLM_CHECK_EQ(A, v3(8.0f, 7.0f, 9.0f));
	Error += GLM_CHECK(A.r == A.x && A.s == A.x && A.b == A.z);

	// Template argument deduction sees the component type
	Error += GLM_CHECK_EQ(std::max(A.x, A.z), 9.0f);

	// References to the components, through operator[] and value_ptr
	A[1] = 3.0f;
	glm::value_ptr(A)[2] = 2.0f;
	float& X = A[0];
	X = 1.0f;
	Error += GLM_CHECK_EQ(A, v3(1.0f, 3.0f, 2.0f));

	return Error;
}

template<glm::qualifier Q>
static int test_quat()
{
	using q = glm::qua<float, Q>;
	int Error = 0;

	q A = q::wxyz(1.0f, 2.0f, 3.0f, 4.0f);
	A.w = 5.0f;
	A.z += 1.0f;
	Error += GLM_CHECK(A.w == 5.0f && A.x == 2.0f && A.y == 3.0f && A.z == 5.0f);
	// operator[] is in memory order: x, y, z, w by default
	Error += GLM_CHECK(A[0] == A.x && A[3] == A.w);

	return Error;
}

#if GLM_CONFIG_COMPONENT_PROPERTIES == GLM_ENABLE && GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
template<glm::qualifier Q>
static int test_swizzles()
{
	using v2 = glm::vec<2, float, Q>;
	using v3 = glm::vec<3, float, Q>;
	using v4 = glm::vec<4, float, Q>;
	int Error = 0;

	v3 A(1.0f, 2.0f, 3.0f);
	v3 const B(4.0f, 5.0f, 6.0f);

	// A swizzle is a value of the vector type
	auto C = A.xz;
	static_assert(std::is_same<decltype(C), v2>::value, "auto c = v.xz is a vec2");
	Error += GLM_CHECK_EQ(C, v2(1.0f, 3.0f));
	Error += GLM_CHECK_EQ(glm::dot(A.xy, B.xy), 14.0f);
	Error += GLM_CHECK_EQ(A.xz(), C);

	// Assigning the same swizzle of another vector (a.xy = b.xy) assigns the values
	A.xy = B.xy;
	Error += GLM_CHECK_EQ(A, v3(4.0f, 5.0f, 3.0f));

	// Overlapping swizzles of the same vector: all components are read before any is written
	A.xz = A.zx;
	Error += GLM_CHECK_EQ(A, v3(3.0f, 5.0f, 4.0f));

	// Scalar and compound assignment
	A.yz = 0.0f;
	Error += GLM_CHECK_EQ(A, v3(3.0f, 0.0f, 0.0f));
	A.zx += v2(1.0f, 2.0f);
	Error += GLM_CHECK_EQ(A, v3(5.0f, 0.0f, 1.0f));

	// Swizzles to and from other sizes
	v4 D = A.xyzx;
	Error += GLM_CHECK_EQ(D, v4(5.0f, 0.0f, 1.0f, 5.0f));
	D.wzyx = v4(1.0f, 2.0f, 3.0f, 4.0f);
	Error += GLM_CHECK_EQ(D, v4(4.0f, 3.0f, 2.0f, 1.0f));
	D.bgr = A.rgb;
	Error += GLM_CHECK_EQ(D, v4(1.0f, 0.0f, 5.0f, 1.0f));

	return Error;
}
#endif

template<glm::qualifier Q>
static int test_all()
{
	int Error = 0;
	Error += glm_test::context(test_components<Q>(), "Q", Q);
	Error += glm_test::context(test_quat<Q>(), "Q", Q);
#if GLM_CONFIG_COMPONENT_PROPERTIES == GLM_ENABLE && GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR
	Error += glm_test::context(test_swizzles<Q>(), "Q", Q);
#endif
	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_all<glm::packed_highp>());
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	Error += GLM_TEST(test_all<glm::aligned_highp>());
#endif

	return GLM_TEST_FINISH(Error);
}
