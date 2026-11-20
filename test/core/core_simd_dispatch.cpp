// Differential test of the SIMD-dispatched operations.
//
// Operations on aligned types go through the SSE/NEON/Clang specializations when an instruction set
// is enabled; the same operations on packed_highp types always take the scalar path. Every operation
// here is computed both ways from the same inputs and the results must agree (exactly for integers,
// within a precision-dependent tolerance for floating point, since aligned_mediump/lowp may use
// reciprocal approximations).
//
// Aligned 3-component vectors are padded to 4 lanes. The inputs are built with NaN/garbage in that
// padding lane, so a SIMD path that lets it leak into a result (e.g. comparisons or horizontal sums
// over all 4 lanes) fails here.

#ifndef GLM_FORCE_ALIGNED_GENTYPES
#	define GLM_FORCE_ALIGNED_GENTYPES
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <type_traits>

#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE

namespace
{
	int Failures = 0;
	char const* Context = "";

	std::mt19937 Rng(20261119u);

	template<typename T>
	T random_value(double lo, double hi)
	{
		if constexpr (std::is_floating_point<T>::value)
			return static_cast<T>(std::uniform_real_distribution<double>(lo, hi)(Rng));
		else
			return static_cast<T>(std::uniform_int_distribution<long long>(static_cast<long long>(lo), static_cast<long long>(hi))(Rng));
	}

	// A packed vector of random components; Lo/Hi bound them and NonZero keeps them away from zero.
	template<glm::length_t L, typename T>
	glm::vec<L, T, glm::packed_highp> random_vec(double lo, double hi, bool nonZero = false)
	{
		glm::vec<L, T, glm::packed_highp> v;
		for (glm::length_t i = 0; i < L; ++i)
		{
			do
				v[i] = random_value<T>(lo, hi);
			while (nonZero && v[i] == T(0));
		}
		return v;
	}

	template<glm::length_t C, glm::length_t R, typename T>
	glm::mat<C, R, T, glm::packed_highp> random_mat(double lo, double hi)
	{
		glm::mat<C, R, T, glm::packed_highp> m;
		for (glm::length_t c = 0; c < C; ++c)
			m[c] = random_vec<R, T>(lo, hi);
		return m;
	}

	// The aligned counterpart of a packed vector, with the padding lane (if any) set to garbage.
	template<glm::qualifier Q, glm::length_t L, typename T>
	glm::vec<L, T, Q> to_aligned_dirty(glm::vec<L, T, glm::packed_highp> const& v)
	{
		using A = glm::vec<L, T, Q>;
		static_assert(std::is_trivially_copyable<A>::value, "aligned vectors are trivially copyable");
		// Convert normally, then overwrite only the padding bytes (if any). Building the whole vector from a
		// byte buffer instead made GCC 13 route part of a 12-byte copy through an MMX register without EMMS,
		// which poisons later x87 (long double) math in the test's random number generation.
		A Result(v);
		constexpr std::size_t Used = sizeof(T) * L;
		if constexpr (sizeof(A) > Used)
		{
			unsigned char Garbage[sizeof(A) - Used];
			if constexpr (std::is_floating_point<T>::value)
			{
				T const Nan = std::numeric_limits<T>::quiet_NaN();
				for (std::size_t i = 0; i + sizeof(T) <= sizeof(Garbage); i += sizeof(T))
					std::memcpy(Garbage + i, &Nan, sizeof(T));
			}
			else
				std::memset(Garbage, 0xA5, sizeof(Garbage));
			std::memcpy(reinterpret_cast<unsigned char*>(&Result) + Used, Garbage, sizeof(Garbage));
		}
		return Result;
	}

	template<glm::qualifier Q, glm::length_t C, glm::length_t R, typename T>
	glm::mat<C, R, T, Q> to_aligned_dirty(glm::mat<C, R, T, glm::packed_highp> const& m)
	{
		glm::mat<C, R, T, Q> Result;
		for (glm::length_t c = 0; c < C; ++c)
			Result[c] = to_aligned_dirty<Q>(m[c]);
		return Result;
	}

	template<glm::qualifier Q>
	double tolerance()
	{
		// mediump/lowp may use hardware reciprocal (square root) estimates with fewer refinement steps.
		if (glm::detail::is_highp<Q>::value)
			return 1e-5;
		return glm::detail::is_mediump<Q>::value ? 2e-3 : 5e-3;
	}

	template<typename T>
	bool close(T a, T b, double tol)
	{
		if constexpr (std::is_floating_point<T>::value)
		{
			double const Diff = std::abs(double(a) - double(b));
			return Diff <= tol * std::max(1.0, std::abs(double(b)));
		}
		else
			return a == b;
	}

	template<typename T>
	void report(char const* op, int index, T got, T expected)
	{
		std::printf("  FAIL %s: %s (component %d): got %.9g, expected %.9g\n", Context, op, index, double(got), double(expected));
		++Failures;
	}

	// Result comparisons: scalars, bools, vectors and matrices.
	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value>::type check(char const* op, T got, T expected, double tol)
	{
		if (!close(got, expected, tol))
			report(op, 0, got, expected);
	}

	template<glm::length_t L, typename T, glm::qualifier Q, glm::qualifier P>
	void check(char const* op, glm::vec<L, T, Q> const& got, glm::vec<L, T, P> const& expected, double tol)
	{
		for (glm::length_t i = 0; i < L; ++i)
			if (!close(got[i], expected[i], tol))
				return report(op, i, got[i], expected[i]);
	}

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q, glm::qualifier P>
	void check(char const* op, glm::mat<C, R, T, Q> const& got, glm::mat<C, R, T, P> const& expected, double tol)
	{
		for (glm::length_t c = 0; c < C; ++c)
			for (glm::length_t r = 0; r < R; ++r)
				if (!close(got[c][r], expected[c][r], tol))
					return report(op, c * R + r, got[c][r], expected[c][r]);
	}

	// Copy a vector and overwrite one component through the anonymous-struct members. With 3-lane
	// ext_vector storage, Clang/LLVM 18-21 and Apple clang from Xcode 16.3 to 26.3 miscompiled this
	// on AArch64 (the padding lane was poison, which poisoned the whole vector once it was handled as
	// an integer), so it's checked both on its own and on the result of an arithmetic operation.
	template<typename V>
	V copy_then_write(V const& c)
	{
		V h = c;
		h.z = c.x;
		return h;
	}

	// Run Op on the aligned inputs and on the packed ones, and compare.
#	define DIFF1(name, tol, expr) \
		do { auto Fn = [](auto const& a) { return expr; }; check(name, Fn(A), Fn(PA), tol); } while (false)
#	define DIFF2(name, tol, expr) \
		do { auto Fn = [](auto const& a, auto const& b) { return expr; }; check(name, Fn(A, B), Fn(PA, PB), tol); } while (false)
#	define DIFF3(name, tol, expr) \
		do { auto Fn = [](auto const& a, auto const& b, auto const& c) { return expr; }; check(name, Fn(A, B, C), Fn(PA, PB, PC), tol); } while (false)

	template<glm::length_t L, typename T, glm::qualifier Q>
	void test_vec_arith()
	{
		double const Tol = tolerance<Q>();
		for (int Iter = 0; Iter < 64; ++Iter)
		{
			auto const PA = random_vec<L, T>(-100, 100);
			auto const PB = random_vec<L, T>(-100, 100, true);
			auto const A = to_aligned_dirty<Q>(PA);
			auto const B = to_aligned_dirty<Q>(PB);
			T const S = PB[0];

			DIFF2("a + b", Tol, a + b);
			DIFF2("a - b", Tol, a - b);
			DIFF2("a * b", Tol, a * b);
			DIFF2("a / b", Tol, a / b);
			DIFF1("a + s", Tol, a + T(3));
			DIFF1("a * s", Tol, a * T(3));
			if constexpr (std::is_signed<T>::value)
				DIFF1("-a", Tol, -a);
			check("a / s", A / S, PA / S, Tol);

			DIFF2("a == b", 0, a == b);
			DIFF2("a != b", 0, a != b);
			DIFF1("a == a", 0, a == a);
			DIFF1("a != a", 0, a != a);
			// Equal in the real lanes, different in the padding lane.
			auto const A2 = to_aligned_dirty<Q>(PA);
			check("a == copy", A == A2, true, 0);
			check("a != copy", A != A2, false, 0);

			if constexpr (L >= 3)
			{
				// Built by the component constructor, which is what stored a poison padding lane.
				glm::vec<L, T, Q> C3(T(0));
				if constexpr (L == 3)
					C3 = glm::vec<L, T, Q>(PA[0], PA[1], PA[2]);
				else
					C3 = glm::vec<L, T, Q>(PA[0], PA[1], PA[2], PA[3]);
				check("copy then write", copy_then_write(C3), copy_then_write(PA), 0);
				DIFF2("copy then write (a + b)", Tol, copy_then_write(a + b));
			}

			DIFF2("min", Tol, glm::min(a, b));
			DIFF2("max", Tol, glm::max(a, b));
			{
				auto const PC = random_vec<L, T>(-100, 100);
				auto const C = to_aligned_dirty<Q>(PC);
				DIFF3("clamp", Tol, glm::clamp(a, glm::min(b, c), glm::max(b, c)));
			}

			if constexpr (std::is_signed<T>::value)
			{
				DIFF1("abs", Tol, glm::abs(a));
				DIFF1("sign", Tol, glm::sign(a));
			}

			if constexpr (std::is_integral<T>::value)
			{
				auto const Shift = random_vec<L, T>(0, 7);
				auto const ShiftA = to_aligned_dirty<Q>(Shift);
				DIFF2("a % b", 0, a % b);
				DIFF2("a & b", 0, a & b);
				DIFF2("a | b", 0, a | b);
				DIFF2("a ^ b", 0, a ^ b);
				DIFF1("~a", 0, ~a);
				check("a << n", A << ShiftA, PA << Shift, 0);
				check("a >> n", A >> ShiftA, PA >> Shift, 0);
				if constexpr (sizeof(T) == 4)
				{
					DIFF1("bitCount", 0, glm::bitCount(a));
					DIFF1("findLSB", 0, glm::findLSB(a));
					DIFF1("findMSB", 0, glm::findMSB(a));
					DIFF1("bitfieldReverse", 0, glm::bitfieldReverse(a));
				}
			}
		}
	}

	template<glm::length_t L, typename T, glm::qualifier Q>
	void test_vec_float()
	{
		double const Tol = tolerance<Q>();
		for (int Iter = 0; Iter < 64; ++Iter)
		{
			auto const PA = random_vec<L, T>(-8, 8);
			auto const PB = random_vec<L, T>(-8, 8, true);
			auto const PC = random_vec<L, T>(0, 1);
			auto const PPos = random_vec<L, T>(0.01, 16);
			auto const A = to_aligned_dirty<Q>(PA);
			auto const B = to_aligned_dirty<Q>(PB);
			auto const C = to_aligned_dirty<Q>(PC);

			DIFF1("floor", Tol, glm::floor(a));
			DIFF1("ceil", Tol, glm::ceil(a));
			DIFF1("trunc", Tol, glm::trunc(a));
			DIFF1("round", Tol, glm::round(a));
			DIFF1("roundEven", Tol, glm::roundEven(a));
			DIFF1("fract", Tol, glm::fract(a));
			DIFF2("mod", Tol, glm::mod(a, b));
			DIFF3("mix", Tol, glm::mix(a, b, c));
			DIFF2("step", Tol, glm::step(a, b));
			DIFF3("smoothstep", Tol, glm::smoothstep(glm::min(a, b), glm::max(a, b) + T(1), c * T(8) - T(4)));
			DIFF3("fma", Tol, glm::fma(a, b, c));

			{
				auto const Pos = to_aligned_dirty<Q>(PPos);
				check("sqrt", glm::sqrt(Pos), glm::sqrt(PPos), Tol);
				check("inversesqrt", glm::inversesqrt(Pos), glm::inversesqrt(PPos), Tol);
			}

			if constexpr (L >= 2)
			{
				DIFF1("length", Tol, glm::length(a));
				DIFF2("distance", Tol, glm::distance(a, b));
				DIFF2("dot", Tol, glm::dot(a, b));
				check("normalize", glm::normalize(B), glm::normalize(PB), Tol);
				DIFF3("faceforward", Tol, glm::faceforward(a, b, c));
				DIFF2("reflect", Tol, glm::reflect(a, glm::normalize(b)));
				DIFF2("refract", Tol, glm::refract(glm::normalize(a), glm::normalize(b), T(0.5)));
				// With eta = 4 many inputs sit near the total internal reflection threshold, where any
				// rounding difference flips the result, so only highp is compared.
				if (glm::detail::is_highp<Q>::value)
					DIFF2("refract (total internal reflection)", Tol, glm::refract(glm::normalize(a), glm::normalize(b), T(4)));
			}
			if constexpr (L == 3)
				DIFF2("cross", Tol, glm::cross(a, b));
		}

		if constexpr (L >= 2)
		{
			// dot(Nref, I) == 0 must give -N, not a zero vector.
			glm::vec<L, T, glm::packed_highp> PN(T(0)), PI(T(0)), PRef(T(0));
			PN[0] = T(1);
			PN[1] = T(2);
			PI[0] = T(1);
			PRef[1] = T(1);
			auto const N = to_aligned_dirty<Q>(PN);
			auto const I = to_aligned_dirty<Q>(PI);
			auto const Ref = to_aligned_dirty<Q>(PRef);
			check("faceforward (dot == 0)", glm::faceforward(N, I, Ref), -PN, 0);
			check("faceforward (dot == 0) reference", glm::faceforward(PN, PI, PRef), -PN, 0);
		}
	}

	template<glm::length_t L, glm::qualifier Q>
	void test_vec_conversion()
	{
		for (int Iter = 0; Iter < 16; ++Iter)
		{
			auto const PI = random_vec<L, int>(-1000, 1000);
			auto const PU = random_vec<L, glm::uint>(0, 2000);
			auto const PF = random_vec<L, float>(-1000, 1000);
			auto const PD = random_vec<L, double>(-1000, 1000);
			auto const I = to_aligned_dirty<Q>(PI);
			auto const U = to_aligned_dirty<Q>(PU);
			auto const F = to_aligned_dirty<Q>(PF);
			auto const D = to_aligned_dirty<Q>(PD);

			check("vec<float>(vec<int>)", glm::vec<L, float, Q>(I), glm::vec<L, float, glm::packed_highp>(PI), 0);
			check("vec<float>(vec<uint>)", glm::vec<L, float, Q>(U), glm::vec<L, float, glm::packed_highp>(PU), 0);
			check("vec<int>(vec<float>)", glm::vec<L, int, Q>(F), glm::vec<L, int, glm::packed_highp>(PF), 0);
			check("vec<float>(vec<double>)", glm::vec<L, float, Q>(D), glm::vec<L, float, glm::packed_highp>(PD), 0);
			check("vec<double>(vec<double, packed>)", glm::vec<L, double, Q>(PD), PD, 0);
			check("vec<float>(vec<float, packed>)", glm::vec<L, float, Q>(PF), PF, 0);
			check("vec<float>(float)", glm::vec<L, float, Q>(PF[0]), glm::vec<L, float, glm::packed_highp>(PF[0]), 0);
			check("vec<int64>(int64)", glm::vec<L, glm::int64, Q>(glm::int64(PI[0]) << 33), glm::vec<L, glm::int64, glm::packed_highp>(glm::int64(PI[0]) << 33), 0);
			if constexpr (L == 3)
			{
				check("vec3(int, int, int)", glm::vec<3, float, Q>(PI[0], PI[1], PI[2]), glm::vec<3, float, glm::packed_highp>(PI[0], PI[1], PI[2]), 0);
				check("vec3(uint, uint, uint)", glm::vec<3, float, Q>(PU[0], PU[1], PU[2]), glm::vec<3, float, glm::packed_highp>(PU[0], PU[1], PU[2]), 0);
			}
			if constexpr (L == 4)
			{
				check("vec4(int, int, int, int)", glm::vec<4, float, Q>(PI[0], PI[1], PI[2], PI[3]), glm::vec<4, float, glm::packed_highp>(PI[0], PI[1], PI[2], PI[3]), 0);
				check("vec4(uint, uint, uint, uint)", glm::vec<4, float, Q>(PU[0], PU[1], PU[2], PU[3]), glm::vec<4, float, glm::packed_highp>(PU[0], PU[1], PU[2], PU[3]), 0);
			}
		}
	}

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	void test_mat()
	{
		double const Tol = tolerance<Q>();
		for (int Iter = 0; Iter < 32; ++Iter)
		{
			auto const PA = random_mat<C, R, T>(-4, 4);
			auto const PB = random_mat<C, R, T>(-4, 4);
			auto const PT = random_mat<R, C, T>(-4, 4);
			auto const PV = random_vec<C, T>(-4, 4);
			auto const PW = random_vec<R, T>(-4, 4);
			auto const A = to_aligned_dirty<Q>(PA);
			auto const B = to_aligned_dirty<Q>(PB);
			auto const Tr = to_aligned_dirty<Q>(PT);
			auto const V = to_aligned_dirty<Q>(PV);
			auto const W = to_aligned_dirty<Q>(PW);

			DIFF2("m + m", Tol, a + b);
			DIFF2("m - m", Tol, a - b);
			DIFF1("m * s", Tol, a * T(3));
			DIFF1("-m", Tol, -a);
			DIFF1("transpose", Tol, glm::transpose(a));
			DIFF2("matrixCompMult", Tol, glm::matrixCompMult(a, b));
			check("m * v", A * V, PA * PV, Tol * 16);
			check("v * m", W * A, PW * PA, Tol * 16);
			check("m * m", A * Tr, PA * PT, Tol * 16);
			check("outerProduct", glm::outerProduct(W, V), glm::outerProduct(PW, PV), Tol);
			DIFF2("m == m", 0, a == b);
			DIFF1("m == self", 0, a == a);

			if constexpr (C == R)
			{
				DIFF1("determinant", Tol * 64, glm::determinant(a));
				// Keep the inverse well conditioned.
				auto const PI = PA + glm::mat<C, R, T, glm::packed_highp>(T(16));
				auto const I = to_aligned_dirty<Q>(PI);
				check("inverse", glm::inverse(I), glm::inverse(PI), Tol * 16);
			}
		}
	}

	template<glm::qualifier Q>
	void test_qualifier(char const* Name)
	{
		static char Buffer[128];

#		define RUN(test, ...) \
			do { std::snprintf(Buffer, sizeof(Buffer), "%s %s<%s>", Name, #test, #__VA_ARGS__); Context = Buffer; \
				test<__VA_ARGS__, Q>(); } while (false)

		RUN(test_vec_arith, 1, float); RUN(test_vec_arith, 2, float); RUN(test_vec_arith, 3, float); RUN(test_vec_arith, 4, float);
		RUN(test_vec_arith, 1, double); RUN(test_vec_arith, 2, double); RUN(test_vec_arith, 3, double); RUN(test_vec_arith, 4, double);
		RUN(test_vec_arith, 1, int); RUN(test_vec_arith, 2, int); RUN(test_vec_arith, 3, int); RUN(test_vec_arith, 4, int);
		RUN(test_vec_arith, 1, glm::uint); RUN(test_vec_arith, 2, glm::uint); RUN(test_vec_arith, 3, glm::uint); RUN(test_vec_arith, 4, glm::uint);
		RUN(test_vec_arith, 2, glm::int64); RUN(test_vec_arith, 3, glm::int64); RUN(test_vec_arith, 4, glm::int64);
		RUN(test_vec_arith, 2, glm::uint64); RUN(test_vec_arith, 3, glm::uint64); RUN(test_vec_arith, 4, glm::uint64);

		RUN(test_vec_float, 1, float); RUN(test_vec_float, 2, float); RUN(test_vec_float, 3, float); RUN(test_vec_float, 4, float);
		RUN(test_vec_float, 1, double); RUN(test_vec_float, 2, double); RUN(test_vec_float, 3, double); RUN(test_vec_float, 4, double);

		RUN(test_vec_conversion, 2); RUN(test_vec_conversion, 3); RUN(test_vec_conversion, 4);

		RUN(test_mat, 2, 2, float); RUN(test_mat, 2, 3, float); RUN(test_mat, 2, 4, float);
		RUN(test_mat, 3, 2, float); RUN(test_mat, 3, 3, float); RUN(test_mat, 3, 4, float);
		RUN(test_mat, 4, 2, float); RUN(test_mat, 4, 3, float); RUN(test_mat, 4, 4, float);
		RUN(test_mat, 2, 2, double); RUN(test_mat, 3, 3, double); RUN(test_mat, 4, 4, double);
		RUN(test_mat, 4, 3, double);

#		undef RUN
	}
}//namespace

int main()
{
	test_qualifier<glm::aligned_highp>("aligned_highp");
	test_qualifier<glm::aligned_mediump>("aligned_mediump");
	test_qualifier<glm::aligned_lowp>("aligned_lowp");

	std::printf(Failures == 0 ? "PASSED\n" : "FAILED (%d)\n", Failures);
	return Failures;
}

#else

int main()
{
	std::printf("SKIPPED (aligned gentypes disabled)\n");
	return 0;
}

#endif
