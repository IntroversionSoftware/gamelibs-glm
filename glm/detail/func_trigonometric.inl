#include "_vectorize.hpp"
#include <cmath>
#include <limits>
#include <utility>

namespace glm
{
	namespace detail
	{
		// Constants
		template<typename T> GLM_FUNC_QUALIFIER GLM_CONSTEXPR T pi() { return T(3.14159265358979323846264338327950288); }
		template<typename T> GLM_FUNC_QUALIFIER GLM_CONSTEXPR T half_pi() { return T(1.57079632679489661923132169163975144); }
		template<typename T> GLM_FUNC_QUALIFIER GLM_CONSTEXPR T two_pi() { return T(6.28318530717958647692528676655900576); }
		template<typename T> GLM_FUNC_QUALIFIER GLM_CONSTEXPR T quarter_pi() { return T(0.78539816339744830961566084581987572); }

		// Fast modulo function that's more efficient than std::fmod in many cases
		template<typename T>
		GLM_FUNC_QUALIFIER T fast_fmod(T x, T y) {
			// Assumes y > 0
			T n = std::floor(std::fabs(x) / y);
			return x - y * std::copysign(n, x);
		}

		// Basic range reduction to [-π, π]
		template<typename T>
		GLM_FUNC_QUALIFIER T reduce_to_minus_pi_pi(T x) {
			x = fast_fmod(x, two_pi<T>());

			if (x > pi<T>()) {
				x = x - two_pi<T>();
			}
			else if (x < -pi<T>()) {
				x = x + two_pi<T>();
			}

			return x;
		}

		// Helper to clamp values to [-1, 1]
		template<typename T>
		GLM_FUNC_QUALIFIER T clamp_unit(T x) {
			return std::max(T(-1.0), std::min(x, T(1.0)));
		}

		// Unified sincos implementation
		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_sincos_vec {
			// Structure to hold both sine and cosine results
			struct result {
				vec<L, T, Q> sin;
				vec<L, T, Q> cos;
			};

			GLM_FUNC_QUALIFIER static result call(vec<L, T, Q> const& angle) {
				result res;
				for (length_t i = 0; i < L; ++i) {
					// First, reduce the angle to [-π, π]
					T a0 = reduce_to_minus_pi_pi(angle[i]);
					// Unified range reduction for sin and cos:
					// For sine we need an angle in [-π/2, π/2].
					// For cosine we reduce to [0, π/2] and record a sign.
					T sin_r, cos_r, cos_sign;
					if (a0 > half_pi<T>()) {
						sin_r = pi<T>() - a0;  // Maps [π/2, π] to [0, π/2]
						cos_r = pi<T>() - a0;
						cos_sign = T(-1.0);       // cosine is negative in quadrant II
					}
					else if (a0 < -half_pi<T>()) {
						sin_r = -pi<T>() - a0; // Maps [-π, -π/2] to [-π/2, 0]
						cos_r = -pi<T>() - a0;
						cos_sign = T(-1.0);       // cosine is negative in quadrant III
					}
					else {
						sin_r = a0;
						// For cosine, we want a positive angle in [0, π/2]
						if (a0 < T(0)) {
							cos_r = -a0;
							cos_sign = T(1.0);
						}
						else {
							cos_r = a0;
							cos_sign = T(1.0);
						}
					}

					if constexpr (is_highp<Q>::value) {
						// High precision: defer to standard library functions.
						res.sin[i] = std::sin(angle[i]);
						res.cos[i] = std::cos(angle[i]);
					}
					else if constexpr (is_mediump<Q>::value) {
						// Sine approximation: 7th degree minimax for sin(x) on [-π/2, π/2]
						const T a3 = T(1.0) / T(6.0);      // 1/3!
						const T a5 = T(1.0) / T(120.0);     // 1/5!
						const T a7 = T(1.0) / T(5040.0);    // 1/7!
						T s = sin_r;
						T s2 = s * s;
						T sin_val = s * (T(1.0) - s2 * (a3 - s2 * (a5 - s2 * a7)));

						// Cosine approximation: 6th degree minimax for cos(x) on [0, π/2]
						const T a2 = T(1.0) / T(2.0);       // 1/2!
						const T a4 = T(1.0) / T(24.0);      // 1/4!
						const T a6 = T(1.0) / T(720.0);     // 1/6!
						T c = cos_r;
						T c2 = c * c;
						T cos_val = T(1.0) - c2 * (a2 - c2 * (a4 - c2 * a6));

						res.sin[i] = sin_val;
						res.cos[i] = cos_sign * cos_val;
					}
					else { // lowp
						// Sine approximation: 3rd degree minimax
						const T a3 = T(1.0) / T(6.0);
						T s = sin_r;
						T s2 = s * s;
						T sin_val = s * (T(1.0) - s2 * a3);

						// Cosine approximation: 4th degree minimax
						const T a2 = T(1.0) / T(2.0);
						const T a4 = T(1.0) / T(24.0);
						T c = cos_r;
						T c2 = c * c;
						T cos_val = T(1.0) - c2 * (a2 - c2 * a4);

						res.sin[i] = sin_val;
						res.cos[i] = cos_sign * cos_val;
					}
				}
				return res;
			}
		};

		// Separate sine template using the shared sincos implementation.
		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_sin_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				if (is_highp<Q>::value) {
					vec<L, T, Q> result;
					for (length_t i = 0; i < L; ++i) {
						result[i] = std::sin(angle[i]);
					}
					return result;
				}
				else {
					return compute_sincos_vec<L, T, Q, UseSimd>::call(angle).sin;
				}
			}
		};

		// Separate cosine template using the shared sincos implementation.
		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_cos_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				if (is_highp<Q>::value) {
					vec<L, T, Q> result;
					for (length_t i = 0; i < L; ++i) {
						result[i] = std::cos(angle[i]);
					}
					return result;
				}
				else {
					return compute_sincos_vec<L, T, Q, UseSimd>::call(angle).cos;
				}
			}
		};

		// Tangent is computed as sin/cos, with a safeguard for near-asymptotic values.
		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_tan_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				vec<L, T, Q> result;
				if (is_highp<Q>::value) {
					for (length_t i = 0; i < L; ++i) {
						result[i] = std::tan(angle[i]);
					}
				}
				else {
					// Compute sin and cos
					auto sc = compute_sincos_vec<L, T, Q, UseSimd>::call(angle);
					for (length_t i = 0; i < L; ++i) {
						// When cosine is too close to zero, return a large value with the proper sign.
						const T epsilon_threshold = T(1e-3);  // Adjust threshold as needed.
						if (std::abs(sc.cos[i]) < epsilon_threshold) {
							result[i] = std::copysign(T(1000000.0), sc.sin[i]);
						}
						else {
							result[i] = sc.sin[i] / sc.cos[i];
						}
					}
				}
				return result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_asin_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> result;
				for (length_t i = 0; i < L; ++i) {
					// Clamp the input and work with its absolute value.
					T x_clamped = clamp_unit(x[i]);
					T abs_x = std::abs(x_clamped);
					T asin_val = T(0);
					if constexpr (is_highp<Q>::value) {
						asin_val = std::asin(x_clamped);
					}
					else if constexpr (is_mediump<Q>::value) {
						if (abs_x <= T(0.5)) {
							// Taylor series for |x| <= 0.5
							T x2 = abs_x * abs_x;
							asin_val = abs_x + abs_x * x2 * (T(1.0) / T(6.0)
															 + x2 * (T(3.0) / T(40.0)
																	 + x2 * (T(5.0) / T(112.0))));
						}
						else {
							// For |x| > 0.5, use the transformation:
							// asin(x) ≈ π/2 - √(1 - |x|) * (1.408 + 0.145*(1 - |x|))
							T t = T(1.0) - abs_x;
							T sqrt_term = std::sqrt(t);
							T poly = T(1.408) + t * T(0.145);
							asin_val = half_pi<T>() - sqrt_term * poly;
						}
					}
					else { // lowp
						// Special case for values close to ±1
						if (abs_x > T(0.9)) {
							asin_val = half_pi<T>();
						}
						else {
							T x2 = abs_x * abs_x;
							// Improved polynomial approximation
							asin_val = abs_x + abs_x * x2 * T(0.1667) + abs_x * x2 * x2 * T(0.075);
						}
					}
					// Since we computed asin(abs(x)), we need to restore the sign
					result[i] = std::copysign(asin_val, x_clamped);
				}
				return result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_acos_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> result;
				if (is_highp<Q>::value) {
					for (length_t i = 0; i < L; ++i) {
						result[i] = std::acos(x[i]);
					}
				}
				else {
					vec<L, T, Q> asin_val = compute_asin_vec<L, T, Q, UseSimd>::call(x);
					for (length_t i = 0; i < L; ++i) {
						result[i] = half_pi<T>() - asin_val[i];
					}
				}
				return result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_atan_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> result;
				for (length_t i = 0; i < L; ++i) {
					T x_val = x[i];
					T abs_x = std::abs(x_val);
					T atan_val = T(0);

					if constexpr (is_highp<Q>::value) {
						atan_val = std::atan(abs_x);
					}
					else if constexpr (is_mediump<Q>::value) {
						// Range reduction: map x to z in [0,1] via
						//   atan(x) = π/2 - atan(1/x)  for x > 1.
						bool flip = abs_x > T(1.0);
						T z = flip ? T(1.0) / abs_x : abs_x;
						// Use a 9th-degree polynomial approximation:
						//   atan(z) ≈ z * (c0 + c1*z^2 + c2*z^4 + c3*z^6 + c4*z^8)
						// These coefficients (derived via a minimax process) yield
						// maximum errors on the order of 0.001 on [0,1].
						constexpr T c0 = T(0.999866);
						constexpr T c1 = T(-0.3302995);
						constexpr T c2 = T(0.1801410);
						constexpr T c3 = T(-0.0851330);
						constexpr T c4 = T(0.0208351);
						T z2 = z * z;
						T poly = c0 + z2 * (c1 + z2 * (c2 + z2 * (c3 + z2 * c4)));
						T approx = z * poly;
						atan_val = flip ? half_pi<T>() - approx : approx;
					}
					else { // lowp
						// Reduce domain: use atan(x) = π/2 - atan(1/x) for |x| > 1.
						bool flip = abs_x > T(1.0);
						T z = flip ? T(1.0) / abs_x : abs_x;

						// Fast approximation on [0, 1].
						// (π/4)*z + 0.273*z*(1 - z) is known to be effective in graphics.
						T approx = T(0.7853981633974483) * z + T(0.273) * z * (T(1.0) - z);

						// Reconstruct the range.
						atan_val = flip ? half_pi<T>() - approx : approx;
					}

					// Apply sign
					result[i] = std::copysign(atan_val, x_val);
				}
				return result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_atan2_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& y, vec<L, T, Q> const& x) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					// For highp, use the standard library:
					if constexpr (is_highp<Q>::value) {
						Result[i] = std::atan2(y[i], x[i]);
					}
					else { // mediump and lowp
						T xi = x[i];
						T yi = y[i];

						// Handle special cases
						if (xi == T(0)) {
							if (yi > T(0))
								Result[i] = half_pi<T>();
							else if (yi < T(0))
								Result[i] = -half_pi<T>();
							else
								Result[i] = T(0); // undefined; return 0
							continue;
						}
						if (yi == T(0)) {
							Result[i] = (xi >= T(0)) ? T(0) : pi<T>();
							continue;
						}

						// Compute r = y/x
						T r = yi / xi;
						T approx = T(0);

						if constexpr (is_mediump<Q>::value) {
							// Use the same 9th-degree polynomial approximation as in compute_atan_vec.
							bool flip = std::abs(r) > T(1.0);
							if (!flip) {
								T r2 = r * r;
								// Coefficients determined by minimax optimization
								constexpr T c0 = T(0.999866);
								constexpr T c1 = T(-0.3302995);
								constexpr T c2 = T(0.1801410);
								constexpr T c3 = T(-0.0851330);
								constexpr T c4 = T(0.0208351);
								approx = r * (c0 + r2 * (c1 + r2 * (c2 + r2 * (c3 + r2 * c4))));
							}
							else {
								// For |r|>1, use: atan(r) = sign(r)*(pi/2 - atan(1/|r|))
								T sign = (r >= T(0)) ? T(1) : T(-1);
								T inv = T(1) / std::abs(r);
								T inv2 = inv * inv;
								constexpr T c0 = T(0.999866);
								constexpr T c1 = T(-0.3302995);
								constexpr T c2 = T(0.1801410);
								constexpr T c3 = T(-0.0851330);
								constexpr T c4 = T(0.0208351);
								T approx_inv = inv * (c0 + inv2 * (c1 + inv2 * (c2 + inv2 * (c3 + inv2 * c4))));
								approx = sign * (half_pi<T>() - approx_inv);
							}
						}
						else { // lowp
							// Choose k = 0.5 to improve accuracy.
							constexpr T k = T(0.5);
							bool flip = std::abs(r) > T(1.0);
							if (!flip) {
								T r2 = r * r;
								approx = r * (T(1.0) + k * r2) / (T(1.0) + r2);
							}
							else {
								T sign = (r >= T(0)) ? T(1) : T(-1);
								// Compute approximant for atan(1/|r|)
								T inv = T(1) / std::abs(r);
								T inv2 = inv * inv;
								T approx_inv = inv * (T(1.0) + k * inv2) / (T(1.0) + inv2);
								approx = sign * (half_pi<T>() - approx_inv);
							}
						}

						// Quadrant correction: if x < 0, add or subtract pi.
						if (xi < T(0)) {
							approx = (yi >= T(0)) ? approx + pi<T>() : approx - pi<T>();
						}

						Result[i] = approx;
					}
				}
				return Result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_sinh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					T x = angle[i];
					if constexpr (is_highp<Q>::value || (is_mediump<Q>::value && L < 4)) {
						Result[i] = std::sinh(x);
					}
					else if constexpr (is_mediump<Q>::value) {
						Result[i] = (std::exp(x) - std::exp(-x)) * T(0.5);
					}
					else { // lowp
						// Simple approximation for sinh in lowp.
						if (std::abs(x) > T(3)) {
							T sign = (x >= T(0)) ? T(1) : T(-1);
							T exp_val = std::exp(std::abs(x));
							Result[i] = sign * exp_val * T(0.5);
						}
						else {
							// Use a 5th-order Taylor series: sinh(x) ≈ x + x³/3! + x⁵/5!
							T x2 = x * x;
							T x3 = x2 * x;
							T x5 = x3 * x2;
							const T a3 = T(1) / T(6);   // 1/3!
							const T a5 = T(1) / T(120);  // 1/5!
							Result[i] = x + x3 * a3 + x5 * a5;
						}
					}
				}
				return Result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_cosh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					T x = angle[i];
					if constexpr (is_highp<Q>::value || (is_mediump<Q>::value && L < 4)) {
						Result[i] = std::cosh(x);
					}
					else if constexpr (is_mediump<Q>::value) {
						Result[i] = (std::exp(x) + std::exp(-x)) * T(0.5);
					}
					else { // lowp
						// In the lowp branch for cosh:
						if (std::abs(x) > T(3)) {
							// For larger values, it's acceptable to use std::exp.
							T exp_val = std::exp(x);
							T exp_neg = std::exp(-x);
							Result[i] = (exp_val + exp_neg) * T(0.5);
						}
						else {
							// Use a 6th-order Taylor series for cosh(x):
							T x2 = x * x;
							T x4 = x2 * x2;
							T x6 = x4 * x2;
							const T t2 = T(1) / T(2);     // 1/2!
							const T t4 = T(1) / T(24);    // 1/4!
							const T t6 = T(1) / T(720);   // 1/6!
							Result[i] = T(1) + x2 * t2 + x4 * t4 + x6 * t6;
						}

					}
				}
				return Result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_tanh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& angle) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					// Choose implementation based on precision qualifier
					if constexpr (is_highp<Q>::value) {
						Result[i] = std::tanh(angle[i]);
					}
					else if constexpr (is_mediump<Q>::value) {
						// Implement tanh(x) = sinh(x) / cosh(x) = (exp(2x) - 1) / (exp(2x) + 1)
						T x = angle[i];
						T abs_x = std::abs(x);

						// For large values, tanh(x) approaches ±1
						if (abs_x > T(5)) {
							Result[i] = (x > T(0)) ? T(1) : T(-1);
						}
						// For medium to small values, use the exponential formula
						else if (abs_x > T(0.5)) {
							T exp_2x = std::exp(T(2) * x);
							Result[i] = (exp_2x - T(1)) / (exp_2x + T(1));
						}
						// For very small values, use polynomial approximation
						else {
							// tanh(x) ≈ x - x³/3 + 2x⁵/15 - ...
							T x2 = x * x;
							const T a3 = T(1) / T(3);
							const T a5 = T(2) / T(15);

							Result[i] = x * (T(1) - x2 * (a3 - x2 * a5));
						}
					}
					else { // lowp
						T x = angle[i];
						T abs_x = std::abs(x);
						if (abs_x > T(3)) {
							Result[i] = (x > T(0)) ? T(1) : T(-1);
						}
						else {
							// A Padé approximant for tanh(x):
							// tanh(x) ≈ x * (27 + x²) / (27 + 9x²)
							T x2 = x * x;
							Result[i] = x * (T(27) + x2) / (T(27) + T(9) * x2);
						}
					}
				}
				return Result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_asinh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					T value = x[i];
					if constexpr (is_highp<Q>::value) {
#if GLM_HAS_CXX11_STL
						Result[i] = std::asinh(value);
#else
						// asinh(x) = ln(x + sqrt(x² + 1))
						T sign = (value < T(0)) ? T(-1) : T(1);
						T abs_value = std::abs(value);
						Result[i] = sign * std::log(abs_value + std::sqrt(abs_value * abs_value + T(1)));
#endif
					}
					else if constexpr (is_mediump<Q>::value) {
						T sign = (value < T(0)) ? T(-1) : T(1);
						T abs_value = std::abs(value);

						if (abs_value > T(3)) {
							// For large |x|, use an improved asymptotic expansion
							// The correction term is important for precision but can be simplified
							Result[i] = sign * (std::log(T(2) * abs_value) + T(1) / (T(4) * abs_value * abs_value));
						}
						else if (abs_value < T(0.5)) {
							// For small |x|, use a more accurate approximation than lowp
							T x2 = abs_value * abs_value;
							// Use a simplified polynomial that's still accurate enough
							Result[i] = sign * abs_value * (T(1) - x2 * (T(1) / T(6)) + x2 * x2 * (T(3) / T(40)));
						}
						else {
							// For medium |x|, use a more efficient calculation than before
							// This is the standard formula but with fewer operations
							T x2 = abs_value * abs_value;
							T sqrt_term = std::sqrt(x2 + T(1));
							Result[i] = sign * std::log(abs_value + sqrt_term);
						}
					}
					else { // lowp
						T sign = (value < T(0)) ? T(-1) : T(1);
						T abs_value = std::abs(value);

						if (abs_value > T(3)) {
							// For large |x|, use the asymptotic behavior.
							Result[i] = sign * (std::log(T(2) * abs_value));
						}
						else {
							// Use the rational approximation which is already fast
							T x2 = abs_value * abs_value;
							Result[i] = sign * abs_value * (T(1) + T(0.12117) * x2) / (T(1) + T(0.2722) * x2);
						}
					}
				}
				return Result;
			}
		};

		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_acosh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					T value = x[i];

					// Choose implementation based on precision qualifier
					if constexpr (is_highp<Q>::value) {
#if GLM_HAS_CXX11_STL
						Result[i] = std::acosh(value);
#else
						// Use the definition: acosh(x) = ln(x + sqrt(x² - 1))

						// acosh is only defined for x >= 1
						if (value < T(1))
							Result[i] = T(0); // Undefined, return 0
						else
							Result[i] = std::log(value + std::sqrt(value * value - T(1)));
#endif
					}
					else if constexpr (is_mediump<Q>::value) {
						// Implement acosh(x) = ln(x + sqrt(x² - 1))

						// acosh is only defined for x >= 1
						if (value < T(1)) {
							Result[i] = T(0); // Undefined, return 0
						}
						// For values very close to 1, use a series expansion
						else if (value < T(1.1)) {
							T dx = value - T(1);

							// acosh(1+dx) ≈ sqrt(2*dx) * (1 - dx/12 + ...)
							Result[i] = std::sqrt(T(2) * dx) * (T(1) - dx / T(12));
						}
						// For large values, acosh(x) ≈ ln(2x)
						else if (value > T(10)) {
							Result[i] = std::log(T(2) * value);
						}
						// For medium range, use the logarithmic definition
						else {
							Result[i] = std::log(value + std::sqrt(value * value - T(1)));
						}
					}
					else { // lowp
						if (value < T(1)) {
							Result[i] = T(0); // Undefined for x < 1
						}
						else if (value < T(3)) {
							// Use a rational approximant for 1 <= x < 3.
							// Let y = x - 1; then for small y, acosh(x) ~ √(2y).
							// We propose:
							//   acosh(x) ≈ √(2*(x-1)) * (1 + 0.113*(x-1)) / (1 + 0.196*(x-1))
							T y = value - T(1);
							T sqrtTerm = std::sqrt(T(2) * y);
							Result[i] = sqrtTerm * (T(1) + T(0.113) * y) / (T(1) + T(0.196) * y);
						}
						else {
							// For x >= 3, use the asymptotic expansion:
							//   acosh(x) ≈ ln(2x) + 1/(4x²)
							Result[i] = std::log(T(2) * value) + T(1) / (T(4) * value * value);
						}
					}
				}
				return Result;
			}
		};

		// Vector implementation for inverse hyperbolic tangent
		template<length_t L, typename T, qualifier Q, bool UseSimd = false>
		struct compute_atanh_vec {
			GLM_FUNC_QUALIFIER static vec<L, T, Q> call(vec<L, T, Q> const& x) {
				vec<L, T, Q> Result;
				for (length_t i = 0; i < L; ++i) {
					T value = x[i];
					// Choose implementation based on precision qualifier
					if constexpr (is_highp<Q>::value) {
#if GLM_HAS_CXX11_STL
						Result[i] = std::atanh(value);
#else
						// Use the definition: atanh(x) = 0.5 * ln((1+x)/(1-x))

						// atanh is only defined for |x| < 1
						if (std::abs(value) >= T(1))
							Result[i] = T(0); // Undefined, return 0
						else
							Result[i] = T(0.5) * std::log((T(1) + value) / (T(1) - value));
#endif
					}
					else if constexpr (is_mediump<Q>::value) {
						// atanh is only defined for |x| < 1
						if (std::abs(value) >= T(1)) {
							Result[i] = T(0); // Undefined, return 0
						}
						// Use series expansion for very small |x|
						else if (std::abs(value) < T(0.25)) {
							T x2 = value * value;
							const T a3 = T(1) / T(3);
							const T a5 = T(1) / T(5);
							Result[i] = value * (T(1) + x2 * (a3 + x2 * a5));
						}
						// For values close to ±1, use a stable formulation via log1p
						else if (std::abs(value) > T(0.9)) {
							T sign = (value >= T(0)) ? T(1) : T(-1);
							T diff = T(1) - std::abs(value);
							// Compute atanh(x) = 0.5 * log1p((2|x|)/(1-|x|))
							Result[i] = sign * T(0.5) * std::log1p((T(2) * std::abs(value)) / diff);
						}
						// For medium range, use the straightforward definition.
						else {
							Result[i] = T(0.5) * std::log((T(1) + value) / (T(1) - value));
						}
					}
					else { // lowp
						// Simple approximation for atanh

						// atanh is only defined for |x| < 1
						if (std::abs(value) >= T(1)) {
							Result[i] = T(0); // Undefined, return 0
						}
						// For small to medium values, use a simplified approach
						else if (std::abs(value) < T(0.5)) {
							// Simplified approximation: x + x³/3
							T x2 = value * value;
							Result[i] = value * (T(1) + x2 / T(3));
						}
						// For values closer to ±1, use a simple approximation
						else {
							T sign = (value >= T(0)) ? T(1) : T(-1);
							Result[i] = sign * T(0.5) * std::log((T(1) + std::abs(value)) / (T(1) - std::abs(value)));
						}
					}
				}
				return Result;
			}
		};

	} // namespace detail

	// radians
	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType radians(genType degrees)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559 || GLM_CONFIG_UNRESTRICTED_FLOAT, "'radians' only accept floating-point input");

		return degrees * static_cast<genType>(0.01745329251994329576923690768489);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<L, T, Q> radians(vec<L, T, Q> const& v)
	{
		return detail::functor1<vec, L, T, T, Q>::call(radians, v);
	}

	// degrees
	template<typename genType>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR genType degrees(genType radians)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559 || GLM_CONFIG_UNRESTRICTED_FLOAT, "'degrees' only accept floating-point input");

		return radians * static_cast<genType>(57.295779513082320876798154814105);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec<L, T, Q> degrees(vec<L, T, Q> const& v)
	{
		return detail::functor1<vec, L, T, T, Q>::call(degrees, v);
	}

	// Scalar sin implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType sin(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return sin(vec<1, genType, Q>(angle)).x;
	}

	// Vector sin implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> sin(vec<L, T, Q> const& angle) {
		return detail::compute_sin_vec<L, T, Q, detail::is_aligned<Q>::value>::call(angle);
	}

	// Scalar cos implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType cos(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return cos(vec<1, genType, Q>(angle)).x;
	}

	// Vector cos implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> cos(vec<L, T, Q> const& angle) {
		return detail::compute_cos_vec<L, T, Q, detail::is_aligned<Q>::value>::call(angle);
	}

	// Scalar tan implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType tan(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return tan(vec<1, genType, Q>(angle)).x;
	}

	// Vector tan implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> tan(vec<L, T, Q> const& angle) {
		return detail::compute_tan_vec<L, T, Q>::call(angle);
	}

	// Scalar asin implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType asin(genType x) {
		// Route through the vector implementation with a 1-wide vector
		return asin(vec<1, genType, Q>(x)).x;
	}

	// Vector asin implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> asin(vec<L, T, Q> const& x) {
		return detail::compute_asin_vec<L, T, Q>::call(x);
	}

	// Scalar acos implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType acos(genType x) {
		// Route through the vector implementation with a 1-wide vector
		return acos(vec<1, genType, Q>(x)).x;
	}

	// Vector acos implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> acos(vec<L, T, Q> const& x) {
		return detail::compute_acos_vec<L, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

	// Scalar atan implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType atan(genType y_over_x) {
		// Route through the vector implementation with a 1-wide vector
		return atan(vec<1, genType, Q>(y_over_x)).x;
	}

	// Vector atan implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> atan(vec<L, T, Q> const& y_over_x) {
		return detail::compute_atan_vec<L, T, Q, detail::is_aligned<Q>::value>::call(y_over_x);
	}

	// Scalar atan2 implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType atan(genType y, genType x) {
		// Route through the vector implementation with a 1-wide vector
		return atan(vec<1, genType, Q>(y), vec<1, genType, Q>(x)).x;
	}

	// Vector atan2 implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> atan(vec<L, T, Q> const& y, vec<L, T, Q> const& x) {
		return detail::compute_atan2_vec<L, T, Q, detail::is_aligned<Q>::value>::call(y, x);
	}

	// Scalar sinh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType sinh(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return sinh(vec<1, genType, Q>(angle)).x;
	}

	// Vector sinh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> sinh(vec<L, T, Q> const& angle) {
		return detail::compute_sinh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(angle);
	}

	// Scalar cosh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType cosh(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return cosh(vec<1, genType, Q>(angle)).x;
	}

	// Vector cosh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> cosh(vec<L, T, Q> const& angle) {
		return detail::compute_cosh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(angle);
	}

	// Scalar tanh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType tanh(genType angle) {
		// Route through the vector implementation with a 1-wide vector
		return tanh(vec<1, genType, Q>(angle)).x;
	}

	// Vector tanh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> tanh(vec<L, T, Q> const& angle) {
		return detail::compute_tanh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(angle);
	}

	// Scalar asinh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType asinh(genType x) {
		// Route through the vector implementation with a 1-wide vector
		return asinh(vec<1, genType, Q>(x)).x;
	}

	// Vector asinh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> asinh(vec<L, T, Q> const& x) {
		return detail::compute_asinh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

	// Scalar acosh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType acosh(genType x) {
		// Route through the vector implementation with a 1-wide vector
		return acosh(vec<1, genType, Q>(x)).x;
	}

	// Vector acosh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> acosh(vec<L, T, Q> const& x) {
		return detail::compute_acosh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

	// Scalar atanh implementation with precision qualifier
	template<typename genType, qualifier Q>
	GLM_FUNC_QUALIFIER genType atanh(genType x) {
		// Route through the vector implementation with a 1-wide vector
		return atanh(vec<1, genType, Q>(x)).x;
	}

	// Vector atanh implementation with precision from the vector's qualifier
	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> atanh(vec<L, T, Q> const& x) {
		return detail::compute_atanh_vec<L, T, Q, detail::is_aligned<Q>::value>::call(x);
	}

}//namespace glm

#if GLM_CONFIG_SIMD == GLM_ENABLE
#	include "func_trigonometric_simd.inl"
#endif

