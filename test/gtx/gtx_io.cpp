#include "glm_test.hpp"
#include <glm/glm.hpp>
#if GLM_LANG & GLM_LANG_CXXMS_FLAG
#include <glm/gtc/type_precision.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>

namespace
{
	// What Value streams to, in a fresh stream of characters CTy with the manipulators Setup applies
	template<typename CTy, typename T, typename Setup>
	std::basic_string<CTy> streamed(T const& Value, Setup const& setup)
	{
		std::basic_ostringstream<CTy> os;
		setup(os);
		os << Value;
		return os.str();
	}

	// The expected text, as characters CTy (the text is ASCII)
	template<typename CTy>
	std::basic_string<CTy> text(char const* s)
	{
		return std::basic_string<CTy>(s, s + std::strlen(s));
	}

	template<typename OS>
	void formatted(OS& os)
	{
		os << glm::io::precision(2) << glm::io::width(1 + 2 + 1 + 2);
	}

	template<typename OS>
	void defaults(OS&)
	{
	}

	template<typename OS>
	void unformatted(OS& os)
	{
		os << glm::io::unformatted;
	}
} // namespace

template<typename T, glm::qualifier P, typename CTy>
static int test_io_quat()
{
	int Error = 0;

	glm::qua<T, P> const q(1, 0, 0, 0);

	if constexpr (std::is_floating_point<T>::value)
		Error += GLM_CHECK_EQ(streamed<CTy>(q, formatted<std::basic_ostream<CTy>>), text<CTy>("[  0.00,  0.00,  0.00,  1.00]"));
	else
		Error += GLM_CHECK_EQ(streamed<CTy>(q, formatted<std::basic_ostream<CTy>>), text<CTy>("[     0,     0,     0,     1]"));
	Error += GLM_CHECK_EQ(streamed<CTy>(q, unformatted<std::basic_ostream<CTy>>), text<CTy>("0 0 0 1"));

	return Error;
}

template<typename T, glm::qualifier P, typename CTy>
static int test_io_vec()
{
	int Error = 0;

	glm::vec<2, T,P> const v2(0, 1);
	glm::vec<3, T,P> const v3(2, 3, 4);
	glm::vec<4, T,P> const v4(5, 6, 7, 8);

	auto const def = defaults<std::basic_ostream<CTy>>;
	auto const fmt = formatted<std::basic_ostream<CTy>>;
	if constexpr (std::is_floating_point<T>::value)
	{
		Error += GLM_CHECK_EQ(streamed<CTy>(v2, def), text<CTy>("[    0.000,    1.000]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v3, def), text<CTy>("[    2.000,    3.000,    4.000]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v4, def), text<CTy>("[    5.000,    6.000,    7.000,    8.000]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v2, fmt), text<CTy>("[  0.00,  1.00]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v3, fmt), text<CTy>("[  2.00,  3.00,  4.00]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v4, fmt), text<CTy>("[  5.00,  6.00,  7.00,  8.00]"));
	}
	else
	{
		Error += GLM_CHECK_EQ(streamed<CTy>(v2, def), text<CTy>("[        0,        1]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v3, def), text<CTy>("[        2,        3,        4]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v4, def), text<CTy>("[        5,        6,        7,        8]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v2, fmt), text<CTy>("[     0,     1]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v3, fmt), text<CTy>("[     2,     3,     4]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(v4, fmt), text<CTy>("[     5,     6,     7,     8]"));
	}

	return Error;
}

// The matrices are built from columns (0, 1), (2, 3), ... for 2 rows, (8, 9, 10), ... for 3 and
// (20, 21, 22, 23), ... for 4. column_major prints them in mathematical layout, a row per line;
// row_major prints a column per line.
template<typename T, glm::qualifier P, typename CTy>
static int test_io_mat(glm::io::order_type otype)
{
	int Error = 0;

	glm::vec<2, T,P> const v2_1( 0,  1);
	glm::vec<2, T,P> const v2_2( 2,  3);
	glm::vec<2, T,P> const v2_3( 4,  5);
	glm::vec<3, T,P> const v3_1( 8,  9, 10);
	glm::vec<3, T,P> const v3_2(11, 12, 13);
	glm::vec<4, T,P> const v4_1(20, 21, 22, 23);
	glm::vec<4, T,P> const v4_2(24, 25, 26, 27);
	glm::vec<4, T,P> const v4_3(28, 29, 30, 31);
	glm::vec<4, T,P> const v4_4(32, 33, 34, 35);

	glm::mat<2, 3, T,P> const m2x3(v3_1, v3_2);
	glm::mat<3, 2, T,P> const m3x2(v2_1, v2_2, v2_3);
	glm::mat<4, 4, T,P> const m4x4(v4_1, v4_2, v4_3, v4_4);

	auto const fmt = [otype](std::basic_ostream<CTy>& os) { formatted(os); os << glm::io::order(otype); };
	auto const raw = [otype](std::basic_ostream<CTy>& os) { os << glm::io::unformatted << glm::io::order(otype); };

	if (otype == glm::io::column_major)
	{
		Error += GLM_CHECK_EQ(streamed<CTy>(m2x3, fmt), text<CTy>(
			"\n[[  8.00, 11.00]\n [  9.00, 12.00]\n [ 10.00, 13.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m3x2, fmt), text<CTy>(
			"\n[[  0.00,  2.00,  4.00]\n [  1.00,  3.00,  5.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m4x4, fmt), text<CTy>(
			"\n[[ 20.00, 24.00, 28.00, 32.00]\n [ 21.00, 25.00, 29.00, 33.00]\n [ 22.00, 26.00, 30.00, 34.00]\n [ 23.00, 27.00, 31.00, 35.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m2x3, raw), text<CTy>("8 9 10 11 12 13"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m3x2, raw), text<CTy>("0 1 2 3 4 5"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m4x4, raw), text<CTy>("20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35"));
	}
	else
	{
		Error += GLM_CHECK_EQ(streamed<CTy>(m2x3, fmt), text<CTy>(
			"\n[[  8.00,  9.00, 10.00]\n [ 11.00, 12.00, 13.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m3x2, fmt), text<CTy>(
			"\n[[  0.00,  1.00]\n [  2.00,  3.00]\n [  4.00,  5.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m4x4, fmt), text<CTy>(
			"\n[[ 20.00, 21.00, 22.00, 23.00]\n [ 24.00, 25.00, 26.00, 27.00]\n [ 28.00, 29.00, 30.00, 31.00]\n [ 32.00, 33.00, 34.00, 35.00]]"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m2x3, raw), text<CTy>("8 11 9 12 10 13"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m3x2, raw), text<CTy>("0 2 4 1 3 5"));
		Error += GLM_CHECK_EQ(streamed<CTy>(m4x4, raw), text<CTy>("20 24 28 32 21 25 29 33 22 26 30 34 23 27 31 35"));
	}

	return Error;
}

int main()
{
	int Error(0);

	Error += GLM_TEST(test_io_quat<float, glm::highp, char>());
	Error += GLM_TEST(test_io_quat<float, glm::highp, wchar_t>());
	Error += GLM_TEST(test_io_quat<int, glm::mediump, char>());
	Error += GLM_TEST(test_io_quat<int, glm::mediump, wchar_t>());
	Error += GLM_TEST(test_io_quat<glm::uint, glm::lowp, char>());
	Error += GLM_TEST(test_io_quat<glm::uint, glm::lowp, wchar_t>());

	Error += GLM_TEST(test_io_vec<float, glm::highp, char>());
	Error += GLM_TEST(test_io_vec<float, glm::highp, wchar_t>());
	Error += GLM_TEST(test_io_vec<int, glm::mediump, char>());
	Error += GLM_TEST(test_io_vec<int, glm::mediump, wchar_t>());
	Error += GLM_TEST(test_io_vec<glm::uint, glm::lowp, char>());
	Error += GLM_TEST(test_io_vec<glm::uint, glm::lowp, wchar_t>());

	Error += GLM_TEST(test_io_mat<float, glm::highp, char>(glm::io::column_major));
	Error += GLM_TEST(test_io_mat<float, glm::lowp, wchar_t>(glm::io::column_major));
	Error += GLM_TEST(test_io_mat<float, glm::highp, char>(glm::io::row_major));
	Error += GLM_TEST(test_io_mat<float, glm::lowp, wchar_t>(glm::io::row_major));

	return GLM_TEST_FINISH(Error);
}
#else

int main()
{
	return GLM_TEST_SKIP_ALL("requires GLM language extensions (GLM_LANG_CXXMS_FLAG)");
}

#endif// GLM_LANG & GLM_LANG_CXXMS_FLAG
