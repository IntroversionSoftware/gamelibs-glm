#include "glm_test.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <cstring>
#include <memory>
#include <type_traits>

static int test_value_ptr_vec()
{
	int Error = 0;

	{
		glm::vec2 v(1.0);
		float * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::vec3 v(1.0);
		float * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::vec4 v(1.0);
		float * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}

	{
		glm::dvec2 v(1.0);
		double * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::dvec3 v(1.0);
		double * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::dvec4 v(1.0);
		double * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}

	return Error;
}

static int test_value_ptr_vec_const()
{
	int Error = 0;

	{
		glm::vec2 const v(1.0);
		float const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::vec3 const v(1.0);
		float const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::vec4 const v(1.0);
		float const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}

	{
		glm::dvec2 const v(1.0);
		double const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::dvec3 const v(1.0);
		double const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}
	{
		glm::dvec4 const v(1.0);
		double const * p = glm::value_ptr(v);
		Error += GLM_CHECK_EQ(p, &v[0]);
	}

	return Error;
}

static int test_value_ptr_mat()
{
	int Error = 0;

	{
		glm::mat2x2 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat2x3 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat2x4 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x2 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x3 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x4 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x2 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x3 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x4 m(1.0);
		float * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}

	return Error;
}

static int test_value_ptr_mat_const()
{
	int Error = 0;

	{
		glm::mat2x2 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat2x3 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat2x4 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x2 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x3 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat3x4 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x2 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x3 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}
	{
		glm::mat4x4 const m(1.0);
		float const * p = glm::value_ptr(m);
		Error += GLM_CHECK_EQ(p, &m[0][0]);
	}

	return Error;
}

static int test_make_pointer_mat()
{
	int Error = 0;

	float ArrayA[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	double ArrayB[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	glm::mat2x2 Mat2x2A = glm::make_mat2x2(ArrayA);
	Error += GLM_CHECK_NEAR(Mat2x2A, glm::mat2x2(0, 1, 2, 3), 0.001f);

	glm::mat2x3 Mat2x3A = glm::make_mat2x3(ArrayA);
	Error += GLM_CHECK_NEAR(Mat2x3A, glm::mat2x3(0, 1, 2, 3, 4, 5), 0.001f);

	glm::mat2x4 Mat2x4A = glm::make_mat2x4(ArrayA);
	Error += GLM_CHECK_NEAR(Mat2x4A, glm::mat2x4(0, 1, 2, 3, 4, 5, 6, 7), 0.001f);

	glm::mat3x2 Mat3x2A = glm::make_mat3x2(ArrayA);
	Error += GLM_CHECK_NEAR(Mat3x2A, glm::mat3x2(0, 1, 2, 3, 4, 5), 0.001f);

	glm::mat3x3 Mat3x3A = glm::make_mat3x3(ArrayA);
	Error += GLM_CHECK_NEAR(Mat3x3A, glm::mat3x3(0, 1, 2, 3, 4, 5, 6, 7, 8), 0.001f);

	glm::mat3x4 Mat3x4A = glm::make_mat3x4(ArrayA);
	Error += GLM_CHECK_NEAR(Mat3x4A, glm::mat3x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11), 0.001f);

	glm::mat4x2 Mat4x2A = glm::make_mat4x2(ArrayA);
	Error += GLM_CHECK_NEAR(Mat4x2A, glm::mat4x2(0, 1, 2, 3, 4, 5, 6, 7), 0.001f);

	glm::mat4x3 Mat4x3A = glm::make_mat4x3(ArrayA);
	Error += GLM_CHECK_NEAR(Mat4x3A, glm::mat4x3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11), 0.001f);

	glm::mat4x4 Mat4x4A = glm::make_mat4x4(ArrayA);
	Error += GLM_CHECK_NEAR(Mat4x4A, glm::mat4x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), 0.001f);



	glm::dmat2x2 Mat2x2B = glm::make_mat2x2(ArrayB);
	Error += GLM_CHECK_NEAR(Mat2x2B, glm::dmat2x2(0, 1, 2, 3), 0.001);

	glm::dmat2x3 Mat2x3B = glm::make_mat2x3(ArrayB);
	Error += GLM_CHECK_NEAR(Mat2x3B, glm::dmat2x3(0, 1, 2, 3, 4, 5), 0.001);

	glm::dmat2x4 Mat2x4B = glm::make_mat2x4(ArrayB);
	Error += GLM_CHECK_NEAR(Mat2x4B, glm::dmat2x4(0, 1, 2, 3, 4, 5, 6, 7), 0.001);

	glm::dmat3x2 Mat3x2B = glm::make_mat3x2(ArrayB);
	Error += GLM_CHECK_NEAR(Mat3x2B, glm::dmat3x2(0, 1, 2, 3, 4, 5), 0.001);

	glm::dmat3x3 Mat3x3B = glm::make_mat3x3(ArrayB);
	Error += GLM_CHECK_NEAR(Mat3x3B, glm::dmat3x3(0, 1, 2, 3, 4, 5, 6, 7, 8), 0.001);

	glm::dmat3x4 Mat3x4B = glm::make_mat3x4(ArrayB);
	Error += GLM_CHECK_NEAR(Mat3x4B, glm::dmat3x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11), 0.001);

	glm::dmat4x2 Mat4x2B = glm::make_mat4x2(ArrayB);
	Error += GLM_CHECK_NEAR(Mat4x2B, glm::dmat4x2(0, 1, 2, 3, 4, 5, 6, 7), 0.001);

	glm::dmat4x3 Mat4x3B = glm::make_mat4x3(ArrayB);
	Error += GLM_CHECK_NEAR(Mat4x3B, glm::dmat4x3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11), 0.001);

	glm::dmat4x4 Mat4x4B = glm::make_mat4x4(ArrayB);
	Error += GLM_CHECK_NEAR(Mat4x4B, glm::dmat4x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), 0.001);

	return Error;
}

static int test_make_pointer_vec()
{
	int Error = 0;

	float ArrayA[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	int ArrayB[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	bool ArrayC[] = {true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false};

	glm::vec2 Vec2A = glm::make_vec2(ArrayA);
	Error += GLM_CHECK_NEAR(Vec2A, glm::vec2(0, 1), 0.001f);

	glm::vec3 Vec3A = glm::make_vec3(ArrayA);
	Error += GLM_CHECK_NEAR(Vec3A, glm::vec3(0, 1, 2), 0.001f);

	glm::vec4 Vec4A = glm::make_vec4(ArrayA);
	Error += GLM_CHECK_NEAR(Vec4A, glm::vec4(0, 1, 2, 3), 0.001f);

	glm::ivec2 Vec2B = glm::make_vec2(ArrayB);
	Error += GLM_CHECK(glm::all(glm::equal(Vec2B, glm::ivec2(0, 1))));

	glm::ivec3 Vec3B = glm::make_vec3(ArrayB);
	Error += GLM_CHECK(glm::all(glm::equal(Vec3B, glm::ivec3(0, 1, 2))));

	glm::ivec4 Vec4B = glm::make_vec4(ArrayB);
	Error += GLM_CHECK(glm::all(glm::equal(Vec4B, glm::ivec4(0, 1, 2, 3))));

	glm::bvec2 Vec2C = glm::make_vec2(ArrayC);
	Error += GLM_CHECK(glm::all(glm::equal(Vec2C, glm::bvec2(true, false))));

	glm::bvec3 Vec3C = glm::make_vec3(ArrayC);
	Error += GLM_CHECK(glm::all(glm::equal(Vec3C, glm::bvec3(true, false, true))));

	glm::bvec4 Vec4C = glm::make_vec4(ArrayC);
	Error += GLM_CHECK(glm::all(glm::equal(Vec4C, glm::bvec4(true, false, true, false))));

	return Error;
}

static int test_make_vec1()
{
	int Error = 0;

	glm::ivec1 const v1 = glm::make_vec1(glm::ivec1(2));
	Error += GLM_CHECK_EQ(v1, glm::ivec1(2));

	glm::ivec1 const v2 = glm::make_vec1(glm::ivec2(2));
	Error += GLM_CHECK_EQ(v2, glm::ivec1(2));

	glm::ivec1 const v3 = glm::make_vec1(glm::ivec3(2));
	Error += GLM_CHECK_EQ(v3, glm::ivec1(2));

	glm::ivec1 const v4 = glm::make_vec1(glm::ivec4(2));
	Error += GLM_CHECK_EQ(v4, glm::ivec1(2));

	return Error;
}

static int test_make_vec2()
{
	int Error = 0;

	glm::ivec2 const v1 = glm::make_vec2(glm::ivec1(2));
	Error += GLM_CHECK_EQ(v1, glm::ivec2(2, 0));

	glm::ivec2 const v2 = glm::make_vec2(glm::ivec2(2));
	Error += GLM_CHECK_EQ(v2, glm::ivec2(2, 2));

	glm::ivec2 const v3 = glm::make_vec2(glm::ivec3(2));
	Error += GLM_CHECK_EQ(v3, glm::ivec2(2, 2));

	glm::ivec2 const v4 = glm::make_vec2(glm::ivec4(2));
	Error += GLM_CHECK_EQ(v4, glm::ivec2(2, 2));

	return Error;
}

static int test_make_vec3()
{
	int Error = 0;

	glm::ivec3 const v1 = glm::make_vec3(glm::ivec1(2));
	Error += GLM_CHECK_EQ(v1, glm::ivec3(2, 0, 0));

	glm::ivec3 const v2 = glm::make_vec3(glm::ivec2(2));
	Error += GLM_CHECK_EQ(v2, glm::ivec3(2, 2, 0));

	glm::ivec3 const v3 = glm::make_vec3(glm::ivec3(2));
	Error += GLM_CHECK_EQ(v3, glm::ivec3(2, 2, 2));

	glm::ivec3 const v4 = glm::make_vec3(glm::ivec4(2));
	Error += GLM_CHECK_EQ(v4, glm::ivec3(2, 2, 2));

	return Error;
}

static int test_make_vec4()
{
	int Error = 0;

	glm::ivec4 const v1 = glm::make_vec4(glm::ivec1(2));
	Error += GLM_CHECK_EQ(v1, glm::ivec4(2, 0, 0, 1));

	glm::ivec4 const v2 = glm::make_vec4(glm::ivec2(2));
	Error += GLM_CHECK_EQ(v2, glm::ivec4(2, 2, 0, 1));

	glm::ivec4 const v3 = glm::make_vec4(glm::ivec3(2));
	Error += GLM_CHECK_EQ(v3, glm::ivec4(2, 2, 2, 1));

	glm::ivec4 const v4 = glm::make_vec4(glm::ivec4(2));
	Error += GLM_CHECK_EQ(v4, glm::ivec4(2, 2, 2, 2));

	return Error;
}

// to_packed gives a tightly packed copy whatever the source's qualifier, so value_ptr of it is safe to
// hand to APIs that expect C * R contiguous components, even when the default types are aligned and padded.
static int test_to_packed()
{
	int Error = 0;

	static_assert(std::is_same<glm::packed_t<glm::mediump_mat3>, glm::mat<3, 3, float, glm::packed_mediump> >::value, "to_packed keeps precision");
	static_assert(std::is_same<glm::packed_t<glm::lowp_vec3>, glm::vec<3, float, glm::packed_lowp> >::value, "to_packed keeps precision");
	static_assert(std::is_same<glm::packed_t<glm::dquat>, glm::qua<double, glm::packed_highp> >::value, "to_packed of a quaternion");
	static_assert(std::is_same<glm::packed_t<glm::vec<3, float, glm::packed_highp> >, glm::vec<3, float, glm::packed_highp> >::value, "packed types map to themselves");
	static_assert(sizeof(glm::packed_t<glm::mat3>) == 9 * sizeof(float), "packed mat3 has no padding");
	static_assert(sizeof(glm::packed_t<glm::dmat4x3>) == 12 * sizeof(double), "packed dmat4x3 has no padding");
	static_assert(sizeof(glm::packed_t<glm::vec3>) == 3 * sizeof(float), "packed vec3 has no padding");

	{
		float const Data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		glm::mat3 const m(1, 2, 3, 4, 5, 6, 7, 8, 9);
		Error += GLM_CHECK_EQ(std::memcmp(glm::value_ptr(glm::to_packed(m)), Data, sizeof(Data)), 0);
		Error += GLM_CHECK_NEAR(glm::mat3(glm::to_aligned(glm::to_packed(m))), m, 0.0f);
	}
	{
		double const Data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		glm::dmat4x3 const m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
		Error += GLM_CHECK_EQ(std::memcmp(glm::value_ptr(glm::to_packed(m)), Data, sizeof(Data)), 0);
	}
	{
		int const Data[] = {1, 2, 3};
		glm::ivec3 const v(1, 2, 3);
		Error += GLM_CHECK_EQ(std::memcmp(glm::value_ptr(glm::to_packed(v)), Data, sizeof(Data)), 0);
		Error += GLM_CHECK_EQ(glm::ivec3(glm::to_packed(v)), v);
	}
	{
		glm::quat const q(1, 2, 3, 4);
		Error += GLM_CHECK_EQ(glm::quat(glm::to_packed(q)), q);
	}

	return Error;
}

// The make_* functions read exactly the number of components from the pointer, so exact-size sources are fine
// even when the default types are padded. The sources are on the heap so an over-read is sanitizer-visible.
static int test_make_pointer_exact()
{
	int Error = 0;

	{
		std::unique_ptr<float[]> const Data(new float[3]{1, 2, 3});
		Error += GLM_CHECK_NEAR(glm::make_vec3(Data.get()), glm::vec3(1, 2, 3), 0.0f);
	}
	{
		std::unique_ptr<double[]> const Data(new double[3]{1, 2, 3});
		Error += GLM_CHECK_NEAR(glm::make_vec3(Data.get()), glm::dvec3(1, 2, 3), 0.0);
	}
	{
		std::unique_ptr<float[]> const Data(new float[9]{1, 2, 3, 4, 5, 6, 7, 8, 9});
		Error += GLM_CHECK_NEAR(glm::make_mat3(Data.get()), glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9), 0.0f);
	}
	{
		std::unique_ptr<float[]> const Data(new float[4]{1, 2, 3, 4});
		glm::quat const q = glm::make_quat(Data.get());
		Error += GLM_CHECK_EQ(std::memcmp(glm::value_ptr(glm::to_packed(q)), Data.get(), 4 * sizeof(float)), 0);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += GLM_TEST(test_make_vec1());
	Error += GLM_TEST(test_make_vec2());
	Error += GLM_TEST(test_make_vec3());
	Error += GLM_TEST(test_make_vec4());
	Error += GLM_TEST(test_make_pointer_vec());
	Error += GLM_TEST(test_make_pointer_mat());
	Error += GLM_TEST(test_value_ptr_vec());
	Error += GLM_TEST(test_value_ptr_vec_const());
	Error += GLM_TEST(test_value_ptr_mat());
	Error += GLM_TEST(test_value_ptr_mat_const());
	Error += GLM_TEST(test_to_packed());
	Error += GLM_TEST(test_make_pointer_exact());

	return GLM_TEST_FINISH(Error);
}
