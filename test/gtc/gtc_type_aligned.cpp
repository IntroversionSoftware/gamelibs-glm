// Force explicit ctors in this test to catch returning the wrong vector size from e.g. glm::xyz
#define GLM_FORCE_EXPLICIT_CTOR

#include "glm_test.hpp"
#include <glm/glm.hpp>

#if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
#include <glm/gtc/type_aligned.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/ext/scalar_relational.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>

static_assert(glm::detail::is_aligned<glm::aligned_lowp>::value, "aligned_lowp is not aligned");
static_assert(glm::detail::is_aligned<glm::aligned_mediump>::value, "aligned_mediump is not aligned");
static_assert(glm::detail::is_aligned<glm::aligned_highp>::value, "aligned_highp is not aligned");
static_assert(!glm::detail::is_aligned<glm::packed_highp>::value, "packed_highp is aligned");
static_assert(!glm::detail::is_aligned<glm::packed_mediump>::value, "packed_mediump is aligned");
static_assert(!glm::detail::is_aligned<glm::packed_lowp>::value, "packed_lowp is aligned");

struct my_vec4_packed
{
	glm::uint32 a;
	glm::packed_vec4 b;
};
static_assert(sizeof(my_vec4_packed) == sizeof(glm::uint32) + sizeof(glm::packed_vec4), "glm::vec4 packed is not correct");

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

struct my_vec4_aligned
{
	glm::uint32 a;
	glm::aligned_vec4 b;
};
static_assert(sizeof(my_vec4_aligned) == sizeof(glm::aligned_vec4) * 2, "glm::vec4 aligned is not correct");

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

struct my_dvec4_packed
{
	glm::uint64 a;
	glm::packed_dvec4 b;
};
static_assert(sizeof(my_dvec4_packed) == sizeof(glm::uint64) + sizeof(glm::packed_dvec4), "glm::dvec4 packed is not correct");

struct my_dvec4_aligned
{
	glm::uint64 a;
	glm::aligned_dvec4 b;
};
//static_assert(sizeof(my_dvec4_aligned) == sizeof(glm::aligned_dvec4) * 2, "glm::dvec4 aligned is not correct");

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

struct my_ivec4_packed
{
	glm::uint32 a;
	glm::packed_ivec4 b;
};

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

static_assert(sizeof(my_ivec4_packed) == sizeof(glm::uint32) + sizeof(glm::packed_ivec4), "glm::ivec4 packed is not correct");

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wpadded"
#endif

struct my_ivec4_aligned
{
	glm::uint32 a;
	glm::aligned_ivec4 b;
};

#if GLM_COMPILER & GLM_COMPILER_CLANG
#	pragma clang diagnostic pop
#endif

static_assert(sizeof(my_ivec4_aligned) == sizeof(glm::aligned_ivec4) * 2, "glm::ivec4 aligned is not correct");

struct my_u8vec4_packed
{
	glm::uint32 a;
	glm::u8vec4 b;
};
static_assert(sizeof(my_u8vec4_packed) == sizeof(glm::uint32) + sizeof(glm::u8vec4), "glm::u8vec4 packed is not correct");

static int test_copy_vec4()
{
	int Error = 0;
	{
		glm::aligned_vec4 const u(1.f, 2.f, 3.f, 4.f);
		glm::packed_vec4 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<float>());
	}
	{
		glm::packed_vec4 const u(1.f, 2.f, 3.f, 4.f);
		glm::aligned_vec4 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<float>());
	}

	{
		glm::aligned_dvec4 const u(1., 2., 3., 4.);
		glm::packed_dvec4 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<double>());
	}
	{
		glm::packed_dvec4 const u(1.f, 2.f, 3.f, 4.f);
		glm::aligned_dvec4 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<double>());
	}

	{
		glm::aligned_ivec4 const u(1, 2, 3, 4);
		glm::packed_ivec4 const v(u);
		glm::aligned_ivec4 const w(v);
		Error += GLM_CHECK(glm::all(glm::equal(u, w)));
	}
	{
		glm::packed_ivec4 const u(1, 2, 3, 4);
		glm::aligned_ivec4 const v(u);
		glm::packed_ivec4 const w(v);
		Error += GLM_CHECK(glm::all(glm::equal(u, w)));
	}

	return Error;
}

static int test_copy_vec3()
{
	int Error = 0;
	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::packed_vec3 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
	}
	{
		glm::packed_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec3 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::packed_dvec3 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
	}
	{
		glm::packed_dvec3 const u(1.f, 2.f, 3.f);
		glm::aligned_dvec3 const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
	}

	{
		glm::aligned_ivec3 const u(1, 2, 3);
		glm::packed_ivec3 const v(u);
		glm::aligned_ivec3 const w(v);
		Error += GLM_CHECK(glm::all(glm::equal(u, w)));
	}
	{
		glm::packed_ivec3 const u(1, 2, 3);
		glm::aligned_ivec3 const v(u);
		glm::packed_ivec3 const w(v);
		Error += GLM_CHECK(glm::all(glm::equal(u, w)));
	}

	return Error;
}

static int test_copy_quat()
{
	int Error = 0;
	{
		glm::aligned_quat const u(1.f, 2.f, 3.f, 4.f);
		glm::packed_quat const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<float>());
	}
	{
		glm::packed_quat const u(1.f, 2.f, 3.f, 4.f);
		glm::aligned_quat const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<float>());
	}

	{
		glm::aligned_dquat const u(1., 2., 3., 4.);
		glm::packed_dquat const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<double>());
	}
	{
		glm::packed_dquat const u(1., 2., 3., 4.);
		glm::aligned_dquat const v(u);
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, u.w, glm::epsilon<double>());
	}

	return Error;
}

static int test_splat_vec3()
{
	int Error = 0;
	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec3 const v(glm::splatX(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.x, glm::epsilon<float>());
	}

	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec3 const v(glm::splatY(u));
		Error += GLM_CHECK_NEAR(v.x, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.y, glm::epsilon<float>());
	}

	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec3 const v(glm::splatZ(u));
		Error += GLM_CHECK_NEAR(v.x, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec3 const v(glm::splatX(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.x, glm::epsilon<double>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec3 const v(glm::splatY(u));
		Error += GLM_CHECK_NEAR(v.x, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.y, glm::epsilon<double>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec3 const v(glm::splatZ(u));
		Error += GLM_CHECK_NEAR(v.x, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
	}

	return Error;
}

static int test_splat_vec4()
{
	int Error = 0;
	{
		glm::aligned_vec4 const u(1.f, 2.f, 3.f, 4.f);
		{
			glm::aligned_vec4 const v(glm::splatX(u));
			Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.y, u.x, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.z, u.x, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.w, u.x, glm::epsilon<float>());
		}

		{
			glm::aligned_vec4 const v(glm::splatY(u));
			Error += GLM_CHECK_NEAR(v.x, u.y, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.z, u.y, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.w, u.y, glm::epsilon<float>());
		}

		{
			glm::aligned_vec4 const v(glm::splatZ(u));
			Error += GLM_CHECK_NEAR(v.x, u.z, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.y, u.z, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		    Error += GLM_CHECK_NEAR(v.w, u.z, glm::epsilon<float>());
		}
	}
	{
		glm::aligned_dvec4 const u(1., 2., 3., 4.);
		{
			glm::aligned_dvec4 const v(glm::splatX(u));
			Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.y, u.x, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.z, u.x, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.w, u.x, glm::epsilon<double>());
		}

		{
			glm::aligned_dvec4 const v(glm::splatY(u));
			Error += GLM_CHECK_NEAR(v.x, u.y, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.z, u.y, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.w, u.y, glm::epsilon<double>());
		}

		{
			glm::aligned_dvec4 const v(glm::splatZ(u));
			Error += GLM_CHECK_NEAR(v.x, u.z, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.y, u.z, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		    Error += GLM_CHECK_NEAR(v.w, u.z, glm::epsilon<double>());
		}
	}
	return Error;
}

static int test_copy_vec4_vec3()
{
	int Error = 0;

	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec4 const v(glm::xyz0(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, 0.0f, glm::epsilon<float>());
	}

	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec4 const v(glm::xyz1(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += glm::equal(v.w, 1.0f, glm::epsilon<float>()) ? 0 : 1;	;
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec4 const v(glm::xyz0(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, 0.0, glm::epsilon<double>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec4 const v(glm::xyz1(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, 1.0, glm::epsilon<double>());
	}

	{
		glm::aligned_vec3 const u(1.f, 2.f, 3.f);
		glm::aligned_vec4 const v(glm::xyzz(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.w, u.z, glm::epsilon<float>());
	}

	{
		glm::aligned_dvec3 const u(1., 2., 3.);
		glm::aligned_dvec4 const v(glm::xyzz(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.w, u.z, glm::epsilon<double>());
	}


	{
		glm::aligned_vec4 const u(1.f, 2.f, 3.f, 4.f);
		glm::aligned_vec3 const v(glm::xyz(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<float>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<float>());
	}

	{
		glm::aligned_dvec4 const u(1., 2., 3., 4.);
		glm::aligned_dvec3 const v(glm::xyz(u));
		Error += GLM_CHECK_NEAR(v.x, u.x, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.y, u.y, glm::epsilon<double>());
		Error += GLM_CHECK_NEAR(v.z, u.z, glm::epsilon<double>());
	}
	return Error;
}

static int test_copy()
{
	int Error = 0;

	{
		glm::aligned_ivec4 const a(1, 2, 3, 4);
		glm::ivec4 const u(a);
		glm::aligned_ivec4 const v(u);

		Error += GLM_CHECK(glm::all(glm::equal(a, v)));
	}

	{
		my_ivec4_aligned a;
		a.b = glm::ivec4(1, 2, 3, 4);

		my_ivec4_packed u;
		u.b = a.b;

		Error += GLM_CHECK_EQ(a.b.x, u.b.x);
		Error += GLM_CHECK_EQ(a.b.y, u.b.y);
		Error += GLM_CHECK_EQ(a.b.z, u.b.z);
		Error += GLM_CHECK_EQ(a.b.w, u.b.w);
	}

	return Error;
}

static int test_aligned_ivec4()
{
	int Error = 0;

	glm::aligned_ivec4 const v(1, 2, 3, 4);
	Error += GLM_CHECK(glm::all(glm::equal(v, glm::aligned_ivec4(1, 2, 3, 4))));

	glm::aligned_ivec4 const u = v * 2;
	Error += GLM_CHECK(glm::all(glm::equal(u, glm::aligned_ivec4(2, 4, 6, 8))));

	return Error;
}

static int test_aligned_mat4()
{
	int Error = 0;

	glm::aligned_vec4 const u(1.f, 2.f, 3.f, 4.f);
	Error += GLM_CHECK_NEAR(u, glm::aligned_vec4(1.f, 2.f, 3.f, 4.f), 0.0001f);

	glm::aligned_vec4 const v(1, 2, 3, 4);
	Error += GLM_CHECK_NEAR(v, glm::aligned_vec4(1.f, 2.f, 3.f, 4.f), 0.0001f);

	glm::aligned_mat4 const m(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	glm::aligned_mat4 const t = glm::transpose(m);
	glm::aligned_mat4 const expected = glm::mat4(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
	Error += GLM_CHECK_NEAR(t, expected, 0.0001f);

	return Error;
}


int main()
{
	int Error = 0;

	Error += GLM_TEST(test_copy_vec4());
	Error += GLM_TEST(test_copy_vec3());
	Error += GLM_TEST(test_splat_vec3());
	Error += GLM_TEST(test_splat_vec4());
	Error += GLM_TEST(test_copy_vec4_vec3());
	Error += GLM_TEST(test_copy());
	Error += GLM_TEST(test_copy_vec4());
	Error += GLM_TEST(test_copy_vec3());
	Error += GLM_TEST(test_copy_quat());
	Error += GLM_TEST(test_aligned_ivec4());
	Error += GLM_TEST(test_aligned_mat4());


	return GLM_TEST_FINISH(Error);
}

#else

int main()
{
	return GLM_TEST_SKIP_ALL("aligned gentypes are disabled");
}

#endif
