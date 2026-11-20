// Regression test for the vectorized SIMD sin/cos/tan path (aligned qualifiers).
// Forces aligned default gentypes so glm::sin/cos/tan(vec) dispatch to the SIMD
// specialization in func_trigonometric_simd.inl. Covers large angles (range
// reduction) and near-asymptote tan, which the small-angle scalar tests do not.
// Ground truth = double-precision libm.
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#	define GLM_FORCE_SSE42 1   // enable the SIMD path (fork requires an explicit GLM_FORCE_*)
#endif

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <cmath>
#include <cstdio>
#include <random>
#include <string>

// Without the vectorized path (GLM_FORCE_PURE, no FMA, non-x86) mediump/lowp use the scalar
// reduced-precision implementations, which these tolerances are not meant for.
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE && GLM_CONFIG_SIMD == GLM_ENABLE && GLM_HAS_FMA && ((GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT))

namespace {

// sin/cos over |x| < Range, as absolute error against double-precision libm.
template<glm::qualifier Q>
int test_sin_cos(double Range)
{
	int Error = 0;
	std::mt19937 rng(20260725u);

	// Tolerances: highp defers to libm (~1e-6); mediump/lowp use the SIMD poly
	// (measured ~3.6e-6 up to 1e4, ~1.4e-5 at 1e6). Bounds are generous vs the
	// measured error so they catch regressions without being flaky.
	double const Tolerance = glm::detail::is_highp<Q>::value ? 1e-6 : (Range <= 1e4 ? 5e-5 : 5e-4);

	std::uniform_real_distribution<double> d(-Range, Range);
	double SinError = 0.0, CosError = 0.0;
	for (int i = 0; i < 50000; ++i)
	{
		float in[4];
		for (float& f : in) f = glm_test::opaque((float)d(rng));
		glm::vec<4, float, Q> v(in[0], in[1], in[2], in[3]);
		glm::vec<4, float, Q> s = glm::sin(v);
		glm::vec<4, float, Q> c = glm::cos(v);
		for (int j = 0; j < 4; ++j)
		{
			SinError = std::max(SinError, std::abs((double)s[j] - std::sin((double)in[j])));
			CosError = std::max(CosError, std::abs((double)c[j] - std::cos((double)in[j])));
		}
	}

	Error += GLM_CHECK_LE(SinError, Tolerance);
	Error += GLM_CHECK_LE(CosError, Tolerance);
	return Error;
}

// tan near pi/2 must track the true value (regression guard against the old
// |cos| < 1e-3 -> +-1e6 clamp).
template<glm::qualifier Q>
int test_tan_near_half_pi(double Offset)
{
	double const Tolerance = glm::detail::is_highp<Q>::value ? 1e-5 : 5e-3;

	float const x = glm_test::opaque((float)(1.5707963267948966 - Offset));
	glm::vec<4, float, Q> v(x, x, x, x);
	double const Result = glm::tan(v).x;
	double const Reference = std::tan((double)x);
	double const RelativeError = std::abs((Result - Reference) / Reference);

	return GLM_CHECK_LE(RelativeError, Tolerance);
}

// inverse trig: asin/acos over [-1,1], atan over a wide range (abs error)
template<glm::qualifier Q>
int test_inverse()
{
	int Error = 0;
	double const Tolerance = glm::detail::is_highp<Q>::value ? 1e-6 : 5e-6;

	double AsinError = 0, AcosError = 0, AtanError = 0, Atan2Error = 0;
	std::mt19937 r2(99u);
	std::uniform_real_distribution<double> d1(-1.0, 1.0), d2(-50.0, 50.0);
	for (int i = 0; i < 50000; ++i)
	{
		float u[4];
		for (float& f : u) f = glm_test::opaque((float)d1(r2));
		float w[4];
		for (float& f : w) f = glm_test::opaque((float)d2(r2));
		float w2[4];
		for (float& f : w2) f = glm_test::opaque((float)d2(r2));
		glm::vec<4, float, Q> vu(u[0],u[1],u[2],u[3]), vw(w[0],w[1],w[2],w[3]), vw2(w2[0],w2[1],w2[2],w2[3]);
		glm::vec<4, float, Q> as = glm::asin(vu), ac = glm::acos(vu), at = glm::atan(vw);
		glm::vec<4, float, Q> a2 = glm::atan(vw, vw2); // atan2(y, x), all quadrants
		for (int j = 0; j < 4; ++j)
		{
			AsinError = std::max(AsinError, std::abs((double)as[j] - std::asin((double)u[j])));
			AcosError = std::max(AcosError, std::abs((double)ac[j] - std::acos((double)u[j])));
			AtanError = std::max(AtanError, std::abs((double)at[j] - std::atan((double)w[j])));
			Atan2Error = std::max(Atan2Error, std::abs((double)a2[j] - std::atan2((double)w[j], (double)w2[j])));
		}
	}

	Error += GLM_CHECK_LE(AsinError, Tolerance);
	Error += GLM_CHECK_LE(AcosError, Tolerance);
	Error += GLM_CHECK_LE(AtanError, Tolerance);
	Error += GLM_CHECK_LE(Atan2Error, Tolerance);
	return Error;
}

template<glm::qualifier Q>
void test_qualifier(const char* q)
{
	char Name[96];

	for (double Range : { 3.14159, 100.0, 1.0e4, 1.0e6 })
	{
		std::snprintf(Name, sizeof(Name), "sin/cos<%s> |x| < %g", q, Range);
		glm_test::run(Name, [&] { return test_sin_cos<Q>(Range); });
	}

	for (double Offset : { 1e-1, 1e-2, 1e-3, 1e-4 })
	{
		std::snprintf(Name, sizeof(Name), "tan<%s>(pi/2 - %g)", q, Offset);
		glm_test::run(Name, [&] { return test_tan_near_half_pi<Q>(Offset); });
	}

	std::snprintf(Name, sizeof(Name), "asin/acos/atan/atan2<%s>", q);
	glm_test::run(Name, [&] { return test_inverse<Q>(); });
}

} // namespace

int main()
{
	test_qualifier<glm::aligned_highp>("aligned_highp");
	test_qualifier<glm::aligned_mediump>("aligned_mediump");
	test_qualifier<glm::aligned_lowp>("aligned_lowp");
	return GLM_TEST_FINISH();
}

#else

int main()
{
	return GLM_TEST_SKIP_ALL("vectorized transcendental path not compiled in");
}

#endif
