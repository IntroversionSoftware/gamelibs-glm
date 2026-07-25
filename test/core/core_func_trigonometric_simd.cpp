// Regression test for the vectorized SIMD sin/cos/tan path (aligned qualifiers).
// Forces aligned default gentypes so glm::sin/cos/tan(vec) dispatch to the SIMD
// specialization in func_trigonometric_simd.inl. Covers large angles (range
// reduction) and near-asymptote tan, which the small-angle scalar tests do not.
// Ground truth = double-precision libm.
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1
#define GLM_FORCE_SSE42 1   // enable the SIMD path (fork requires an explicit GLM_FORCE_*)

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <cmath>
#include <cstdio>
#include <random>

namespace {

template<glm::qualifier Q>
int test_qualifier(const char* qname)
{
	int Error = 0;
	std::mt19937 rng(20260725u);
	const bool highp = glm::detail::is_highp<Q>::value;

	// Tolerances: highp defers to libm (~1e-6); mediump/lowp use the SIMD poly
	// (measured ~3.6e-6 up to 1e4, ~1.4e-5 at 1e6). Bounds are generous vs the
	// measured error so they catch regressions without being flaky.
	auto tol = [&](double mag) -> double {
		if (highp) return 1e-6;
		return (mag <= 1e4) ? 5e-5 : 5e-4;
	};

	const double ranges[] = { 3.14159, 100.0, 1.0e4, 1.0e6 };
	for (double hi : ranges)
	{
		std::uniform_real_distribution<double> d(-hi, hi);
		double maxs = 0.0, maxc = 0.0;
		for (int i = 0; i < 50000; ++i)
		{
			const float in[4] = { (float)d(rng), (float)d(rng), (float)d(rng), (float)d(rng) };
			glm::vec<4, float, Q> v(in[0], in[1], in[2], in[3]);
			glm::vec<4, float, Q> s = glm::sin(v);
			glm::vec<4, float, Q> c = glm::cos(v);
			for (int j = 0; j < 4; ++j)
			{
				maxs = std::max(maxs, std::abs((double)s[j] - std::sin((double)in[j])));
				maxc = std::max(maxc, std::abs((double)c[j] - std::cos((double)in[j])));
			}
		}
		if (maxs > tol(hi) || maxc > tol(hi))
		{
			std::printf("  [FAIL] %s sin/cos |x|<%g: maxerr sin=%.3e cos=%.3e tol=%.1e\n",
				qname, hi, maxs, maxc, tol(hi));
			++Error;
		}
		else
		{
			std::printf("  [ ok ] %s sin/cos |x|<%-7g maxerr sin=%.2e cos=%.2e\n", qname, hi, maxs, maxc);
		}
	}

	// tan near pi/2 must track the true value (regression guard against the old
	// |cos| < 1e-3 -> +-1e6 clamp).
	const double halfpi = 1.5707963267948966;
	for (double off : { 1e-1, 1e-2, 1e-3, 1e-4 })
	{
		float x = (float)(halfpi - off);
		glm::vec<4, float, Q> v(x, x, x, x);
		float t = glm::tan(v).x;
		double ref = std::tan((double)x);
		double rel = std::abs(((double)t - ref) / ref);
		double rtol = highp ? 1e-5 : 5e-3;
		if (rel > rtol)
		{
			std::printf("  [FAIL] %s tan(pi/2 - %g) = %.6e ref %.6e relerr %.2e\n",
				qname, off, (double)t, ref, rel);
			++Error;
		}
	}
	// inverse trig: asin/acos over [-1,1], atan over a wide range (abs error)
	{
		double ea = 0, es = 0, ec = 0, at = highp ? 1e-6 : 5e-6;
		std::mt19937 r2(99u);
		std::uniform_real_distribution<double> d1(-1.0, 1.0), d2(-50.0, 50.0);
		for (int i = 0; i < 50000; ++i)
		{
			const float u[4] = { (float)d1(r2), (float)d1(r2), (float)d1(r2), (float)d1(r2) };
			const float w[4] = { (float)d2(r2), (float)d2(r2), (float)d2(r2), (float)d2(r2) };
			glm::vec<4, float, Q> vu(u[0],u[1],u[2],u[3]), vw(w[0],w[1],w[2],w[3]);
			glm::vec<4, float, Q> as = glm::asin(vu), ac = glm::acos(vu), at_ = glm::atan(vw);
			for (int j = 0; j < 4; ++j) {
				es = std::max(es, std::abs((double)as[j] - std::asin((double)u[j])));
				ec = std::max(ec, std::abs((double)ac[j] - std::acos((double)u[j])));
				ea = std::max(ea, std::abs((double)at_[j] - std::atan((double)w[j])));
			}
		}
		if (es > at || ec > at || ea > at) {
			std::printf("  [FAIL] %s asin=%.2e acos=%.2e atan=%.2e tol=%.1e\n", qname, es, ec, ea, at);
			++Error;
		} else {
			std::printf("  [ ok ] %s asin=%.2e acos=%.2e atan=%.2e\n", qname, es, ec, ea);
		}
	}
	return Error;
}

} // namespace

int main()
{
	std::printf("=== SIMD aligned sin/cos/tan regression ===\n");
	int Error = 0;
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	Error += test_qualifier<glm::aligned_highp>("aligned_highp");
	Error += test_qualifier<glm::aligned_mediump>("aligned_mediump");
	Error += test_qualifier<glm::aligned_lowp>("aligned_lowp");
	std::printf(Error == 0 ? "ALL PASSED\n" : "FAILED (%d)\n", Error);
#else
	std::printf("SKIPPED (aligned gentypes not enabled)\n");
#endif
	return Error;
}
