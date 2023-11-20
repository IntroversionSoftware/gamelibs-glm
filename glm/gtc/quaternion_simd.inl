#if GLM_ARCH & GLM_ARCH_CLANG_BIT

namespace glm
{
namespace detail
{

}//namespace detail

template<>
template<>
GLM_FUNC_QUALIFIER qua<float, aligned_highp> &qua<float, aligned_highp>::operator*=(
	qua<float, aligned_highp> const &q)
{
	const glm_f32vec4 l = data;
	const glm_f32vec4 r = q.data;

	constexpr glm_f32vec4 pnpn = { 1.0, -1.0f,  1.0f, -1.0f};
	constexpr glm_f32vec4 ppnn = { 1.0,  1.0f, -1.0f, -1.0f};
	constexpr glm_f32vec4 nppn = {-1.0,  1.0f,  1.0f, -1.0f};

	data =
		l[3] * r +
		pnpn * l[0] * __builtin_shufflevector(r, r, 3, 2, 1, 0) +
		ppnn * l[1] * __builtin_shufflevector(r, r, 2, 3, 0, 1) +
		nppn * l[2] * __builtin_shufflevector(r, r, 1, 0, 3, 2);

	return *this;
}

template<>
GLM_FUNC_QUALIFIER mat<3, 3, float, aligned_highp> mat3_cast(qua<float, aligned_highp> const &_q)
{
	const glm_f32vec4 q = _q.data;

	const glm_f32vec4 r0 =
		glm_f32vec4{ 1.f, 0.f, 0.f, 0.f } +
		glm_f32vec4{ -2.f, 2.f, 2.f, 0.f } *
			__builtin_shufflevector(q, q, 1, 0, 0, 0) *
			__builtin_shufflevector(q, q, 1, 1, 2, 2) +
		glm_f32vec4{ -2.f, 2.f, -2.f, 0.f } *
			__builtin_shufflevector(q, q, 2, 3, 3, 3) *
			__builtin_shufflevector(q, q, 2, 2, 1, 1);

	const glm_f32vec4 r1 =
		glm_f32vec4{ 0.f, 1.f, 0.f, 0.f } +
		glm_f32vec4{ 2.f, -2.f, 2.f, 0.f } *
			__builtin_shufflevector(q, q, 0, 0, 1, 1) *
			__builtin_shufflevector(q, q, 1, 0, 2, 2) +
		glm_f32vec4{ -2.f, -2.f, 2.f, 0.f } *
			__builtin_shufflevector(q, q, 3, 2, 3, 3) *
			__builtin_shufflevector(q, q, 2, 2, 0, 0);

	const glm_f32vec4 r2 =
		glm_f32vec4{ 0.f, 0.f, 1.f, 0.f } +
		glm_f32vec4{ 2.f, 2.f, -2.f, 0.f } *
			__builtin_shufflevector(q, q, 0, 1, 0, 0) *
			__builtin_shufflevector(q, q, 2, 2, 0, 0) +
		glm_f32vec4{ 2.f, -2.f, -2.f, 0.f } *
			__builtin_shufflevector(q, q, 3, 3, 1, 1) *
			__builtin_shufflevector(q, q, 1, 0, 1, 1);

	mat<3, 3, float, aligned_highp> Result;
	Result[0].data = r0.xyz;
	Result[1].data = r1.xyz;
	Result[2].data = r2.xyz;
	return Result;
}

}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
