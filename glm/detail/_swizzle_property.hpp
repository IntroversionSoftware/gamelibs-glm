/// @ref core
/// @file glm/detail/_swizzle_property.hpp

#pragma once

// Swizzles as properties, with component properties (see setup.hpp) and GLM_FORCE_SWIZZLE: v.xz is
// a property whose accessors read and write the components it names.
//
//   glm::vec2 a = v.xz;   // auto a = v.xz is a vec2 too
//   v.xz = a;             // assigns x and z
//   v.zx += a;            // reads z and x, adds, and writes them back
//   v.xz = 0.0f;          // assigns 0 to x and z
//
// A swizzle that names a component more than once (v.xx) has no setter, so assigning to it doesn't
// compile. v.xz() is the same vec2, as with the proxy swizzles these replace: the vector types have
// an operator() that returns the vector.
//
// GLM_SWIZZLE_PROPERTY<N>(I..., names) declares the N-component swizzle of components I... under
// each of the names (one per naming set: xyzw, rgba, stpq), and its getter; the _RW variant also
// declares its setter. GLM_SWIZZLE_PROPERTIES<L> declares all the swizzles of an L-component vector.

#define GLM_SWIZZLE_GET2(I, J) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<2, T, Q> _get##I##J() const \
	{ return vec<2, T, Q>(_get##I(), _get##J()); }
#define GLM_SWIZZLE_GET3(I, J, K) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<3, T, Q> _get##I##J##K() const \
	{ return vec<3, T, Q>(_get##I(), _get##J(), _get##K()); }
#define GLM_SWIZZLE_GET4(I, J, K, M) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<4, T, Q> _get##I##J##K##M() const \
	{ return vec<4, T, Q>(_get##I(), _get##J(), _get##K(), _get##M()); }

#define GLM_SWIZZLE_PROPERTY2(I, J, ...) \
	GLM_SWIZZLE_GET2(I, J) \
	__declspec(property(get = _get##I##J)) vec<2, T, Q> __VA_ARGS__;
#define GLM_SWIZZLE_PROPERTY3(I, J, K, ...) \
	GLM_SWIZZLE_GET3(I, J, K) \
	__declspec(property(get = _get##I##J##K)) vec<3, T, Q> __VA_ARGS__;
#define GLM_SWIZZLE_PROPERTY4(I, J, K, M, ...) \
	GLM_SWIZZLE_GET4(I, J, K, M) \
	__declspec(property(get = _get##I##J##K##M)) vec<4, T, Q> __VA_ARGS__;

#define GLM_SWIZZLE_PROPERTY2_RW(I, J, ...) \
	GLM_SWIZZLE_GET2(I, J) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J(vec<2, T, Q> const& _v) \
	{ _put##I(_v.x); _put##J(_v.y); } \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J(T _s) { _put##I(_s); _put##J(_s); } \
	__declspec(property(get = _get##I##J, put = _put##I##J)) vec<2, T, Q> __VA_ARGS__;
#define GLM_SWIZZLE_PROPERTY3_RW(I, J, K, ...) \
	GLM_SWIZZLE_GET3(I, J, K) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J##K(vec<3, T, Q> const& _v) \
	{ _put##I(_v.x); _put##J(_v.y); _put##K(_v.z); } \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J##K(T _s) { _put##I(_s); _put##J(_s); _put##K(_s); } \
	__declspec(property(get = _get##I##J##K, put = _put##I##J##K)) vec<3, T, Q> __VA_ARGS__;
#define GLM_SWIZZLE_PROPERTY4_RW(I, J, K, M, ...) \
	GLM_SWIZZLE_GET4(I, J, K, M) \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J##K##M(vec<4, T, Q> const& _v) \
	{ _put##I(_v.x); _put##J(_v.y); _put##K(_v.z); _put##M(_v.w); } \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR void _put##I##J##K##M(T _s) { _put##I(_s); _put##J(_s); _put##K(_s); _put##M(_s); } \
	__declspec(property(get = _get##I##J##K##M, put = _put##I##J##K##M)) vec<4, T, Q> __VA_ARGS__;

#define GLM_SWIZZLE_PROPERTIES2 \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<2, T, Q> operator()() const { return *this; } \
	GLM_SWIZZLE_PROPERTY2(0, 0, xx, rr, ss) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 1, xy, rg, st) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 0, yx, gr, ts) \
	GLM_SWIZZLE_PROPERTY2(1, 1, yy, gg, tt) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 0, xxx, rrr, sss) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 1, xxy, rrg, sst) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 0, xyx, rgr, sts) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 1, xyy, rgg, stt) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 0, yxx, grr, tss) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 1, yxy, grg, tst) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 0, yyx, ggr, tts) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 1, yyy, ggg, ttt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 0, xxxx, rrrr, ssss) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 1, xxxy, rrrg, ssst) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 0, xxyx, rrgr, ssts) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 1, xxyy, rrgg, sstt) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 0, xyxx, rgrr, stss) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 1, xyxy, rgrg, stst) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 0, xyyx, rggr, stts) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 1, xyyy, rggg, sttt) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 0, yxxx, grrr, tsss) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 1, yxxy, grrg, tsst) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 0, yxyx, grgr, tsts) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 1, yxyy, grgg, tstt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 0, yyxx, ggrr, ttss) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 1, yyxy, ggrg, ttst) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 0, yyyx, gggr, ttts) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 1, yyyy, gggg, tttt)

#define GLM_SWIZZLE_PROPERTIES3 \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<3, T, Q> operator()() const { return *this; } \
	GLM_SWIZZLE_PROPERTY2(0, 0, xx, rr, ss) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 1, xy, rg, st) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 2, xz, rb, sp) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 0, yx, gr, ts) \
	GLM_SWIZZLE_PROPERTY2(1, 1, yy, gg, tt) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 2, yz, gb, tp) \
	GLM_SWIZZLE_PROPERTY2_RW(2, 0, zx, br, ps) \
	GLM_SWIZZLE_PROPERTY2_RW(2, 1, zy, bg, pt) \
	GLM_SWIZZLE_PROPERTY2(2, 2, zz, bb, pp) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 0, xxx, rrr, sss) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 1, xxy, rrg, sst) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 2, xxz, rrb, ssp) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 0, xyx, rgr, sts) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 1, xyy, rgg, stt) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 1, 2, xyz, rgb, stp) \
	GLM_SWIZZLE_PROPERTY3(0, 2, 0, xzx, rbr, sps) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 2, 1, xzy, rbg, spt) \
	GLM_SWIZZLE_PROPERTY3(0, 2, 2, xzz, rbb, spp) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 0, yxx, grr, tss) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 1, yxy, grg, tst) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 0, 2, yxz, grb, tsp) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 0, yyx, ggr, tts) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 1, yyy, ggg, ttt) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 2, yyz, ggb, ttp) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 2, 0, yzx, gbr, tps) \
	GLM_SWIZZLE_PROPERTY3(1, 2, 1, yzy, gbg, tpt) \
	GLM_SWIZZLE_PROPERTY3(1, 2, 2, yzz, gbb, tpp) \
	GLM_SWIZZLE_PROPERTY3(2, 0, 0, zxx, brr, pss) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 0, 1, zxy, brg, pst) \
	GLM_SWIZZLE_PROPERTY3(2, 0, 2, zxz, brb, psp) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 1, 0, zyx, bgr, pts) \
	GLM_SWIZZLE_PROPERTY3(2, 1, 1, zyy, bgg, ptt) \
	GLM_SWIZZLE_PROPERTY3(2, 1, 2, zyz, bgb, ptp) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 0, zzx, bbr, pps) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 1, zzy, bbg, ppt) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 2, zzz, bbb, ppp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 0, xxxx, rrrr, ssss) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 1, xxxy, rrrg, ssst) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 2, xxxz, rrrb, sssp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 0, xxyx, rrgr, ssts) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 1, xxyy, rrgg, sstt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 2, xxyz, rrgb, sstp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 0, xxzx, rrbr, ssps) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 1, xxzy, rrbg, sspt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 2, xxzz, rrbb, sspp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 0, xyxx, rgrr, stss) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 1, xyxy, rgrg, stst) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 2, xyxz, rgrb, stsp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 0, xyyx, rggr, stts) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 1, xyyy, rggg, sttt) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 2, xyyz, rggb, sttp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 0, xyzx, rgbr, stps) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 1, xyzy, rgbg, stpt) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 2, xyzz, rgbb, stpp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 0, xzxx, rbrr, spss) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 1, xzxy, rbrg, spst) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 2, xzxz, rbrb, spsp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 0, xzyx, rbgr, spts) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 1, xzyy, rbgg, sptt) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 2, xzyz, rbgb, sptp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 0, xzzx, rbbr, spps) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 1, xzzy, rbbg, sppt) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 2, xzzz, rbbb, sppp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 0, yxxx, grrr, tsss) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 1, yxxy, grrg, tsst) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 2, yxxz, grrb, tssp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 0, yxyx, grgr, tsts) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 1, yxyy, grgg, tstt) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 2, yxyz, grgb, tstp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 0, yxzx, grbr, tsps) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 1, yxzy, grbg, tspt) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 2, yxzz, grbb, tspp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 0, yyxx, ggrr, ttss) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 1, yyxy, ggrg, ttst) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 2, yyxz, ggrb, ttsp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 0, yyyx, gggr, ttts) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 1, yyyy, gggg, tttt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 2, yyyz, gggb, tttp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 0, yyzx, ggbr, ttps) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 1, yyzy, ggbg, ttpt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 2, yyzz, ggbb, ttpp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 0, yzxx, gbrr, tpss) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 1, yzxy, gbrg, tpst) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 2, yzxz, gbrb, tpsp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 0, yzyx, gbgr, tpts) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 1, yzyy, gbgg, tptt) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 2, yzyz, gbgb, tptp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 0, yzzx, gbbr, tpps) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 1, yzzy, gbbg, tppt) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 2, yzzz, gbbb, tppp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 0, zxxx, brrr, psss) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 1, zxxy, brrg, psst) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 2, zxxz, brrb, pssp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 0, zxyx, brgr, psts) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 1, zxyy, brgg, pstt) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 2, zxyz, brgb, pstp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 0, zxzx, brbr, psps) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 1, zxzy, brbg, pspt) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 2, zxzz, brbb, pspp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 0, zyxx, bgrr, ptss) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 1, zyxy, bgrg, ptst) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 2, zyxz, bgrb, ptsp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 0, zyyx, bggr, ptts) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 1, zyyy, bggg, pttt) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 2, zyyz, bggb, pttp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 0, zyzx, bgbr, ptps) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 1, zyzy, bgbg, ptpt) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 2, zyzz, bgbb, ptpp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 0, zzxx, bbrr, ppss) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 1, zzxy, bbrg, ppst) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 2, zzxz, bbrb, ppsp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 0, zzyx, bbgr, ppts) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 1, zzyy, bbgg, pptt) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 2, zzyz, bbgb, pptp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 0, zzzx, bbbr, ppps) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 1, zzzy, bbbg, pppt) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 2, zzzz, bbbb, pppp)

#define GLM_SWIZZLE_PROPERTIES4 \
	GLM_COMPONENT_INLINE GLM_CONSTEXPR vec<4, T, Q> operator()() const { return *this; } \
	GLM_SWIZZLE_PROPERTY2(0, 0, xx, rr, ss) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 1, xy, rg, st) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 2, xz, rb, sp) \
	GLM_SWIZZLE_PROPERTY2_RW(0, 3, xw, ra, sq) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 0, yx, gr, ts) \
	GLM_SWIZZLE_PROPERTY2(1, 1, yy, gg, tt) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 2, yz, gb, tp) \
	GLM_SWIZZLE_PROPERTY2_RW(1, 3, yw, ga, tq) \
	GLM_SWIZZLE_PROPERTY2_RW(2, 0, zx, br, ps) \
	GLM_SWIZZLE_PROPERTY2_RW(2, 1, zy, bg, pt) \
	GLM_SWIZZLE_PROPERTY2(2, 2, zz, bb, pp) \
	GLM_SWIZZLE_PROPERTY2_RW(2, 3, zw, ba, pq) \
	GLM_SWIZZLE_PROPERTY2_RW(3, 0, wx, ar, qs) \
	GLM_SWIZZLE_PROPERTY2_RW(3, 1, wy, ag, qt) \
	GLM_SWIZZLE_PROPERTY2_RW(3, 2, wz, ab, qp) \
	GLM_SWIZZLE_PROPERTY2(3, 3, ww, aa, qq) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 0, xxx, rrr, sss) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 1, xxy, rrg, sst) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 2, xxz, rrb, ssp) \
	GLM_SWIZZLE_PROPERTY3(0, 0, 3, xxw, rra, ssq) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 0, xyx, rgr, sts) \
	GLM_SWIZZLE_PROPERTY3(0, 1, 1, xyy, rgg, stt) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 1, 2, xyz, rgb, stp) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 1, 3, xyw, rga, stq) \
	GLM_SWIZZLE_PROPERTY3(0, 2, 0, xzx, rbr, sps) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 2, 1, xzy, rbg, spt) \
	GLM_SWIZZLE_PROPERTY3(0, 2, 2, xzz, rbb, spp) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 2, 3, xzw, rba, spq) \
	GLM_SWIZZLE_PROPERTY3(0, 3, 0, xwx, rar, sqs) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 3, 1, xwy, rag, sqt) \
	GLM_SWIZZLE_PROPERTY3_RW(0, 3, 2, xwz, rab, sqp) \
	GLM_SWIZZLE_PROPERTY3(0, 3, 3, xww, raa, sqq) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 0, yxx, grr, tss) \
	GLM_SWIZZLE_PROPERTY3(1, 0, 1, yxy, grg, tst) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 0, 2, yxz, grb, tsp) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 0, 3, yxw, gra, tsq) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 0, yyx, ggr, tts) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 1, yyy, ggg, ttt) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 2, yyz, ggb, ttp) \
	GLM_SWIZZLE_PROPERTY3(1, 1, 3, yyw, gga, ttq) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 2, 0, yzx, gbr, tps) \
	GLM_SWIZZLE_PROPERTY3(1, 2, 1, yzy, gbg, tpt) \
	GLM_SWIZZLE_PROPERTY3(1, 2, 2, yzz, gbb, tpp) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 2, 3, yzw, gba, tpq) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 3, 0, ywx, gar, tqs) \
	GLM_SWIZZLE_PROPERTY3(1, 3, 1, ywy, gag, tqt) \
	GLM_SWIZZLE_PROPERTY3_RW(1, 3, 2, ywz, gab, tqp) \
	GLM_SWIZZLE_PROPERTY3(1, 3, 3, yww, gaa, tqq) \
	GLM_SWIZZLE_PROPERTY3(2, 0, 0, zxx, brr, pss) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 0, 1, zxy, brg, pst) \
	GLM_SWIZZLE_PROPERTY3(2, 0, 2, zxz, brb, psp) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 0, 3, zxw, bra, psq) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 1, 0, zyx, bgr, pts) \
	GLM_SWIZZLE_PROPERTY3(2, 1, 1, zyy, bgg, ptt) \
	GLM_SWIZZLE_PROPERTY3(2, 1, 2, zyz, bgb, ptp) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 1, 3, zyw, bga, ptq) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 0, zzx, bbr, pps) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 1, zzy, bbg, ppt) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 2, zzz, bbb, ppp) \
	GLM_SWIZZLE_PROPERTY3(2, 2, 3, zzw, bba, ppq) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 3, 0, zwx, bar, pqs) \
	GLM_SWIZZLE_PROPERTY3_RW(2, 3, 1, zwy, bag, pqt) \
	GLM_SWIZZLE_PROPERTY3(2, 3, 2, zwz, bab, pqp) \
	GLM_SWIZZLE_PROPERTY3(2, 3, 3, zww, baa, pqq) \
	GLM_SWIZZLE_PROPERTY3(3, 0, 0, wxx, arr, qss) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 0, 1, wxy, arg, qst) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 0, 2, wxz, arb, qsp) \
	GLM_SWIZZLE_PROPERTY3(3, 0, 3, wxw, ara, qsq) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 1, 0, wyx, agr, qts) \
	GLM_SWIZZLE_PROPERTY3(3, 1, 1, wyy, agg, qtt) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 1, 2, wyz, agb, qtp) \
	GLM_SWIZZLE_PROPERTY3(3, 1, 3, wyw, aga, qtq) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 2, 0, wzx, abr, qps) \
	GLM_SWIZZLE_PROPERTY3_RW(3, 2, 1, wzy, abg, qpt) \
	GLM_SWIZZLE_PROPERTY3(3, 2, 2, wzz, abb, qpp) \
	GLM_SWIZZLE_PROPERTY3(3, 2, 3, wzw, aba, qpq) \
	GLM_SWIZZLE_PROPERTY3(3, 3, 0, wwx, aar, qqs) \
	GLM_SWIZZLE_PROPERTY3(3, 3, 1, wwy, aag, qqt) \
	GLM_SWIZZLE_PROPERTY3(3, 3, 2, wwz, aab, qqp) \
	GLM_SWIZZLE_PROPERTY3(3, 3, 3, www, aaa, qqq) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 0, xxxx, rrrr, ssss) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 1, xxxy, rrrg, ssst) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 2, xxxz, rrrb, sssp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 0, 3, xxxw, rrra, sssq) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 0, xxyx, rrgr, ssts) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 1, xxyy, rrgg, sstt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 2, xxyz, rrgb, sstp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 1, 3, xxyw, rrga, sstq) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 0, xxzx, rrbr, ssps) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 1, xxzy, rrbg, sspt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 2, xxzz, rrbb, sspp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 2, 3, xxzw, rrba, sspq) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 3, 0, xxwx, rrar, ssqs) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 3, 1, xxwy, rrag, ssqt) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 3, 2, xxwz, rrab, ssqp) \
	GLM_SWIZZLE_PROPERTY4(0, 0, 3, 3, xxww, rraa, ssqq) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 0, xyxx, rgrr, stss) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 1, xyxy, rgrg, stst) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 2, xyxz, rgrb, stsp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 0, 3, xyxw, rgra, stsq) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 0, xyyx, rggr, stts) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 1, xyyy, rggg, sttt) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 2, xyyz, rggb, sttp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 1, 3, xyyw, rgga, sttq) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 0, xyzx, rgbr, stps) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 1, xyzy, rgbg, stpt) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 2, 2, xyzz, rgbb, stpp) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 1, 2, 3, xyzw, rgba, stpq) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 3, 0, xywx, rgar, stqs) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 3, 1, xywy, rgag, stqt) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 1, 3, 2, xywz, rgab, stqp) \
	GLM_SWIZZLE_PROPERTY4(0, 1, 3, 3, xyww, rgaa, stqq) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 0, xzxx, rbrr, spss) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 1, xzxy, rbrg, spst) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 2, xzxz, rbrb, spsp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 0, 3, xzxw, rbra, spsq) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 0, xzyx, rbgr, spts) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 1, xzyy, rbgg, sptt) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 1, 2, xzyz, rbgb, sptp) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 2, 1, 3, xzyw, rbga, sptq) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 0, xzzx, rbbr, spps) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 1, xzzy, rbbg, sppt) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 2, xzzz, rbbb, sppp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 2, 3, xzzw, rbba, sppq) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 3, 0, xzwx, rbar, spqs) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 2, 3, 1, xzwy, rbag, spqt) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 3, 2, xzwz, rbab, spqp) \
	GLM_SWIZZLE_PROPERTY4(0, 2, 3, 3, xzww, rbaa, spqq) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 0, 0, xwxx, rarr, sqss) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 0, 1, xwxy, rarg, sqst) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 0, 2, xwxz, rarb, sqsp) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 0, 3, xwxw, rara, sqsq) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 1, 0, xwyx, ragr, sqts) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 1, 1, xwyy, ragg, sqtt) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 3, 1, 2, xwyz, ragb, sqtp) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 1, 3, xwyw, raga, sqtq) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 2, 0, xwzx, rabr, sqps) \
	GLM_SWIZZLE_PROPERTY4_RW(0, 3, 2, 1, xwzy, rabg, sqpt) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 2, 2, xwzz, rabb, sqpp) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 2, 3, xwzw, raba, sqpq) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 3, 0, xwwx, raar, sqqs) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 3, 1, xwwy, raag, sqqt) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 3, 2, xwwz, raab, sqqp) \
	GLM_SWIZZLE_PROPERTY4(0, 3, 3, 3, xwww, raaa, sqqq) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 0, yxxx, grrr, tsss) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 1, yxxy, grrg, tsst) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 2, yxxz, grrb, tssp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 0, 3, yxxw, grra, tssq) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 0, yxyx, grgr, tsts) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 1, yxyy, grgg, tstt) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 2, yxyz, grgb, tstp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 1, 3, yxyw, grga, tstq) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 0, yxzx, grbr, tsps) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 1, yxzy, grbg, tspt) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 2, 2, yxzz, grbb, tspp) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 0, 2, 3, yxzw, grba, tspq) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 3, 0, yxwx, grar, tsqs) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 3, 1, yxwy, grag, tsqt) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 0, 3, 2, yxwz, grab, tsqp) \
	GLM_SWIZZLE_PROPERTY4(1, 0, 3, 3, yxww, graa, tsqq) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 0, yyxx, ggrr, ttss) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 1, yyxy, ggrg, ttst) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 2, yyxz, ggrb, ttsp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 0, 3, yyxw, ggra, ttsq) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 0, yyyx, gggr, ttts) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 1, yyyy, gggg, tttt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 2, yyyz, gggb, tttp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 1, 3, yyyw, ggga, tttq) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 0, yyzx, ggbr, ttps) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 1, yyzy, ggbg, ttpt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 2, yyzz, ggbb, ttpp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 2, 3, yyzw, ggba, ttpq) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 3, 0, yywx, ggar, ttqs) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 3, 1, yywy, ggag, ttqt) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 3, 2, yywz, ggab, ttqp) \
	GLM_SWIZZLE_PROPERTY4(1, 1, 3, 3, yyww, ggaa, ttqq) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 0, yzxx, gbrr, tpss) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 1, yzxy, gbrg, tpst) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 0, 2, yzxz, gbrb, tpsp) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 2, 0, 3, yzxw, gbra, tpsq) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 0, yzyx, gbgr, tpts) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 1, yzyy, gbgg, tptt) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 2, yzyz, gbgb, tptp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 1, 3, yzyw, gbga, tptq) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 0, yzzx, gbbr, tpps) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 1, yzzy, gbbg, tppt) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 2, yzzz, gbbb, tppp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 2, 3, yzzw, gbba, tppq) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 2, 3, 0, yzwx, gbar, tpqs) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 3, 1, yzwy, gbag, tpqt) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 3, 2, yzwz, gbab, tpqp) \
	GLM_SWIZZLE_PROPERTY4(1, 2, 3, 3, yzww, gbaa, tpqq) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 0, 0, ywxx, garr, tqss) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 0, 1, ywxy, garg, tqst) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 3, 0, 2, ywxz, garb, tqsp) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 0, 3, ywxw, gara, tqsq) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 1, 0, ywyx, gagr, tqts) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 1, 1, ywyy, gagg, tqtt) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 1, 2, ywyz, gagb, tqtp) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 1, 3, ywyw, gaga, tqtq) \
	GLM_SWIZZLE_PROPERTY4_RW(1, 3, 2, 0, ywzx, gabr, tqps) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 2, 1, ywzy, gabg, tqpt) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 2, 2, ywzz, gabb, tqpp) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 2, 3, ywzw, gaba, tqpq) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 3, 0, ywwx, gaar, tqqs) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 3, 1, ywwy, gaag, tqqt) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 3, 2, ywwz, gaab, tqqp) \
	GLM_SWIZZLE_PROPERTY4(1, 3, 3, 3, ywww, gaaa, tqqq) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 0, zxxx, brrr, psss) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 1, zxxy, brrg, psst) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 2, zxxz, brrb, pssp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 0, 3, zxxw, brra, pssq) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 0, zxyx, brgr, psts) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 1, zxyy, brgg, pstt) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 1, 2, zxyz, brgb, pstp) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 0, 1, 3, zxyw, brga, pstq) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 0, zxzx, brbr, psps) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 1, zxzy, brbg, pspt) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 2, zxzz, brbb, pspp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 2, 3, zxzw, brba, pspq) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 3, 0, zxwx, brar, psqs) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 0, 3, 1, zxwy, brag, psqt) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 3, 2, zxwz, brab, psqp) \
	GLM_SWIZZLE_PROPERTY4(2, 0, 3, 3, zxww, braa, psqq) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 0, zyxx, bgrr, ptss) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 1, zyxy, bgrg, ptst) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 0, 2, zyxz, bgrb, ptsp) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 1, 0, 3, zyxw, bgra, ptsq) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 0, zyyx, bggr, ptts) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 1, zyyy, bggg, pttt) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 2, zyyz, bggb, pttp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 1, 3, zyyw, bgga, pttq) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 0, zyzx, bgbr, ptps) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 1, zyzy, bgbg, ptpt) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 2, zyzz, bgbb, ptpp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 2, 3, zyzw, bgba, ptpq) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 1, 3, 0, zywx, bgar, ptqs) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 3, 1, zywy, bgag, ptqt) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 3, 2, zywz, bgab, ptqp) \
	GLM_SWIZZLE_PROPERTY4(2, 1, 3, 3, zyww, bgaa, ptqq) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 0, zzxx, bbrr, ppss) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 1, zzxy, bbrg, ppst) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 2, zzxz, bbrb, ppsp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 0, 3, zzxw, bbra, ppsq) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 0, zzyx, bbgr, ppts) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 1, zzyy, bbgg, pptt) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 2, zzyz, bbgb, pptp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 1, 3, zzyw, bbga, pptq) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 0, zzzx, bbbr, ppps) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 1, zzzy, bbbg, pppt) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 2, zzzz, bbbb, pppp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 2, 3, zzzw, bbba, pppq) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 3, 0, zzwx, bbar, ppqs) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 3, 1, zzwy, bbag, ppqt) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 3, 2, zzwz, bbab, ppqp) \
	GLM_SWIZZLE_PROPERTY4(2, 2, 3, 3, zzww, bbaa, ppqq) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 0, 0, zwxx, barr, pqss) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 3, 0, 1, zwxy, barg, pqst) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 0, 2, zwxz, barb, pqsp) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 0, 3, zwxw, bara, pqsq) \
	GLM_SWIZZLE_PROPERTY4_RW(2, 3, 1, 0, zwyx, bagr, pqts) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 1, 1, zwyy, bagg, pqtt) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 1, 2, zwyz, bagb, pqtp) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 1, 3, zwyw, baga, pqtq) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 2, 0, zwzx, babr, pqps) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 2, 1, zwzy, babg, pqpt) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 2, 2, zwzz, babb, pqpp) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 2, 3, zwzw, baba, pqpq) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 3, 0, zwwx, baar, pqqs) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 3, 1, zwwy, baag, pqqt) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 3, 2, zwwz, baab, pqqp) \
	GLM_SWIZZLE_PROPERTY4(2, 3, 3, 3, zwww, baaa, pqqq) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 0, 0, wxxx, arrr, qsss) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 0, 1, wxxy, arrg, qsst) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 0, 2, wxxz, arrb, qssp) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 0, 3, wxxw, arra, qssq) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 1, 0, wxyx, argr, qsts) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 1, 1, wxyy, argg, qstt) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 0, 1, 2, wxyz, argb, qstp) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 1, 3, wxyw, arga, qstq) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 2, 0, wxzx, arbr, qsps) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 0, 2, 1, wxzy, arbg, qspt) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 2, 2, wxzz, arbb, qspp) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 2, 3, wxzw, arba, qspq) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 3, 0, wxwx, arar, qsqs) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 3, 1, wxwy, arag, qsqt) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 3, 2, wxwz, arab, qsqp) \
	GLM_SWIZZLE_PROPERTY4(3, 0, 3, 3, wxww, araa, qsqq) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 0, 0, wyxx, agrr, qtss) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 0, 1, wyxy, agrg, qtst) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 1, 0, 2, wyxz, agrb, qtsp) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 0, 3, wyxw, agra, qtsq) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 1, 0, wyyx, aggr, qtts) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 1, 1, wyyy, aggg, qttt) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 1, 2, wyyz, aggb, qttp) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 1, 3, wyyw, agga, qttq) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 1, 2, 0, wyzx, agbr, qtps) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 2, 1, wyzy, agbg, qtpt) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 2, 2, wyzz, agbb, qtpp) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 2, 3, wyzw, agba, qtpq) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 3, 0, wywx, agar, qtqs) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 3, 1, wywy, agag, qtqt) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 3, 2, wywz, agab, qtqp) \
	GLM_SWIZZLE_PROPERTY4(3, 1, 3, 3, wyww, agaa, qtqq) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 0, 0, wzxx, abrr, qpss) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 2, 0, 1, wzxy, abrg, qpst) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 0, 2, wzxz, abrb, qpsp) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 0, 3, wzxw, abra, qpsq) \
	GLM_SWIZZLE_PROPERTY4_RW(3, 2, 1, 0, wzyx, abgr, qpts) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 1, 1, wzyy, abgg, qptt) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 1, 2, wzyz, abgb, qptp) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 1, 3, wzyw, abga, qptq) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 2, 0, wzzx, abbr, qpps) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 2, 1, wzzy, abbg, qppt) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 2, 2, wzzz, abbb, qppp) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 2, 3, wzzw, abba, qppq) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 3, 0, wzwx, abar, qpqs) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 3, 1, wzwy, abag, qpqt) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 3, 2, wzwz, abab, qpqp) \
	GLM_SWIZZLE_PROPERTY4(3, 2, 3, 3, wzww, abaa, qpqq) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 0, 0, wwxx, aarr, qqss) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 0, 1, wwxy, aarg, qqst) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 0, 2, wwxz, aarb, qqsp) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 0, 3, wwxw, aara, qqsq) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 1, 0, wwyx, aagr, qqts) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 1, 1, wwyy, aagg, qqtt) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 1, 2, wwyz, aagb, qqtp) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 1, 3, wwyw, aaga, qqtq) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 2, 0, wwzx, aabr, qqps) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 2, 1, wwzy, aabg, qqpt) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 2, 2, wwzz, aabb, qqpp) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 2, 3, wwzw, aaba, qqpq) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 3, 0, wwwx, aaar, qqqs) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 3, 1, wwwy, aaag, qqqt) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 3, 2, wwwz, aaab, qqqp) \
	GLM_SWIZZLE_PROPERTY4(3, 3, 3, 3, wwww, aaaa, qqqq)
