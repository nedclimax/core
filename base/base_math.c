#include "base.h"

f32 infinity_f32(void) {
	union { f32 f; u32 u; } r;
	r.u = 0x7f800000;
	return r.f;
}

f32 neg_infinity_f32(void) {
	union { f32 f; u32 u; } r;
	r.u = 0xff800000;
	return r.f;
}

f64 infinity_f64(void) {
	union { f64 f; u64 u; } r;
	r.u = 0x7ff0000000000000;
	return r.f;
}

f64 neg_infinity_f64(void) {
	union { f64 f; u64 u; } r;
	r.u = 0xfff0000000000000;
	return r.f;
}

b32 is_infinity_or_nan_f32(f32 value) {
	union { f32 f; u32 u; } r;
	r.f = value;
	return (r.u & 0x7f800000) == 0x7f800000;
}

b64 is_infinity_or_nan_f64(f64 value) {
	union { f64 f; u64 u; } r;
	r.f = value;
	return (r.u & 0x7ff0000000000000) == 0x7ff0000000000000;
}

#include <math.h>
// TODO: ^^ Remove this

f32 truncate_f32(f32 f) {
	return (f32)((s32)f);
}

f64 truncate_f64(f64 f) {
	return (f64)((s64)f);
}

f32 floor_f32(f32 f) {
	f32 res = truncate_f32(f);
	if (res > f) {
		res -= 1.0f;
	}
	return res;
}

f64 floor_f64(f64 f) {
	f64 res = truncate_f64(f);
	if (res > f) {
		res -= 1.0;
	}
	return res;
}

f32 ceil_f32(f32 f) {
	f32 res = truncate_f32(f);
	if (res < f) {
		res += 1.0f;
	}
	return res;
}

f64 ceil_f64(f64 f) {
	f64 res = truncate_f64(f);
	if (res < f) {
		res += 1.0f;
	}
	return res;
}

f32 round_f32(f32 f) {
	return (f >= 0.0f) ? truncate_f32(f + 0.5f) : truncate_f32(f - 0.5f);
}

f64 round_f64(f64 f) {
	return (f >= 0.0) ? truncate_f64(f + 0.5) : truncate_f64(f - 0.5);
}

f32 abs_f32(f32 f) {
	union { f32 f; u32 u; } result;
	result.f = f;
	result.u &= 0x7fffffff;
	return result.f;
}

f64 abs_f64(f64 f) {
	union { f64 f; u64 u; } result;
	result.f = f;
	result.u &= 0x7fffffffffffffff;
	return result.f;
}

f32 sign_f32(f32 x) {
	union { f32 f; u32 u; } r;
	r.f = x;
	f32 result = (r.u & 0x80000000) ? -1.0f : 1.0f;
	return result;
}

f64 sign_f64(f64 x) {
	union { f64 f; u64 u; } r;
	r.f = x;
	f64 result = (r.u & 0x8000000000000000) ? -1.0 : 1.0;
	return result;
}

f32 lerp_f32(f32 a, f32 b, f32 t) {
	return a + (b - a) * t;
}

f64 lerp_f64(f64 a, f64 b, f64 t) {
	return a + (b - a) * t;
}

f32 unlerp_f32(f32 a, f32 b, f32 x) {
	f32 t = 0.0f;
	if (a != b) {
		t = (x - a) / (b - a);
	}
	return t;
}

f64 unlerp_f64(f64 a, f64 b, f64 x) {
	f64 t = 0.0f;
	if (a != b) {
		t = (x - a) / (b - a);
	}
	return t;
}

f32 sqrt_f32(f32 f) {
	return sqrtf(f);
}

f64 sqrt_f64(f64 f) {
	return sqrt(f);
}

f32 sin_f32(f32 f) {
	return sinf(f);
}

f64 sin_f64(f64 f) {
	return sin(f);
}

f32 cos_f32(f32 f) {
	return cosf(f);
}

f64 cos_f64(f64 f) {
	return cos(f);
}

f32 tan_f32(f32 f) {
	return tanf(f);
}

f64 tan_f64(f64 f) {
	return tan(f);
}
