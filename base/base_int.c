#include "base.h"

u32 safe_truncate_u64(u64 value) {
	return value >= U32_MAX ? U32_MAX : (u32)value;
}

size_t u64_to_size_t(u64 value) {
	return value >= SIZE_MAX ? SIZE_MAX : (size_t)value;
}

size_t u32_to_size_t(u32 value) {
	return sizeof(u32) < sizeof(size_t) ? value : (size_t)value;
}
