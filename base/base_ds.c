#include "base.h"
#include <string.h>

/*************************************
 *                                   *
 * NOTE: Dynamic Array Implementaion *
 *                                   *
 *************************************/

void *array_init(size_t item_size, size_t capacity, Allocator *a) {
	void *ptr = 0;
	size_t size = item_size * capacity + sizeof(Array_Header);
	Array_Header *h = a->alloc(size, a->context);

	if (h) {
		h->capacity = capacity;
		h->length = 0;
		h->a = a;
		ptr = h + 1;
	}

	return ptr;
}

void *array_ensure_capacity(void *a, size_t item_count, size_t item_size) {
	Array_Header *h = array_header(a);
	size_t desired_capacity = h->length + item_count;

	if (h->capacity < desired_capacity) {
		size_t new_capacity = h->capacity * 2;
		while (new_capacity < desired_capacity) {
			new_capacity *= 2;
		}

		size_t new_size = sizeof(Array_Header) + new_capacity * item_size;
		Array_Header *new_h = h->a->alloc(new_size, h->a->context);

		if (new_h) {
			size_t old_size = sizeof(*h) + h->length * item_size;
			memcpy(new_h, h, old_size);

			if (h->a->free) {
				h->a->free(old_size, h, h->a->context);
			}

			new_h->capacity = new_capacity;
			h = new_h + 1;
		} else {
			h = 0;
		}
	} else { h += 1; }

	return h;
}

/**************************************
 *                                    *
 * NOTE: Dynamic String Implementaion *
 *                                    *
 **************************************/

StringU8 str_init(size_t len, Allocator *a) {
	StringU8 s = {0};
	s.len = len;
	s.str = a->alloc(len + 1, a->context);
	s.str[len] = 0;
	return s;
}

StringU8 str_concat(StringU8 s1, StringU8 s2, Allocator *a) {
	size_t len = s1.len + s2.len;
	StringU8 s = str_init(len, a);
	memcpy(s.str, s1.str, s1.len);
	memcpy(&s.str[s1.len], s2.str, s2.len);
	return s;
}

StringU8 str_substring(StringU8 s, size_t start, size_t end, Allocator *a) {
	StringU8 r = {0};
	if (end <= s.len && start < end) {
		r = str_init(end - start, a);
		memcpy(r.str, &s.str[start], r.len);
	}
	return r;
}

bool str_contains(StringU8 haystack, StringU8 needle) {
	bool found = false;
	for (size_t i = 0, j = 0; i < haystack.len && !found; i += 1) {
		while (haystack.str[i] == needle.str[j]) {
			j += 1;
			i += 1;
			if (j == needle.len) {
				found = true;
				break;
			}
		}
	}
	return found;
}

size_t str_index_of(StringU8 haystack, StringU8 needle) {
	for (size_t i = 0; i < haystack.len; i += 1) {
		size_t j = 0;
		size_t start = i;
		while (haystack.str[i] == needle.str[j]) {
			j += 1;
			i += 1;
			if (j == needle.len) {
				return start;
			}
		}
	}
	return (size_t)-1;
}

// NOTE: this does not terminate the string with a 0 as that would destroy the original string.
StringU8 str_substring_view(StringU8 haystack, StringU8 needle) {
	StringU8 r = {0};
	size_t start_index = str_index_of(haystack, needle);
	if (start_index < haystack.len) {
		r.str = &haystack.str[start_index];
		r.len = needle.len;
	}
	return r;
}

bool str_equal(StringU8 a, StringU8 b) {
	if (a.len != b.len) {
		return false;
	}
	return memcmp(a.str, b.str, a.len) == 0;
}

StringU8 str_replace(StringU8 s, StringU8 match, StringU8 replacement, Allocator *a) {
	StringU8 r = {0};
	unused_param(s);
	unused_param(match);
	unused_param(replacement);
	unused_param(a);
	// TODO
	return r;
}

StringU8 str_view(StringU8 s, size_t start, size_t end) {
	if (end < start || end - start > s.len) {
		return (StringU8){0};
	}
	return (StringU8){end - start, s.str + start};
}

StringU8 str_clone(StringU8 s, Allocator *a) {
	StringU8 r = {0};
	if (s.len) {
		r.str = a->alloc(s.len, a->context);
		r.len = s.len;
		memcpy(r.str, s.str, s.len);
	}
	return r;
}

StringU8 *str_split(StringU8 s, StringU8 delimiter, Allocator *a) {
	StringU8 *arr = array(StringU8, a);
	size_t start = 0;
	for (size_t i = 0; i < s.len; i++) {
		if (s.str[i] != delimiter.str[0]) {
			continue;
		}

		if (memcmp(&s.str[i], delimiter.str, delimiter.len) == 0) {
			// Clone the substring before the delimiter.
			size_t end = i;
			StringU8 cloned = str_substring(s, start, end, a);
			array_append(arr, cloned);
			start = end + delimiter.len;
		}
	}
	// Get the last segment.
	if (start + delimiter.len < s.len) {
		StringU8 cloned = str_substring(s, start, s.len, a);
		array_append(arr, cloned);
	}
	return arr;
}

StringU8 *str_split_view(StringU8 s, StringU8 delimiter, Allocator *a) {
	StringU8 *arr = 0;
	size_t start = 0;
	for (size_t i = 0; i < s.len; i += 1) {
		if (s.str[i] != delimiter.str[0]) {
			continue;
		}

		if (memcmp(&s.str[i], delimiter.str, delimiter.len) == 0) {
			if (!arr) {
				arr = array(StringU8, a);
			}

			size_t end = i;
			StringU8 view = str_view(s, start, end);
			array_append(arr, view);
			start = end + delimiter.len;
		}
	}
	if (start + delimiter.len < s.len) {
		StringU8 view = str_view(s, start, s.len);
		array_append(arr, view);
	}
	return arr;
}

StringU8 str_join(StringU8 *s, StringU8 join, Allocator *a) {
	Array_Header *h = array_header(s);

	size_t total_length = s->len * join.len;
	for (size_t i = 0; i < h->length; i += 1) {
		total_length += s[i].len;
	}

	u8 *mem = a->alloc(total_length + 1, a->context);
	size_t offset = 0;
	for (size_t i = 0; i < h->length; i += 1) {
		memcpy(&mem[offset], s[i].str, s[i].len);
		offset += s[i].len;

		if (i == h->length - 1) {
			break;
		}

		memcpy(&mem[offset], join.str, join.len);
		offset += join.len;
	}

	mem[total_length] = 0;

	StringU8 result = { total_length, mem };
	return result;
}


/************************************
 *                                  *
 * NOTE: Memory Arena Implementaion *
 *                                  *
 ************************************/

uintptr_t align_forward(uintptr_t ptr, size_t alignment) {
    uintptr_t p, a, modulo;
    if (!is_power_of_two(alignment)) {
        return 0;
    }

    p = ptr;
    a = (uintptr_t)alignment;
    modulo = p & (a - 1);

    if (modulo) {
        p += a - modulo;
    }

    return p;
}

void *arena_alloc_aligned(Arena *a, size_t size, size_t alignment) {
    uintptr_t curr_ptr = (uintptr_t)a->base + (uintptr_t)a->offset;
    uintptr_t offset = align_forward(curr_ptr, alignment);
    offset -= (uintptr_t)a->base;

    if (offset + size > a->size) {
        return 0;
    }

    a->committed += size;
    void *ptr = (uint8_t *)a->base + offset;
    a->offset = offset + size;

    return ptr;
}

void *arena_alloc(size_t size, void *context) {
    if (!size) {
        return 0;
    }
    return arena_alloc_aligned((Arena *)context, size, DEFAULT_ALIGNMENT);
}

// Does nothing.
void arena_free(size_t size, void *ptr, void *context) {
    (void)ptr; (void)size; (void)context;
}

void arena_free_all(void *context) {
    Arena *a = context;
    a->offset = 0;
    a->committed = 0;
}

Arena arena_init(void *buffer, size_t size) {
    return (Arena){.base = buffer, .size = size};
}
