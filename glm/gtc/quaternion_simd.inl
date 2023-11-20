#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm
{
namespace detail
{

}//namespace detail

template<>
GLM_FUNC_QUALIFIER mat<3, 3, float, aligned_highp> mat3_cast(qua<float, aligned_highp> const &_q)
{
	qua<float, aligned_highp> q = _q;
	mat<3, 3, float, aligned_highp> Result;

	glm_f32vec4 r0 =
		glm_f32vec4{ 1.f, 0.f, 0.f, 0.f } +
		glm_f32vec4{ -2.f, 2.f, 2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 1, 0, 0, 0) *
			__builtin_shufflevector(q.data, q.data, 1, 1, 2, 2) +
		glm_f32vec4{ -2.f, 2.f, -2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 2, 3, 3, 3) *
			__builtin_shufflevector(q.data, q.data, 2, 2, 1, 1);

	glm_f32vec4 r1 =
		glm_f32vec4{ 0.f, 1.f, 0.f, 0.f } +
		glm_f32vec4{ 2.f, -2.f, 2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 0, 0, 1, 1) *
			__builtin_shufflevector(q.data, q.data, 1, 0, 2, 2) +
		glm_f32vec4{ -2.f, -2.f, 2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 3, 2, 3, 3) *
			__builtin_shufflevector(q.data, q.data, 2, 2, 0, 0);

	glm_f32vec4 r2 =
		glm_f32vec4{ 0.f, 0.f, 1.f, 0.f } +
		glm_f32vec4{ 2.f, 2.f, -2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 0, 1, 0, 0) *
			__builtin_shufflevector(q.data, q.data, 2, 2, 0, 0) +
		glm_f32vec4{ 2.f, -2.f, -2.f, 0.f } *
			__builtin_shufflevector(q.data, q.data, 3, 3, 1, 1) *
			__builtin_shufflevector(q.data, q.data, 1, 0, 1, 1);

	Result[0].data = r0.xyz;
	Result[1].data = r1.xyz;
	Result[2].data = r2.xyz;

	return Result;
}

}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
