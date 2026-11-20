// GLM test harness.
//
// Checks report the file, line, expression and (for the comparison forms) both values when they
// fail; GLM_TEST runs one test function and prints a status line for it; GLM_TEST_FINISH prints a
// summary and returns the process exit code CTest reads:
//
//	0   everything passed (expected failures count as passing)
//	1   a check or test failed, or an expected failure unexpectedly passed
//	77  every test was skipped (CTest reports the test as skipped; see SKIP_RETURN_CODE)
//
// Output, on stdout and flushed as it happens so a crash still leaves the diagnostics:
//
//	      core_type_vec3.cpp:341: GLM_CHECK_NEAR(A.zyx, B, 0.001f) failed
//	        A.zyx = (3, 2, 1)
//	        B     = (3, 2, 4)
//	  FAIL   test_vec3_swizzle()
//	  PASS   test_vec3_ctor()
//	  SKIP   test_vec3_simd()  no SIMD path compiled in
//	core_type_vec3: 1 passed, 1 failed, 1 skipped (412 checks)
//
// This header includes only standard headers, never GLM, so it can't change a test's GLM_FORCE_*
// configuration and can go anywhere in the file. GLM values are printed by shape (vectors and
// matrices through length() and operator[], quaternions through wxyz), and GLM_CHECK_NEAR calls
// glm::equal at the check's own site, with whatever overloads the test has included.
//
// Usage:
//
//	static int test_foo()
//	{
//		int Error = 0;
//		Error += GLM_CHECK(a == b);                 // any boolean expression
//		Error += GLM_CHECK_EQ(A, B);                // A == B, printing both on failure
//		Error += GLM_CHECK_LE(MaxError, Tolerance); // also _NE, _LT, _GT, _GE
//		Error += GLM_CHECK_NEAR(A, B, 0.001f);      // glm::all(glm::equal(A, B, eps)), or the scalar form
//		return Error;
//	}
//
//	int main()
//	{
//		int Error = 0;
//		Error += GLM_TEST(test_foo());
//		Error += GLM_TEST_XFAIL(Condition, "reason", test_bar());
//		return GLM_TEST_FINISH(Error);
//	}
//
// Arguments to the GLM_CHECK_EQ/NEAR macros that contain a top-level comma (template arguments)
// need their own parentheses: GLM_CHECK_EQ((glm::vec<3, int>(1)), B).

#pragma once

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>

// Built with assumptions that NaN and infinity don't exist, which makes checks of them meaningless.
#if (defined(__FINITE_MATH_ONLY__) && __FINITE_MATH_ONLY__) || defined(_M_FP_FAST)
#	define GLM_TEST_FINITE_MATH_ONLY 1
#else
#	define GLM_TEST_FINITE_MATH_ONLY 0
#endif

// Built with -ffast-math (or MSVC /fp:fast), which lets the compiler reassociate and simplify
// floating-point expressions. (-fassociative-math on its own can't be detected.)
#if defined(__FAST_MATH__) || defined(_M_FP_FAST)
#	define GLM_TEST_FAST_MATH 1
#else
#	define GLM_TEST_FAST_MATH 0
#endif

namespace glm_test
{
	struct state
	{
		char const* test = nullptr;    // running test, or null outside GLM_TEST
		int test_failed_checks = 0;
		char const* skip_reason = nullptr;

		int checks = 0;
		int failed_checks = 0;          // includes ones outside GLM_TEST
		int passed = 0;
		int failed = 0;
		int xfailed = 0;
		int xpassed = 0;
		int skipped = 0;
	};

	inline state& current()
	{
		static state State;
		return State;
	}

	// Returns Value through a volatile, so the optimizer can't see where it came from. For reference
	// computations under fast-math: without it, a compiler may use the double a float input was
	// rounded from (`(double)(float)x` becomes `x`), and the reference no longer matches the input.
	template<typename T>
	T opaque(T Value)
	{
		T volatile Copy = Value;
		return Copy;
	}

	inline char const* basename(char const* Path)
	{
		char const* Name = Path;
		for (char const* p = Path; *p; ++p)
			if (*p == '/' || *p == '\\')
				Name = p + 1;
		return Name;
	}

	namespace detail
	{
		template<typename T, typename = void>
		struct is_quat : std::false_type {};

		template<typename T>
		struct is_quat<T, std::void_t<decltype(T::wxyz(std::declval<typename T::value_type>(), std::declval<typename T::value_type>(),
			std::declval<typename T::value_type>(), std::declval<typename T::value_type>()))>> : std::true_type {};

		template<typename T, typename = void>
		struct is_indexable : std::false_type {};

		template<typename T>
		struct is_indexable<T, std::void_t<decltype(T::length()), decltype(std::declval<T const&>()[0])>> : std::true_type {};

		// One per type, so every format string is a literal the compiler can check.
		inline std::string print(double Value)
		{
			char Buffer[64];
			std::snprintf(Buffer, sizeof(Buffer), "%.9g", Value);
			return Buffer;
		}

		inline std::string print(long double Value)
		{
			char Buffer[64];
			std::snprintf(Buffer, sizeof(Buffer), "%.17Lg", Value);
			return Buffer;
		}

		inline std::string print(long long Value)
		{
			char Buffer[32];
			std::snprintf(Buffer, sizeof(Buffer), "%lld", Value);
			return Buffer;
		}

		inline std::string print(unsigned long long Value)
		{
			char Buffer[32];
			std::snprintf(Buffer, sizeof(Buffer), "%llu", Value);
			return Buffer;
		}

		inline std::string print(void const* Value)
		{
			char Buffer[32];
			std::snprintf(Buffer, sizeof(Buffer), "%p", Value);
			return Buffer;
		}

		template<typename T>
		std::string format(T const& Value)
		{
			if constexpr (std::is_same<T, bool>::value)
				return Value ? "true" : "false";
			else if constexpr (std::is_same<T, float>::value)
				return print(static_cast<double>(Value)); // float: 9 significant digits
			else if constexpr (std::is_floating_point<T>::value)
				return print(static_cast<long double>(Value));
			else if constexpr (std::is_integral<T>::value && std::is_signed<T>::value)
				return print(static_cast<long long>(Value));
			else if constexpr (std::is_integral<T>::value)
				return print(static_cast<unsigned long long>(Value));
			else if constexpr (std::is_enum<T>::value)
				return format(static_cast<typename std::underlying_type<T>::type>(Value));
			else if constexpr (std::is_pointer<T>::value)
				return print(static_cast<void const*>(Value));
			else if constexpr (is_quat<T>::value)
				return "quat(w=" + format(Value.w) + ", x=" + format(Value.x) + ", y=" + format(Value.y) + ", z=" + format(Value.z) + ")";
			else if constexpr (is_indexable<T>::value)
			{
				std::string Text = "(";
				for (decltype(T::length()) i = 0; i < T::length(); ++i)
					Text += (i ? ", " : "") + format(Value[i]);
				return Text + ")";
			}
			else
				return "<value>";
		}

		inline void report(char const* File, int Line, char const* What)
		{
			std::printf("      %s:%d: %s failed\n", basename(File), Line, What);
		}

		inline void value(char const* Name, int Width, std::string const& Text)
		{
			std::printf("        %-*s = %s\n", Width, Name, Text.c_str());
		}

		inline int record(bool Ok)
		{
			state& State = current();
			++State.checks;
			if (Ok)
				return 0;
			++State.failed_checks;
			++State.test_failed_checks;
			return 1;
		}
	}//namespace detail

	// A boolean check; returns 1 on failure so `Error += GLM_CHECK(...)` keeps counting.
	inline int check(bool Ok, char const* Expression, char const* File, int Line)
	{
		if (!Ok)
		{
			std::string What = std::string("GLM_CHECK(") + Expression + ")";
			detail::report(File, Line, What.c_str());
			std::fflush(stdout);
		}
		return detail::record(Ok);
	}

	// A check of two values, printing both (labelled NameA and NameB) on failure. What describes the
	// check in the failure line.
	template<typename A, typename B>
	int check_pair(bool Ok, std::string const& What, char const* NameA, A const& ValueA, char const* NameB, B const& ValueB, char const* File, int Line)
	{
		if (!Ok)
		{
			detail::report(File, Line, What.c_str());
			int const Width = static_cast<int>(std::max(std::strlen(NameA), std::strlen(NameB)));
			detail::value(NameA, Width, detail::format(ValueA));
			detail::value(NameB, Width, detail::format(ValueB));
			std::fflush(stdout);
		}
		return detail::record(Ok);
	}

	// A comparison check that prints both operands on failure.
	template<typename A, typename B>
	int check_values(bool Ok, char const* Macro, char const* NameA, char const* NameB, char const* Extra, A const& ValueA, B const& ValueB,
		char const* File, int Line)
	{
		std::string What = std::string(Macro) + "(" + NameA + ", " + NameB + (Extra ? std::string(", ") + Extra : std::string()) + ")";
		return check_pair(Ok, What, NameA, ValueA, NameB, ValueB, File, Line);
	}

	// glm::all() for the vector of bools glm::equal returns, or the bool itself for scalars.
	inline bool all_of(bool Value)
	{
		return Value;
	}

	template<typename V>
	bool all_of(V const& Value)
	{
		for (decltype(V::length()) i = 0; i < V::length(); ++i)
			if (!all_of(Value[i]))
				return false;
		return true;
	}

	// Skip the running test (from inside it): `return GLM_TEST_SKIP("reason");`.
	inline int skip(char const* Reason)
	{
		current().skip_reason = Reason;
		return 0;
	}

	// Run one test. Fn returns its error count (or nothing); either a nonzero count or a failed check
	// fails it. With Expected set, failing is expected (XFAIL) and passing is an error (XPASS).
	template<typename Fn>
	int run(char const* Name, Fn&& Function, bool ExpectFailure = false, char const* Reason = nullptr)
	{
		state& State = current();
		char const* const Outer = State.test;
		int const OuterFailedChecks = State.test_failed_checks;
		State.test = Name;
		State.test_failed_checks = 0;
		State.skip_reason = nullptr;

		int Returned = 0;
		if constexpr (std::is_void<decltype(Function())>::value)
			Function();
		else
			Returned = static_cast<int>(Function());

		bool const Failed = Returned != 0 || State.test_failed_checks != 0;
		int Result = 0;
		if (State.skip_reason && !Failed)
		{
			std::printf("  SKIP   %s  %s\n", Name, State.skip_reason);
			++State.skipped;
		}
		else if (ExpectFailure)
		{
			if (Failed)
			{
				std::printf("  XFAIL  %s  %s\n", Name, Reason ? Reason : "");
				++State.xfailed;
				// The failures were expected; don't let them fail the process.
				State.failed_checks -= State.test_failed_checks;
			}
			else
			{
				std::printf("  XPASS  %s  expected to fail: %s\n", Name, Reason ? Reason : "");
				++State.xpassed;
				Result = 1;
			}
		}
		else if (Failed)
		{
			if (Returned != 0 && State.test_failed_checks == 0)
				std::printf("  FAIL   %s  (returned %d)\n", Name, Returned);
			else
				std::printf("  FAIL   %s\n", Name);
			++State.failed;
			Result = 1;
		}
		else
		{
			std::printf("  PASS   %s\n", Name);
			++State.passed;
		}
		std::fflush(stdout);

		State.test = Outer;
		State.test_failed_checks = OuterFailedChecks;
		State.skip_reason = nullptr;
		return Result;
	}

	// Print the summary and return the exit code. LegacyErrors is any error count the caller kept
	// itself (e.g. `Error += x;` lines that aren't checks); a nonzero count fails the test too.
	inline int finish(char const* File, int LegacyErrors = 0)
	{
		state const& State = current();
		std::string Name = basename(File);
		std::string::size_type const Dot = Name.rfind('.');
		if (Dot != std::string::npos)
			Name.resize(Dot);

		bool const Failed = State.failed != 0 || State.xpassed != 0 || State.failed_checks != 0 || (LegacyErrors != 0 && State.failed == 0 && State.failed_checks == 0);
		bool const AllSkipped = !Failed && State.skipped != 0 && State.passed == 0 && State.xfailed == 0;

		std::printf("%s: %d passed, %d failed", Name.c_str(), State.passed, State.failed);
		if (State.xfailed)
			std::printf(", %d expected failure%s", State.xfailed, State.xfailed == 1 ? "" : "s");
		if (State.xpassed)
			std::printf(", %d unexpectedly passed", State.xpassed);
		if (State.skipped)
			std::printf(", %d skipped", State.skipped);
		std::printf(" (%d checks", State.checks);
		if (State.failed_checks)
			std::printf(", %d failed", State.failed_checks);
		std::printf(")");
		if (LegacyErrors != 0 && State.failed == 0 && State.failed_checks == 0)
			std::printf("; error count %d", LegacyErrors);
		std::printf("\n");
		std::fflush(stdout);

		return Failed ? 1 : (AllSkipped ? 77 : 0);
	}

	// GLM_TEST_FINISH() and GLM_TEST_FINISH(Error) without needing __VA_OPT__ (C++20).
	struct finisher
	{
		char const* File;
		int operator()() const { return finish(File); }
		int operator()(int LegacyErrors) const { return finish(File, LegacyErrors); }
	};

	// Skip the whole test program: `return glm_test::skip_all("reason");` from main.
	inline int skip_all(char const* File, char const* Reason)
	{
		std::string Name = basename(File);
		std::string::size_type const Dot = Name.rfind('.');
		if (Dot != std::string::npos)
			Name.resize(Dot);
		std::printf("%s: skipped: %s\n", Name.c_str(), Reason);
		std::fflush(stdout);
		return 77;
	}
}//namespace glm_test

#define GLM_CHECK(...) ::glm_test::check(static_cast<bool>(__VA_ARGS__), #__VA_ARGS__, __FILE__, __LINE__)

#define GLM_TEST_DETAIL_COMPARE(Macro, Op, a, b) \
	([&](auto const& glm_test_a, auto const& glm_test_b) { \
		return ::glm_test::check_values(static_cast<bool>(glm_test_a Op glm_test_b), Macro, #a, #b, nullptr, glm_test_a, glm_test_b, __FILE__, __LINE__); \
	}((a), (b)))

#define GLM_CHECK_EQ(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_EQ", ==, a, b)
#define GLM_CHECK_NE(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_NE", !=, a, b)
#define GLM_CHECK_LT(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_LT", <, a, b)
#define GLM_CHECK_LE(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_LE", <=, a, b)
#define GLM_CHECK_GT(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_GT", >, a, b)
#define GLM_CHECK_GE(a, b) GLM_TEST_DETAIL_COMPARE("GLM_CHECK_GE", >=, a, b)

// glm::equal(a, b, eps) (component-wise for vectors and matrices, an epsilon or a ULP count), all
// components required to match: the same as glm::all(glm::equal(a, b, eps)) or the scalar form.
#define GLM_CHECK_NEAR(a, b, eps) \
	([&](auto const& glm_test_a, auto const& glm_test_b, auto const& glm_test_e) { \
		return ::glm_test::check_values(::glm_test::all_of(glm::equal(glm_test_a, glm_test_b, glm_test_e)), "GLM_CHECK_NEAR", #a, #b, #eps, \
			glm_test_a, glm_test_b, __FILE__, __LINE__); \
	}((a), (b), (eps)))

#define GLM_TEST(...) ::glm_test::run(#__VA_ARGS__, [&]() { return __VA_ARGS__; })
#define GLM_TEST_XFAIL(Condition, Reason, ...) ::glm_test::run(#__VA_ARGS__, [&]() { return __VA_ARGS__; }, (Condition), (Reason))
#define GLM_TEST_SKIP(Reason) ::glm_test::skip(Reason)
#define GLM_TEST_FINISH(...) ::glm_test::finisher{__FILE__}(__VA_ARGS__)
#define GLM_TEST_SKIP_ALL(Reason) ::glm_test::skip_all(__FILE__, (Reason))

// For tests of NaN and infinity: skip when the build assumes they don't exist (-ffinite-math-only,
// -ffast-math, MSVC /fp:fast), where the results depend on the optimizer rather than on GLM.
#if GLM_TEST_FINITE_MATH_ONLY
#	define GLM_TEST_REQUIRE_IEEE_SPECIALS() return ::glm_test::skip("requires NaN/Inf; built with finite-math assumptions")
#else
#	define GLM_TEST_REQUIRE_IEEE_SPECIALS() static_cast<void>(0)
#endif

// For tests whose expected results rely on exact IEEE rounding of each operation (compensated
// arithmetic such as log1p's x / ((1 + x) - 1), Cody-Waite range reduction): skip when fast-math
// lets the compiler simplify those expressions away, which is allowed and changes the accuracy.
#if GLM_TEST_FAST_MATH
#	define GLM_TEST_REQUIRE_STRICT_FP() return ::glm_test::skip("relies on exact IEEE rounding; built with fast-math")
#else
#	define GLM_TEST_REQUIRE_STRICT_FP() static_cast<void>(0)
#endif
