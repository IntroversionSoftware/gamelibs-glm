#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/iteration.hpp>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Testing if adl works if it doesn't a compiler error is generated
// Note all usages of glm::begin,glm::end,glm::rbegin,glm::rend 
// in this file is via adl
template<typename T>
void begin(T) {}
template<typename T>
void rbegin(T) {}
template<typename T>
void end(T) {}
template<typename T>
void rend(T) {}


// Flat iteration over a matrix requires packed columns: aligned 3-row matrices pad each column, and
// glm::begin/end reject them at compile time.
template<glm::length_t C, glm::length_t R>
using pmat = glm::mat<C, R, float, glm::packed_highp>;

template<typename T>
std::ptrdiff_t get_iter_distance() {
	T t = T(); // needed for const initializing
	return std::distance(begin(t), end(t));
}

template<typename T>
std::ptrdiff_t get_rev_iter_distance() {
	T t = T(); // needed for const initializing
	return std::distance(rbegin(t), rend(t));
}


template<typename GenTypc>
int rangedSumComp(GenTypc& genType, int compsum) {
	int Error = 0;

#if defined(__cpp_range_based_for)
#if __cpp_range_based_for >= 200907L
	{
		int sum = 0;
		for (const int& elem : genType)
			sum += elem;
		Error += GLM_CHECK_EQ(sum, compsum);
	}
#endif
#endif
	{
		int sum = 0;
		for (const int* it = begin(genType); it != end(genType); ++it)
			sum += *it;
		Error += GLM_CHECK_EQ(sum, compsum);
	}

	{
		int sum = 0;
		for (std::reverse_iterator<const int*> it = rbegin(genType); it != rend(genType); ++it)
			sum += *it;
		Error += GLM_CHECK_EQ(sum, compsum);
	}
	return Error;
}

template<glm::length_t L>
int sumVec() {
	glm::vec<L, int> v;
	int sumManual = 0;
	for (int i = 0; i < L; ++i) {
		v[i] = i;
		sumManual += i;
	}
	glm::vec<L, int> const cv = v;

	int Error = 0;
	Error += GLM_CHECK_EQ(std::accumulate(begin(v), end(v), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(begin(cv), end(cv), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(v), rend(v), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(cv), rend(cv), 0), sumManual);
	Error += rangedSumComp(v, sumManual);
	Error += rangedSumComp(cv, sumManual);
	return Error;
}

template<glm::length_t C, glm::length_t R>
int sumMat() {
	glm::mat<C, R, int, glm::packed_highp> m;
	int sumManual = 0;
	for (int i = 0; i < C; ++i) {
		for (int j = 0; j < R; ++j) {
			m[i][j] = i * j;
			sumManual += i * j;
		}
	}
	glm::mat<C, R, int, glm::packed_highp> const cm = m;
	int Error = 0;
	Error += GLM_CHECK_EQ(std::accumulate(begin(m), end(m), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(begin(cm), end(cm), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(m), rend(m), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(cm), rend(cm), 0), sumManual);
	Error += rangedSumComp(m, sumManual);
	Error += rangedSumComp(cm, sumManual);
	return Error;
}

static int sumQuat() {
	glm::qua<int> q(0, 1, 2, 3);
	glm::qua<int> const cq(0, 1, 2, 3);
	const int sumManual = (0 + 1 + 2 + 3);
	int Error = 0;
	Error += GLM_CHECK_EQ(std::accumulate(begin(q), end(q), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(begin(cq), end(cq), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(q), rend(q), 0), sumManual);
	Error += GLM_CHECK_EQ(std::accumulate(rbegin(cq), rend(cq), 0), sumManual);
	Error += rangedSumComp(q, sumManual);
	Error += rangedSumComp(cq, sumManual);
	return Error;
}


// Iterators span every component
static int test_iter_distance()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(get_iter_distance<glm::vec1>(), 1);
	Error += GLM_CHECK_EQ(get_iter_distance<glm::vec2>(), 2);
	Error += GLM_CHECK_EQ(get_iter_distance<glm::vec3>(), 3);
	Error += GLM_CHECK_EQ(get_iter_distance<glm::vec4>(), 4);
	Error += GLM_CHECK_EQ(get_iter_distance<const glm::vec1>(), 1);
	Error += GLM_CHECK_EQ(get_iter_distance<const glm::vec2>(), 2);
	Error += GLM_CHECK_EQ(get_iter_distance<const glm::vec3>(), 3);
	Error += GLM_CHECK_EQ(get_iter_distance<const glm::vec4>(), 4);

	Error += GLM_CHECK_EQ(get_iter_distance<glm::quat>(), 4);
	Error += GLM_CHECK_EQ(get_iter_distance<const glm::quat>(), 4);

	Error += GLM_CHECK_EQ((get_iter_distance<pmat<2, 2>>()), 2 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<2, 3>>()), 2 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<2, 4>>()), 2 * 4);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<2, 2>>()), 2 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<2, 3>>()), 2 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<2, 4>>()), 2 * 4);

	Error += GLM_CHECK_EQ((get_iter_distance<pmat<3, 2>>()), 3 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<3, 3>>()), 3 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<3, 4>>()), 3 * 4);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<3, 2>>()), 3 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<3, 3>>()), 3 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<3, 4>>()), 3 * 4);

	Error += GLM_CHECK_EQ((get_iter_distance<pmat<4, 2>>()), 4 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<4, 3>>()), 4 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<pmat<4, 4>>()), 4 * 4);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<4, 2>>()), 4 * 2);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<4, 3>>()), 4 * 3);
	Error += GLM_CHECK_EQ((get_iter_distance<const pmat<4, 4>>()), 4 * 4);

	return Error;
}

static int test_rev_iter_distance()
{
	int Error = 0;

	Error += GLM_CHECK_EQ(get_rev_iter_distance<glm::vec1>(), 1);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<glm::vec2>(), 2);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<glm::vec3>(), 3);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<glm::vec4>(), 4);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<const glm::vec1>(), 1);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<const glm::vec2>(), 2);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<const glm::vec3>(), 3);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<const glm::vec4>(), 4);

	Error += GLM_CHECK_EQ(get_rev_iter_distance<glm::quat>(), 4);
	Error += GLM_CHECK_EQ(get_rev_iter_distance<const glm::quat>(), 4);

	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<2, 2>>()), 2 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<2, 3>>()), 2 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<2, 4>>()), 2 * 4);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<2, 2>>()), 2 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<2, 3>>()), 2 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<2, 4>>()), 2 * 4);

	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<3, 2>>()), 3 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<3, 3>>()), 3 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<3, 4>>()), 3 * 4);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<3, 2>>()), 3 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<3, 3>>()), 3 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<3, 4>>()), 3 * 4);

	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<4, 2>>()), 4 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<4, 3>>()), 4 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<pmat<4, 4>>()), 4 * 4);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<4, 2>>()), 4 * 2);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<4, 3>>()), 4 * 3);
	Error += GLM_CHECK_EQ((get_rev_iter_distance<const pmat<4, 4>>()), 4 * 4);

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_iter_distance());
	Error += GLM_TEST(test_rev_iter_distance());

	Error += GLM_TEST(sumVec<1>());
	Error += GLM_TEST(sumVec<2>());
	Error += GLM_TEST(sumVec<3>());
	Error += GLM_TEST(sumVec<4>());

	Error += GLM_TEST(sumMat<2, 2>());
	Error += GLM_TEST(sumMat<2, 3>());
	Error += GLM_TEST(sumMat<2, 4>());

	Error += GLM_TEST(sumMat<3, 2>());
	Error += GLM_TEST(sumMat<3, 3>());
	Error += GLM_TEST(sumMat<3, 4>());

	Error += GLM_TEST(sumMat<4, 2>());
	Error += GLM_TEST(sumMat<4, 3>());
	Error += GLM_TEST(sumMat<4, 4>());

	Error += GLM_TEST(sumQuat());

	return GLM_TEST_FINISH(Error);
}
