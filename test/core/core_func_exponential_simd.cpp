// Regression test for the vectorized SIMD exp/log/exp2/log2 path (aligned qualifiers).
// Forces aligned gentypes + GLM_FORCE_SSE42 so glm::exp/log/exp2/log2(vec) dispatch
// to the SIMD specializations in func_exponential_simd.inl. highp defers to libm;
// mediump/lowp use 4-wide SIMD (rel err ~1e-7). Ground truth = double libm.
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#define GLM_FORCE_SSE42 1

#include <glm/glm.hpp>
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>
#include <cmath>
#include <cstdio>
#include <random>

namespace {

template<glm::qualifier Q, class Fork, class Ref>
int check(const char* name, Fork fork, Ref ref, double lo, double hi, double tol)
{
	std::mt19937 rng(20260725u);
	std::uniform_real_distribution<double> d(lo, hi);
	double maxrel = 0.0;
	for (int i = 0; i < 100000; ++i)
	{
		const float in[4] = { (float)d(rng), (float)d(rng), (float)d(rng), (float)d(rng) };
		glm::vec<4, float, Q> v(in[0], in[1], in[2], in[3]);
		glm::vec<4, float, Q> o = fork(v);
		for (int j = 0; j < 4; ++j)
		{
			double r = ref((double)in[j]);
			if (std::isfinite((double)o[j]) && r != 0.0)
				maxrel = std::max(maxrel, std::fabs((double)o[j] - r) / std::fabs(r));
		}
	}
	if (maxrel > tol) { std::printf("  [FAIL] %s relerr=%.3e tol=%.1e\n", name, maxrel, tol); return 1; }
	std::printf("  [ ok ] %-24s relerr=%.2e\n", name, maxrel);
	return 0;
}

template<glm::qualifier Q>
int test_qualifier(const char* q)
{
	const double tol = glm::detail::is_highp<Q>::value ? 1e-6 : 5e-6;
	int E = 0;
	char nm[64];
	std::snprintf(nm, sizeof nm, "exp  %s", q);  E += check<Q>(nm, [](auto v){ return glm::exp(v); },  [](double x){ return std::exp(x); },  -30.0, 30.0, tol);
	std::snprintf(nm, sizeof nm, "log  %s", q);  E += check<Q>(nm, [](auto v){ return glm::log(v); },  [](double x){ return std::log(x); },   1e-6, 1e6,  tol);
	std::snprintf(nm, sizeof nm, "exp2 %s", q);  E += check<Q>(nm, [](auto v){ return glm::exp2(v); }, [](double x){ return std::exp2(x); }, -40.0, 40.0, tol);
	std::snprintf(nm, sizeof nm, "log2 %s", q);  E += check<Q>(nm, [](auto v){ return glm::log2(v); }, [](double x){ return std::log2(x); },  1e-6, 1e6,  tol);
	// hyperbolics (built on exp/log; cancellation-aware forms target ~1e-6)
	std::snprintf(nm, sizeof nm, "sinh %s", q);  E += check<Q>(nm, [](auto v){ return glm::sinh(v); },  [](double x){ return std::sinh(x); },  -10.0, 10.0, tol);
	std::snprintf(nm, sizeof nm, "cosh %s", q);  E += check<Q>(nm, [](auto v){ return glm::cosh(v); },  [](double x){ return std::cosh(x); },  -10.0, 10.0, tol);
	std::snprintf(nm, sizeof nm, "tanh %s", q);  E += check<Q>(nm, [](auto v){ return glm::tanh(v); },  [](double x){ return std::tanh(x); },  -10.0, 10.0, tol);
	std::snprintf(nm, sizeof nm, "asinh %s", q); E += check<Q>(nm, [](auto v){ return glm::asinh(v); }, [](double x){ return std::asinh(x); }, -100.0, 100.0, tol);
	std::snprintf(nm, sizeof nm, "acosh %s", q); E += check<Q>(nm, [](auto v){ return glm::acosh(v); }, [](double x){ return std::acosh(x); },  1.0, 100.0, tol);
	std::snprintf(nm, sizeof nm, "atanh %s", q); E += check<Q>(nm, [](auto v){ return glm::atanh(v); }, [](double x){ return std::atanh(x); }, -0.999, 0.999, tol);
	return E;
}

} // namespace

int main()
{
	std::printf("=== SIMD aligned exp/log/exp2/log2 regression ===\n");
	int Error = 0;
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	Error += test_qualifier<glm::aligned_highp>("highp");
	Error += test_qualifier<glm::aligned_mediump>("mediump");
	Error += test_qualifier<glm::aligned_lowp>("lowp");
	std::printf(Error == 0 ? "ALL PASSED\n" : "FAILED (%d)\n", Error);
#else
	std::printf("SKIPPED (aligned gentypes not enabled)\n");
#endif
	return Error;
}
