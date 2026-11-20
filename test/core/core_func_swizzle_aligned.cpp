// Swizzle operators on aligned types, which go through the SIMD swizzle implementations
// (detail/_swizzle_simd.inl) when an instruction set is enabled, and on packed types for comparison.
//
// Covers every source width, since the swizzle types don't know how wide their source is: reads
// within x/y of a vec2 must not touch memory past its 8 bytes, which a sanitizer build will catch.

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_ALIGNED_GENTYPES
#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <type_traits>

#if GLM_CONFIG_SWIZZLE == GLM_SWIZZLE_OPERATOR && GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE

// Compares component by component rather than with operator==, which has its own SIMD paths.
template<glm::length_t L, typename T, glm::qualifier Q>
static int check(glm::vec<L, T, Q> const& v, glm::vec<L, T, Q> const& expected, char const* expr, char const* values, int line)
{
	bool Ok = true;
	for (glm::length_t i = 0; i < L; ++i)
		Ok = Ok && v[i] == expected[i];
	return glm_test::check_pair(Ok, std::string(expr) + " == (" + values + ")", expr, v, "expected", expected, __FILE__, line);
}

#define CHECK(v, ...) check(v, std::decay_t<decltype(v)>(__VA_ARGS__), #v, #__VA_ARGS__, __LINE__)

template<typename T, glm::qualifier Q>
static int test_read()
{
	using v2 = glm::vec<2, T, Q>;
	using v3 = glm::vec<3, T, Q>;
	using v4 = glm::vec<4, T, Q>;

	int Error = 0;

	// From a heap-allocated vec2, so an access past its 8 bytes is a sanitizer-visible overflow.
	std::unique_ptr<v2> const A(new v2(T(1), T(2)));
	Error += CHECK(v2(A->yx), T(2), T(1));
	Error += CHECK(v2(A->xx), T(1), T(1));
	Error += CHECK(v3(A->xyx), T(1), T(2), T(1));
	Error += CHECK(v3(A->yyy), T(2), T(2), T(2));
	Error += CHECK(v4(A->yxyx), T(2), T(1), T(2), T(1));

	v3 const B(T(1), T(2), T(3));
	Error += CHECK(v2(B.zx), T(3), T(1));
	Error += CHECK(v2(B.yx), T(2), T(1));
	Error += CHECK(v3(B.zyx), T(3), T(2), T(1));
	Error += CHECK(v3(B.xyz), T(1), T(2), T(3));
	Error += CHECK(v4(B.zyxz), T(3), T(2), T(1), T(3));
	Error += CHECK(v4(B.rgbr), T(1), T(2), T(3), T(1));

	v4 const C(T(1), T(2), T(3), T(4));
	Error += CHECK(v2(C.wz), T(4), T(3));
	Error += CHECK(v2(C.xy), T(1), T(2));
	Error += CHECK(v3(C.wzy), T(4), T(3), T(2));
	Error += CHECK(v3(C.xwz), T(1), T(4), T(3));
	Error += CHECK(v4(C.wzyx), T(4), T(3), T(2), T(1));
	Error += CHECK(v4(C.qpts), T(4), T(3), T(2), T(1));
	Error += CHECK(v4(C.xxxx), T(1), T(1), T(1), T(1));

	return Error;
}

template<typename T, glm::qualifier Q>
static int test_write()
{
	using v2 = glm::vec<2, T, Q>;
	using v3 = glm::vec<3, T, Q>;
	using v4 = glm::vec<4, T, Q>;

	int Error = 0;

	{
		std::unique_ptr<v2> const A(new v2(T(1), T(2)));
		A->yx = v2(T(5), T(6));
		Error += CHECK(*A, T(6), T(5));
		A->xy += v2(T(1), T(2));
		Error += CHECK(*A, T(7), T(7));
		A->yx = T(3);
		Error += CHECK(*A, T(3), T(3));
	}

	{
		v3 B(T(1), T(2), T(3));
		B.zx = v2(T(7), T(8));
		Error += CHECK(B, T(8), T(2), T(7));
		B.zyx += v3(T(1), T(2), T(3));
		Error += CHECK(B, T(11), T(4), T(8));
		B.yz -= v2(T(1), T(2));
		Error += CHECK(B, T(11), T(3), T(6));
		B.xz *= v2(T(2), T(3));
		Error += CHECK(B, T(22), T(3), T(18));
		B.zx /= v2(T(3), T(2));
		Error += CHECK(B, T(11), T(3), T(6));
	}

	{
		v4 C(T(1), T(2), T(3), T(4));
		C.wy = v2(T(9), T(8));
		Error += CHECK(C, T(1), T(8), T(3), T(9));
		C.xwz += v3(T(1), T(1), T(1));
		Error += CHECK(C, T(2), T(8), T(4), T(10));
		C.wzyx *= v4(T(2), T(2), T(2), T(2));
		Error += CHECK(C, T(4), T(16), T(8), T(20));
		C.ywxz /= v4(T(4), T(5), T(2), T(2));
		Error += CHECK(C, T(2), T(4), T(4), T(4));
		C.zw = T(7);
		Error += CHECK(C, T(2), T(4), T(7), T(7));
	}

	// Self-assignment through overlapping swizzles: every component is read before any is written.
	{
		v4 D(T(1), T(2), T(3), T(4));
		D.xyzw = v4(D.yzwx);
		Error += CHECK(D, T(2), T(3), T(4), T(1));
		D.wzyx = v4(D);
		Error += CHECK(D, T(1), T(4), T(3), T(2));
	}

	return Error;
}

template<glm::qualifier Q>
static int test_bool()
{
	int Error = 0;

	using b2 = glm::vec<2, bool, Q>;
	using b3 = glm::vec<3, bool, Q>;
	using b4 = glm::vec<4, bool, Q>;

	b4 A(true, false, false, true);
	Error += CHECK(b2(A.yx), false, true);
	Error += CHECK(b3(A.wzy), true, false, false);
	A.zy = b2(true, true);
	Error += CHECK(A, true, true, true, true);

	return Error;
}

template<glm::qualifier Q>
static int test_all()
{
	int Error = 0;

	Error += test_read<float, Q>();
	Error += test_read<double, Q>();
	Error += test_read<int, Q>();
	Error += test_read<glm::uint, Q>();
	Error += test_write<float, Q>();
	Error += test_write<double, Q>();
	Error += test_write<int, Q>();
	Error += test_write<glm::uint, Q>();
	Error += test_bool<Q>();

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_all<glm::packed_highp>());
	Error += GLM_TEST(test_all<glm::aligned_highp>());
	Error += GLM_TEST(test_all<glm::aligned_mediump>());
	Error += GLM_TEST(test_all<glm::aligned_lowp>());

	return GLM_TEST_FINISH(Error);
}

#else

int main()
{
	return GLM_TEST_SKIP_ALL("swizzle operators or aligned gentypes are disabled");
}

#endif
