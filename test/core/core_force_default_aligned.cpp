// With GLM_FORCE_DEFAULT_ALIGNED_GENTYPES, the bare precision qualifiers
// (highp/mediump/lowp) alias the aligned_* variants, so every gentype - not
// just defaultp - is aligned/padded. packed_* remain for explicit tight packing.
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1

#include "glm_test.hpp"
#include <glm/glm.hpp>
#include <cstdio>

int main()
{
	int Error = 0;
#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	// bare qualifiers alias aligned_*
	Error += GLM_CHECK((glm::highp    == glm::aligned_highp));
	Error += GLM_CHECK((glm::mediump  == glm::aligned_mediump));
	Error += GLM_CHECK((glm::lowp     == glm::aligned_lowp));
	Error += GLM_CHECK(glm::detail::is_aligned<glm::highp>::value);
	Error += GLM_CHECK(glm::detail::is_aligned<glm::mediump>::value);

	// a vec3 with a bare qualifier is padded to 16 bytes, same as glm::vec3 and aligned_highp
	Error += GLM_CHECK((sizeof(glm::vec<3, float, glm::highp>) == sizeof(glm::vec<3, float, glm::aligned_highp>)));
	Error += GLM_CHECK((sizeof(glm::vec3) == sizeof(glm::vec<3, float, glm::highp>)));
	Error += GLM_CHECK((sizeof(glm::vec<3, float, glm::highp>) == 16));

	// packed_* still gives tight, unaligned storage
	Error += GLM_CHECK(!(glm::detail::is_aligned<glm::packed_highp>::value));
	Error += GLM_CHECK((sizeof(glm::vec<3, float, glm::packed_highp>) == 12));

	std::printf(Error == 0 ? "PASSED\n" : "FAILED (%d)\n", Error);
#else
	std::printf("SKIPPED (aligned gentypes disabled)\n");
#endif
	return GLM_TEST_FINISH(Error);
}
