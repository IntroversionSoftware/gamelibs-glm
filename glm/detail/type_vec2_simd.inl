#if GLM_ARCH & GLM_ARCH_CLANG_BIT
namespace glm {
namespace detail {

	template<typename T, qualifier Q>
	struct compute_vec2_add<T, Q, true>
	{
		static
		vec<2, T, Q>
		call(vec<2, T, Q> const& a, vec<2, T, Q> const& b)
		{
			vec<2, T, Q> Result;
			Result.data = a.data + b.data;
			return Result;
		}
	};

	template<typename T, qualifier Q>
	struct compute_vec2_sub<T, Q, true>
	{
		static vec<2, T, Q> call(vec<2, T, Q> const& a, vec<2, T, Q> const& b)
		{
			vec<2, T, Q> Result;
			Result.data = a.data - b.data;
			return Result;
		}
	};

	template<typename T, qualifier Q>
	struct compute_vec2_mul<T, Q, true>
	{
		static vec<2, T, Q> call(vec<2, T, Q> const& a, vec<2, T, Q> const& b)
		{
			vec<2, T, Q> Result;
			Result.data = a.data * b.data;
			return Result;
		}
	};

	template<typename T, qualifier Q>
	struct compute_vec2_div<T, Q, true>
	{
		static vec<2, T, Q> call(vec<2, T, Q> const& a, vec<2, T, Q> const& b)
		{
			vec<2, T, Q> Result;
			Result.data = a.data / b.data;
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_vec2_equal<float, Q, false, 32, true>
	{
		static bool call(vec<2, float, Q> const& v1, vec<2, float, Q> const& v2)
		{
			const glm_f32vec2 vsub = v1.data - v2.data;
			const glm_f32vec2 vabs = __builtin_elementwise_abs(vsub);
			float sum = 0.0f;
			for (int i = 0; i < 2; i++)
				sum += vabs[i];
			return sum < 0.0001f;
		}
	};

	template<qualifier Q>
	struct compute_vec2_equal<uint, Q, false, 32, true>
	{
		static bool call(vec<2, uint, Q> const& v1, vec<2, uint, Q> const& v2)
		{
			return v1.data == v2.data;
		}
	};

	template<qualifier Q>
	struct compute_vec2_equal<int, Q, false, 32, true>
	{
		static bool call(vec<2, int, Q> const& v1, vec<2, int, Q> const& v2)
		{
			return v1.data == v2.data;
		}
	};

	template<qualifier Q>
	struct compute_vec2_nequal<float, Q, false, 32, true>
	{
		static bool call(vec<2, float, Q> const& v1, vec<2, float, Q> const& v2)
		{
			return !compute_vec2_equal<float, Q, false, 32, true>::call(v1, v2);
		}
	};

	template<qualifier Q>
	struct compute_vec2_nequal<uint, Q, false, 32, true>
	{
		static bool call(vec<2, uint, Q> const& v1, vec<2, uint, Q> const& v2)
		{
			return !compute_vec2_equal<uint, Q, false, 32, true>::call(v1, v2);
		}
	};

	template<qualifier Q>
	struct compute_vec2_nequal<int, Q, false, 32, true>
	{
		static bool call(vec<2, int, Q> const& v1, vec<2, int, Q> const& v2)
		{
			return !compute_vec2_equal<int, Q, false, 32, true>::call(v1, v2);
		}
	};

}//namespace detail

#if !GLM_CONFIG_XYZW_ONLY
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_lowp>::vec(float _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_mediump>::vec(float _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(float _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_lowp>::vec(int _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_mediump>::vec(int _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_highp>::vec(int _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_lowp>::vec(uint _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_mediump>::vec(uint _s) :
		data(_s)
	{}

	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_highp>::vec(uint _s) :
		data(_s)
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(const vec<2, float, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(const vec<2, int, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(const vec<2, uint, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_highp>::vec(const vec<2, float, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_i32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_highp>::vec(const vec<2, int, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_i32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, int, aligned_highp>::vec(const vec<2, uint, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_i32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_highp>::vec(const vec<2, float, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_u32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_highp>::vec(const vec<2, int, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_u32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, uint, aligned_highp>::vec(const vec<2, uint, aligned_highp> &rhs) :
		data(__builtin_convertvector(rhs.data, glm_u32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_lowp>::vec(float _x, float _y) :
		data{ _x, _y }
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_mediump>::vec(float _x, float _y) :
		data{ _x, _y }
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(float _x, float _y) :
		data{ _x, _y }
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_lowp>::vec(int _x, int _y) :
		data(__builtin_convertvector((vec<2, int, aligned_lowp>(_x, _y).data), glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_mediump>::vec(int _x, int _y) :
		data(__builtin_convertvector((vec<2, int, aligned_mediump>(_x, _y).data), glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(int _x, int _y) :
		data(__builtin_convertvector((vec<2, int, aligned_highp>(_x, _y).data), glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_lowp>::vec(uint _x, uint _y) :
		data(__builtin_convertvector((vec<2, uint, aligned_lowp>(_x, _y).data), glm_f32vec2))
	{}

	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_mediump>::vec(uint _x, uint _y) :
		data(__builtin_convertvector((vec<2, uint, aligned_mediump>(_x, _y).data), glm_f32vec2))
	{}


	template<>
	template<>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<2, float, aligned_highp>::vec(uint _x, uint _y) :
		data(__builtin_convertvector((vec<2, uint, aligned_highp>(_x, _y).data), glm_f32vec2))
	{}


#endif
}//namespace glm

#endif // GLM_ARCH & GLM_ARCH_CLANG_BIT
