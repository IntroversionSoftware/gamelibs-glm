#include "glm_test.hpp"
#include <glm/glm.hpp>

#if GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
struct vec2;

struct swizzleStruct
{
	char _buffer[1];
};

struct vec2
{
	GLM_CONSTEXPR vec2() :
		x(0), y(0)
	{}

#if (GLM_COMPILER & GLM_COMPILER_VC)
#	pragma warning(push)
#	pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#elif (GLM_COMPILER & GLM_COMPILER_CLANG)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#	pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

	union
	{
		struct { float x, y; };
		struct { swizzleStruct xx; };
	};

#if (GLM_COMPILER & GLM_COMPILER_VC)
#	pragma warning(pop)
#elif (GLM_COMPILER & GLM_COMPILER_CLANG)
#	pragma clang diagnostic pop
#endif
};
#endif

// Visual C++ has a bug generating the error: fatal error C1001: An internal error has occurred in the compiler.
// vec2 Bar;

#if GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
static int test_vec2()
{
	int Error = 0;

	vec2 const Foo;
	Error += GLM_CHECK_EQ(Foo.x, 0.0f);
	Error += GLM_CHECK_EQ(Foo.y, 0.0f);

	return Error;
}
#endif

int main()
{
#if GLM_CONFIG_ANONYMOUS_STRUCT == GLM_ENABLE
	int Error = 0;

	Error += GLM_TEST(test_vec2());

	return GLM_TEST_FINISH(Error);
#else
	return GLM_TEST_SKIP_ALL("anonymous structs are disabled");
#endif
}
