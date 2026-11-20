// Regression test for the vectorized SIMD exp/log/exp2/log2 path (aligned qualifiers).
// Forces aligned gentypes + GLM_FORCE_SSE42 so glm::exp/log/exp2/log2(vec) dispatch
// to the SIMD specializations in func_exponential_simd.inl. highp defers to libm;
// mediump/lowp use 4-wide SIMD (rel err ~1e-7). Ground truth = double libm.
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#	define GLM_FORCE_SSE42 1
#endif

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>
#include <cmath>
#include <limits>
#include <random>
#include <string>

// Without the vectorized path (GLM_FORCE_PURE, no FMA, non-x86) mediump/lowp use the scalar
// reduced-precision implementations, which these tolerances are not meant for.
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE && GLM_CONFIG_SIMD == GLM_ENABLE && GLM_HAS_FMA && ((GLM_ARCH & GLM_ARCH_SSE2_BIT) || (GLM_ARCH & GLM_ARCH_CLANG_BIT))

namespace {

// Largest relative error of a vectorized function against double-precision libm over [lo, hi].
template<glm::qualifier Q, class Fork, class Ref>
double max_relative_error(Fork fork, Ref ref, double lo, double hi)
{
	std::mt19937 rng(20260725u);
	std::uniform_real_distribution<double> d(lo, hi);
	double maxrel = 0.0;
	for (int i = 0; i < 100000; ++i)
	{
		float in[4];
		for (float& f : in) f = glm_test::opaque((float)d(rng));
		glm::vec<4, float, Q> v(in[0], in[1], in[2], in[3]);
		glm::vec<4, float, Q> o = fork(v);
		for (int j = 0; j < 4; ++j)
		{
			double r = ref((double)in[j]);
			if (r != 0.0)
				maxrel = std::max(maxrel, std::fabs((double)o[j] - r) / std::fabs(r));
		}
	}
	return maxrel;
}

// pow(base>0, exp) = exp(exp*log(base))
template<glm::qualifier Q>
double max_relative_error_pow()
{
	std::mt19937 r2(7u);
	std::uniform_real_distribution<double> b(0.01, 20.0), ex(-4.0, 4.0);
	double maxrel = 0.0;
	for (int i = 0; i < 100000; ++i)
	{
		float bb[4];
		for (float& f : bb) f = glm_test::opaque((float)b(r2));
		float ee[4];
		for (float& f : ee) f = glm_test::opaque((float)ex(r2));
		glm::vec<4,float,Q> B(bb[0],bb[1],bb[2],bb[3]), E2(ee[0],ee[1],ee[2],ee[3]);
		glm::vec<4,float,Q> o = glm::pow(B, E2);
		for (int j = 0; j < 4; ++j)
		{
			double r = std::pow((double)bb[j],(double)ee[j]);
			if (r != 0.0)
				maxrel = std::max(maxrel, std::fabs((double)o[j]-r)/std::fabs(r));
		}
	}
	return maxrel;
}

template<glm::qualifier Q>
void test_qualifier(const char* q)
{
	const double tol = glm::detail::is_highp<Q>::value ? 1e-6 : 5e-6;

	auto run = [&](const char* fn, auto&& test)
	{
		std::string const Name = std::string(fn) + "<" + q + ">";
		glm_test::run(Name.c_str(), test);
	};

#	define EXP_SIMD_CHECK(fn, lo, hi) \
		run(#fn, [&] { \
			double const maxrel = max_relative_error<Q>([](auto v){ return glm::fn(v); }, [](double x){ return std::fn(x); }, lo, hi); \
			return GLM_CHECK_LE(maxrel, tol); \
		})

	// The mediump/lowp forms of these rely on exact rounding (expm1/log1p corrections against
	// cancellation), which fast-math may simplify away; highp defers to libm.
#	define EXP_SIMD_CHECK_COMPENSATED(fn, lo, hi) \
		run(#fn, [&] { \
			if constexpr (!glm::detail::is_highp<Q>::value) \
				GLM_TEST_REQUIRE_STRICT_FP(); \
			double const maxrel = max_relative_error<Q>([](auto v){ return glm::fn(v); }, [](double x){ return std::fn(x); }, lo, hi); \
			return GLM_CHECK_LE(maxrel, tol); \
		})

	EXP_SIMD_CHECK(exp, -30.0, 30.0);
	EXP_SIMD_CHECK(log, 1e-6, 1e6);
	EXP_SIMD_CHECK(exp2, -40.0, 40.0);
	EXP_SIMD_CHECK(log2, 1e-6, 1e6);
	// hyperbolics (built on exp/log; cancellation-aware forms target ~1e-6)
	EXP_SIMD_CHECK_COMPENSATED(sinh, -10.0, 10.0);
	EXP_SIMD_CHECK(cosh, -10.0, 10.0);
	EXP_SIMD_CHECK(tanh, -10.0, 10.0);
	EXP_SIMD_CHECK_COMPENSATED(asinh, -100.0, 100.0);
	EXP_SIMD_CHECK_COMPENSATED(acosh, 1.0, 100.0);
	EXP_SIMD_CHECK_COMPENSATED(atanh, -0.999, 0.999);
#	undef EXP_SIMD_CHECK
#	undef EXP_SIMD_CHECK_COMPENSATED

	run("pow", [&] {
		double const maxrel = max_relative_error_pow<Q>();
		return GLM_CHECK_LE(maxrel, tol);
	});

	// The special cases follow libm: log(0) = -inf, log(+inf) = +inf, NaN for a negative or NaN
	// input, and a denormal is logged like any other value.
	run("log special values", [&] {
		GLM_TEST_REQUIRE_IEEE_SPECIALS();
		int Error = 0;
		float const Inf = std::numeric_limits<float>::infinity();
		float const Denormal = glm_test::opaque(1e-40f);
		glm::vec<4, float, Q> const L = glm::log(glm::vec<4, float, Q>(0.0f, Inf, Denormal, 1.0f));
		Error += GLM_CHECK_EQ(L[0], -Inf);
		Error += GLM_CHECK_EQ(L[1], Inf);
		Error += GLM_CHECK_LE(std::fabs(static_cast<double>(L[2]) - std::log(static_cast<double>(Denormal))), 1e-4);
		Error += GLM_CHECK_EQ(L[3], 0.0f);
		glm::vec<4, float, Q> const N(-1.0f, -Inf, std::numeric_limits<float>::quiet_NaN(), -Denormal);
		Error += GLM_CHECK(glm::all(glm::isnan(glm::log(N))));
		Error += GLM_CHECK(glm::all(glm::isnan(glm::log2(N))));
		return Error;
	});
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
