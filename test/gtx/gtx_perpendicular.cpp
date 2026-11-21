#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <cmath>

static int test_perp()
{
	int Error = 0;

	// The part of x perpendicular to the normal.
	Error += GLM_CHECK_NEAR(glm::perp(glm::vec3(1, 2, 3), glm::vec3(0, 0, 1)), glm::vec3(1, 2, 0), 1e-6f);

	return Error;
}

// anyPerpendicular gives a unit vector at right angles to v, for directions all around the sphere:
// the axes, the z = 0 plane where its formula changes branch, and just either side of it.
template<typename T>
static int test_anyPerpendicular()
{
	using vec3 = glm::vec<3, T>;

	int Error = 0;
	T const Eps = static_cast<T>(1e-5);

	vec3 const Axes[] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
	for(vec3 const& V : Axes)
	{
		vec3 const P = glm::anyPerpendicular(V);
		Error += glm_test::context(GLM_CHECK_NEAR(glm::length(P), static_cast<T>(1), Eps), "V", V);
		Error += glm_test::context(GLM_CHECK_NEAR(glm::dot(P, V), static_cast<T>(0), Eps), "V", V);
	}

	for(int i = 0; i < 64; ++i)
	{
		for(int j = 0; j <= 32; ++j)
		{
			T const Azimuth = glm::two_pi<T>() * static_cast<T>(i) / static_cast<T>(64);
			T const Elevation = glm::pi<T>() * (static_cast<T>(j) / static_cast<T>(32) - static_cast<T>(0.5));
			// Scaled, so the input needn't be unit length; and nudged off the z = 0 plane both ways.
			for(T Nudge : {static_cast<T>(0), static_cast<T>(1e-6), static_cast<T>(-1e-6)})
			{
				vec3 const V = vec3(std::cos(Elevation) * std::cos(Azimuth), std::cos(Elevation) * std::sin(Azimuth),
				                    std::sin(Elevation) + Nudge) * static_cast<T>(3);
				vec3 const P = glm::anyPerpendicular(V);
				Error += glm_test::context(GLM_CHECK_NEAR(glm::length(P), static_cast<T>(1), Eps), "V", V);
				Error += glm_test::context(GLM_CHECK_NEAR(glm::dot(P, glm::normalize(V)), static_cast<T>(0), Eps), "V", V);
			}
		}
	}

	// It stands in for the cross product of parallel vectors: a vertical v gives a horizontal result.
	Error += GLM_CHECK_NEAR(glm::anyPerpendicular(vec3(0, 1, 0)).y, static_cast<T>(0), Eps);

	// A zero vector has no direction; it gives a fixed unit vector rather than NaN.
	Error += GLM_CHECK_EQ(glm::anyPerpendicular(vec3(0)), vec3(1, 0, 0));

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_perp());
	Error += GLM_TEST(test_anyPerpendicular<float>());
	Error += GLM_TEST(test_anyPerpendicular<double>());

	return GLM_TEST_FINISH(Error);
}
