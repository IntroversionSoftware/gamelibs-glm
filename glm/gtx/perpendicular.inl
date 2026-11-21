/// @ref gtx_perpendicular

namespace glm
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType perp(genType const& x, genType const& Normal)
	{
		return x - proj(x, Normal);
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> anyPerpendicular(vec<3, T, Q> const& v)
	{
		// For a unit n, (1 + s n.x^2 a, s n.x n.y a, -s n.x) with s = sign(n.z) and a = -1 / (s + n.z) is
		// a unit vector perpendicular to n; |s + n.z| >= 1, so the division is always well conditioned.
		vec<3, T, Q> const n = safeNormalize(v);
		T const s = n.z < static_cast<T>(0) ? static_cast<T>(-1) : static_cast<T>(1);
		T const a = static_cast<T>(-1) / (s + n.z);
		return vec<3, T, Q>(static_cast<T>(1) + s * n.x * n.x * a, s * n.x * n.y * a, -s * n.x);
	}
}//namespace glm
