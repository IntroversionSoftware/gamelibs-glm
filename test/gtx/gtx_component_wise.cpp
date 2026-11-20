#define GLM_ENABLE_EXPERIMENTAL
#include "glm_test.hpp"
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <limits>

namespace compNormalize
{
	// Normalizing the extremes of the range lands within a few ULPs of +-1 (it can be 1 ULP off when
	// the division is reassociated into a multiplication by the reciprocal, e.g. with -ffast-math).
	static int run()
	{
		int Error(0);

		{
			glm::vec4 const A = glm::compNormalize<float>(glm::u8vec4(0, 127, 128, 255));

			Error += GLM_CHECK(glm::epsilonEqual(A.x, 0.0f, glm::epsilon<float>()));
			Error += GLM_CHECK(A.y < 0.5f);
			Error += GLM_CHECK(A.z > 0.5f);
			Error += GLM_CHECK(glm::equal(A.w, 1.0f, 4));
		}

		{
			glm::vec4 const A = glm::compNormalize<float>(glm::i8vec4(-128, -1, 0, 127));

			Error += GLM_CHECK(glm::equal(A.x, -1.0f, 4));
			Error += GLM_CHECK(A.y < 0.0f);
			Error += GLM_CHECK(A.z > 0.0f);
			Error += GLM_CHECK(glm::equal(A.w, 1.0f, 4));
		}

		{
			glm::vec4 const A = glm::compNormalize<float>(glm::u16vec4(
				std::numeric_limits<glm::u16>::min(),
				(std::numeric_limits<glm::u16>::max() >> 1) + 0,
				(std::numeric_limits<glm::u16>::max() >> 1) + 1,
				std::numeric_limits<glm::u16>::max()));

			Error += GLM_CHECK(glm::epsilonEqual(A.x, 0.0f, glm::epsilon<float>()));
			Error += GLM_CHECK(A.y < 0.5f);
			Error += GLM_CHECK(A.z > 0.5f);
			Error += GLM_CHECK(glm::equal(A.w, 1.0f, 4));
		}

		{
			glm::vec4 const A = glm::compNormalize<float>(glm::i16vec4(
				std::numeric_limits<glm::i16>::min(),
				static_cast<glm::i16>(-1),
				static_cast<glm::i16>(0),
				std::numeric_limits<glm::i16>::max()));

			Error += GLM_CHECK(glm::equal(A.x, -1.0f, 4));
			Error += GLM_CHECK(A.y < 0.0f);
			Error += GLM_CHECK(A.z > 0.0f);
			Error += GLM_CHECK(glm::equal(A.w, 1.0f, 4));
		}

		return Error;
	}
}//namespace compNormalize

namespace compScale
{
	static int run()
	{
		int Error(0);

		{
			glm::u8vec4 const A = glm::compScale<glm::u8>(glm::vec4(0.0f, 0.2f, 0.5f, 1.0f));

			Error += GLM_CHECK(A.x == std::numeric_limits<glm::u8>::min());
			Error += GLM_CHECK(A.y < (std::numeric_limits<glm::u8>::max() >> 2));
			Error += GLM_CHECK(A.z == 127);
			Error += GLM_CHECK(A.w == 255);
		}

		{
			glm::i8vec4 const A = glm::compScale<glm::i8>(glm::vec4(0.0f,-1.0f, 0.5f, 1.0f));

			Error += GLM_CHECK(A.x == 0);
			Error += GLM_CHECK(A.y == -128);
			Error += GLM_CHECK(A.z == 63);
			Error += GLM_CHECK(A.w == 127);
		}

		{
			glm::u16vec4 const A = glm::compScale<glm::u16>(glm::vec4(0.0f, 0.2f, 0.5f, 1.0f));

			Error += GLM_CHECK(A.x == std::numeric_limits<glm::u16>::min());
			Error += GLM_CHECK(A.y < (std::numeric_limits<glm::u16>::max() >> 2));
			Error += GLM_CHECK(A.z == 32767);
			Error += GLM_CHECK(A.w == 65535);
		}

		{
			glm::i16vec4 const A = glm::compScale<glm::i16>(glm::vec4(0.0f,-1.0f, 0.5f, 1.0f));

			Error += GLM_CHECK(A.x == 0);
			Error += GLM_CHECK(A.y == -32768);
			Error += GLM_CHECK(A.z == 16383);
			Error += GLM_CHECK(A.w == 32767);
		}

		return Error;
	}
}// compScale

#if ((GLM_LANG & GLM_LANG_CXX11_FLAG) || (GLM_COMPILER & GLM_COMPILER_VC))
namespace fcompMax
{
	static int run()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();

		int Error(0);

		{
			float const A = glm::fcompMax(glm::vec4(NAN, 0.2f, 0.5f, 1.0f));

			Error += GLM_CHECK(glm::equal(A, 1.0f, glm::epsilon<float>()));
		}

		{
			float const A = glm::fcompMax(glm::vec4(2.0f, NAN, 0.3f, 0.7f));

			Error += GLM_CHECK(glm::equal(A, 2.0f, glm::epsilon<float>()));
		}

		{
			float const A = glm::fcompMax(glm::vec4(NAN, NAN, NAN, NAN));

			Error += GLM_CHECK(std::isnan(A));
		}

		return Error;
	}
}// fcompMax

namespace fcompMin
{
	static int run()
	{
		GLM_TEST_REQUIRE_IEEE_SPECIALS();

		int Error(0);

		{
			float const A = glm::fcompMin(glm::vec4(NAN, 0.2f, 0.5f, 1.0f));

			Error += GLM_CHECK(glm::equal(A, 0.2f, glm::epsilon<float>()));
		}

		{
			float const A = glm::fcompMin(glm::vec4(2.0f, NAN, 0.3f, 0.7f));

			Error += GLM_CHECK(glm::equal(A, 0.3f, glm::epsilon<float>()));
		}

		{
			float const A = glm::fcompMin(glm::vec4(NAN, NAN, NAN, NAN));

			Error += GLM_CHECK(std::isnan(A));
		}


		return Error;
	}
}// fcompMin
#endif//((GLM_LANG & GLM_LANG_CXX11_FLAG) || (GLM_COMPILER & GLM_COMPILER_VC))

int main()
{
	int Error(0);

	Error += GLM_TEST(compNormalize::run());
	Error += GLM_TEST(compScale::run());

#if ((GLM_LANG & GLM_LANG_CXX11_FLAG) || (GLM_COMPILER & GLM_COMPILER_VC))
	Error += GLM_TEST(fcompMax::run());
	Error += GLM_TEST(fcompMin::run());
#endif//((GLM_LANG & GLM_LANG_CXX11_FLAG) || (GLM_COMPILER & GLM_COMPILER_VC))

	return GLM_TEST_FINISH(Error);
}
