#ifndef CORE_BASE_H
#define CORE_BASE_H

///
// NOTE: Context cracking macros
///
#if !defined(ENABLE_ASSERT)
	#define ENABLE_ASSERT 0
#endif

// NOTE: Compiler detection
#if defined(__clang__) || defined(__llvm__)
	#define COMPILER_CLANG 1
#elif defined(_MSC_VER)
	#define COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define COMPILER_GCC 1
#else
	#error Missing compiler detection
#endif

// NOTE: OS detection
#if defined(_WIN32)
	#define OS_WINDOWS 1
#elif defined(__gnu_linux__)
	#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
	#define OS_MAC 1
#elif defined(__FreeBSD__)
	#define OS_FREEBSD 1
#else
	#error Missing OS detection
#endif

// NOTE: CPU architecture detection
#if COMPILER_MSVC
	#if defined(_M_AMD64)
		#define CPU_ARCH_X64 1
	#elif defined(_M_IX86)
		#define CPU_ARCH_X86 1
	#elif defined(_M_ARM64)
		#define CPU_ARCH_ARM64 1
	#endif
#elif defined(__amd64__) || defined(__x86_64__)
	#define CPU_ARCH_X64 1
#elif defined(__i386__)
	#define CPU_ARCH_X86 1
#elif defined(__aarch64__)
	#define CPU_ARCH_ARM64 1
#else
	#error Missing CPU arch detection
#endif

// NOTE: Language detection
#if defined(__cplusplus)
	#define LANG_CPP 1
	#define LANG_C 0
#else
	#define LANG_CPP 0
	#define LANG_C 1
#endif

#if !defined(COMPILER_CLANG)
	#define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_MSVC)
	#define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
	#define COMPILER_GCC 0
#endif

#if !defined(OS_WINDOWS)
	#define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
	#define OS_LINUX 0
#endif
#if !defined(OS_MAC)
	#define OS_MAC 0
#endif
#if !defined(OS_FREEBSD)
	#define OS_FREEBSD 0
#endif

#if !defined(CPU_ARCH_X64)
	#define CPU_ARCH_X64 0
#endif
#if !defined(CPU_ARCH_X86)
	#define CPU_ARCH_X86 0
#endif
#if !defined(CPU_ARCH_ARM64)
	#define CPU_ARCH_ARM64 0
#endif

#if OS_MAC || OS_LINUX || OS_FREEBSD
	#define OS_UNIX_LIKE 1
#else
	#define OS_UNIX_LIKE 0
#endif

///
// NOTE: Intrinsics based on platforms
///
// TODO: CPU feature detection (cpuid on x86?)
#if OS_WINDOWS
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#ifndef _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE
	#endif
#endif

#if OS_WINDOWS && (COMPILER_MSVC || COMPILER_CLANG)
	#include <intrin.h>
	#define MICROSOFT_INTRINSICS 1
#endif

#if !OS_WINDOWS && (COMPILER_GCC || COMPILER_CLANG)
	#define GNU_INTRINSICS 1
#endif

#if !defined(MICROSOFT_INTRINSICS)
	#define MICROSOFT_INTRINSICS 0
#endif

#if !defined(GNU_INTRINSICS)
	#define GNU_INTRINSICS 0
#endif

#if COMPILER_CLANG || COMPILER_GCC
#define _printf_fmt _Printf_format_string_
#else
#define _printf_fmt
#endif

#if COMPILER_CLANG || COMPILER_GCC
#define _PRINTF_STYLE_FMT(sig) sig __attribute__((format(printf, 2, 3)))
#else
#define _PRINTF_STYLE_FMT(sig)
#endif

#if MICROSOFT_INTRINSICS
	#define debug_break() __debugbreak()
	#define rdtsc() __rdtsc()
	#if CPU_ARCH_X64
		#define cpuid(fn, a, b, c, d) do {	\
				int cpu_info[4];            \
				__cpuidex(cpu_info, fn, 0); \
				a = cpu_info[0];            \
				b = cpu_info[1];            \
				c = cpu_info[2];            \
				d = cpu_info[3];            \
			} while(0)
	#endif
#elif GNU_INTRINSICS
	#if __has_builtin(__builtin_debugtrap)
		#define debug_break() __builtin_debugtrap()
	#else
		#include <signal.h>
		#define debug_break() raise(SIGTRAP)
	#endif
	#define rdstc() _rdtsc()
	#define cpuid(fn, a, b, c, d)
#endif

#if !defined(debug_break)
	#define debug_break()
#endif
#if !defined(rdtsc)
	#define rdtsc()
#endif
#if !defined(cpuid)
	#define cpuid(fn, a, b, c, d)
#endif

///
// NOTE: Helper/keyword macros
///
#if !defined(assert_break)
	#define assert_break() (*((volatile int *)0) = 0)
#endif

#if ENABLE_ASSERT
	#define assert(expr) do { if (!(expr)) { assert_break(); } } while (0)
#else
	#define assert(expr)
#endif

#define CONCAT__(a, b) a##b
#define CONCAT(a, b) CONCAT__(a, b)
#define STR__(x) #x
#define STR(x) STR__(x)

#define bit(x) (1 << (x))

#define array_count(a) (sizeof((a)) / sizeof(*(a)))
#define int_from_ptr(p) (size_t)(p)
#define ptr_from_int(i) (void *)(i)
#define struct_member(T, m) (((T*)0)->m)
#define size_of_member(T, m) sizeof(struct_member(T, m))
#define offset_of_member(T, m) (size_t)(&struct_member(T, m))

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp(x, a, b) (((x) < (a)) ? (a) : ((x) > (b)) ? (b) : (x))

#define is_power_of_two(x) ((x > 0) && !(x & (x - 1)))
#define align_up_power_of_two(n, p) (((n) + (p) - 1) & ~((p) - 1))

#define kilobytes(n) (1024ull * n)
#define megabytes(n) kilobytes(1024ull * n)
#define gigabytes(n) megabytes(1024ull * n)
#define terabytes(n) gigabytes(1024ull * n)

#define unused_param(a) ((void)a)
#define unused_var(a) ((void)a)

#define global static
#define internal static
#define persistent static

#if LANG_CPP
	#define C_API extern "C"
	#define C_API_BEGIN extern "C" {
	#define C_API_END }
#else
	#define C_API
	#define C_API_BEGIN
	#define C_API_END
#endif

#if LANG_CPP
	#define external C_API
#else
	#define external extern
#endif

#if COMPILER_MSVC
	#define dllexport external __declspec(dllexport)
	#define dllimport external __declspec(dllimport)
#elif COMPILER_GCC || COMPILER_CLANG
	#define dllexport external __attribute__((visibility("default")))
	#define dllimport external
#else
	#define dllexport external
	#define dllimport external
#endif

///
// NOTE: Symbolic constants
///
typedef enum {
	OS_Enum_NULL,
	OS_Enum_Windows,
	OS_Enum_Linux,
	OS_Enum_Mac,
	OS_Enum_FreeBSD,
	OS_Enum_COUNT,
} OS_Enum;

typedef enum {
	CPU_Arch_Enum_NULL,
	CPU_Arch_Enum_x86,
	CPU_Arch_Enum_x64,
	CPU_Arch_Enum_Arm64,
	CPU_Arch_Enum_COUNT,
} CPU_Arch_Enum;

C_API const char *string_from_os_enum(OS_Enum os);
C_API const char *string_from_cpu_arch_enum(CPU_Arch_Enum arch);
C_API OS_Enum get_os_from_context(void);
C_API CPU_Arch_Enum get_cpu_arch_context(void);

// NOTE: (The only) Required standard C headers
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

// NOTE: Base types
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;
typedef float f32;
typedef double f64;
typedef void void_func(void);

///
// NOTE: Integer macros/helpers
///
#define U8_MAX  0xFFui8
#define U16_MAX 0xFFFFui16
#define U32_MAX 0xFFFFFFFFui32
#define U64_MAX 0xFFFFFFFFFFFFFFFFui64
#define S8_MIN  INT8_MIN
#define S16_MIN INT16_MIN
#define S32_MIN INT32_MIN
#define S64_MIN INT64_MIN
#define S8_MAX  INT8_MAX
#define S16_MAX INT16_MAX
#define S32_MAX INT32_MAX
#define S64_MAX INT64_MAX

#if CPU_ARCH_X64 || CPU_ARCH_ARM64
	#define SIZET_MAX U64_MAX
#elif CPU_ARCH_X86
	#define SIZET_MAX U32_MAX
#endif

C_API u32 safe_truncate_u64(u64 value);
C_API size_t u64_to_size_t(u64 value);
C_API size_t u32_to_size_t(u32 value);

///
// NOTE: Floating point helpers/macros
///
#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f
#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308
#define EPSILON32 1.19209289551e-7f
#define EPSILON64 2.22044604925e-16
#define PI32 3.141592653589f
#define PI64 3.14159265358979323846
#define TAU32 6.28318530718f
#define TAU64 6.2831853071795864769

C_API f32 infinity_f32(void);
C_API f32 neg_infinity_f32(void);
C_API f64 infinity_f64(void);
C_API f64 neg_infinity_f64(void);
C_API b32 is_infinity_or_nan_f32(f32 value);
C_API b64 is_infinity_or_nan_f64(f64 value);

///
// NOTE: Basic math functions
///
C_API f32 truncate_f32(f32 f);
C_API f64 truncate_f64(f64 f);
C_API f32 floor_f32(f32 f);
C_API f64 floor_f64(f64 f);
C_API f32 ceil_f32(f32 f);
C_API f64 ceil_f64(f64 f);
C_API f32 round_f32(f32 f);
C_API f64 round_f64(f64 f);
C_API f32 abs_f32(f32 f);
C_API f64 abs_f64(f64 f);
C_API f32 sign_f32(f32 x);
C_API f64 sign_f64(f64 x);
C_API f32 lerp_f32(f32 a, f32 b, f32 t);
C_API f64 lerp_f64(f64 a, f64 b, f64 t);
C_API f32 unlerp_f32(f32 a, f32 b, f32 x);
C_API f64 unlerp_f64(f64 a, f64 b, f64 x);
C_API f32 sqrt_f32(f32 f);
C_API f64 sqrt_f64(f64 f);
C_API f32 sin_f32(f32 f);
C_API f64 sin_f64(f64 f);
C_API f32 cos_f32(f32 f);
C_API f64 cos_f64(f64 f);
C_API f32 tan_f32(f32 f);
C_API f64 tan_f64(f64 f);

///
// NOTE: Doublely Linked Lists
///
#define dll_push_back_np(f,l,n,next,prev) ((f)==0? \
                                           ((f)=(l)=(n),(n)->next=(n)->prev=0): \
                                           ((n)->prev=(l),(l)->next=(n),(l)=(n),(n)->next=0))
#define dll_push_back(f,l,n) dll_push_back_np(f,l,n,next,prev)
#define dll_push_front(f,l,n) dll_push_back_np(l,f,n,prev,next)
#define dll_remove_np(f,l,n,next,prev) ((f)==(n)? \
                                        ((f)==(l)? \
                                         ((f)=(l)=(0)): \
                                         ((f)=(f)->next,(f)->prev=0)): \
                                        (l)==(n)? \
                                        ((l)=(l)->prev,(l)->next=0): \
                                        ((n)->next->prev=(n)->prev, \
                                         (n)->prev->next=(n)->next))
#define dll_remove(f,l,n) dll_remove_np(f,l,n,next,prev)

///
// NOTE: Singlely Linked Lists
///
#define sll_queue_push_N(f,l,n,next) (((f)==0? \
                                       (f)=(l)=(n): \
                                       ((l)->next=(n),(l)=(n))), \
                                      (n)->next=0)
#define sll_queue_push(f,l,n) sll_queue_push_N(f,l,n,
#define sll_queue_pushfront_N(f,l,n,next) ((f) == 0 ? \
                                           ((f) = (l) = (n), (n)->next = 0) : \
                                           ((n)->next = (f), (f) = (n)))
#define sll_queue_pushfront(f,l,n) sll_queue_pushfront_N(f,l,n,next)
#define sll_queue_pop_N(f,l,next) ((f) == (l)? \
                                   (f) = (l) = 0 : \
                                   ((f) = (f)->next))
#define sll_queue_pop(f,l) sll_queue_pop_N(f,l,next)

#define sll_stack_push_N(f,n,next) ((n)->next = (f), (f) = (n))
#define sll_stack_push(f,n) sll_stack_push_N(f,n,next)
#define sll_stack_pop_N(f,next) ((f) == 0 ? 0 : \
                                 ((f) = (f)->next))
#define sll_stack_pop(f) sll_stack_pop_N(f,next)

/// TODO
// NOTE: Binary Trees
///
/// TODO
// NOTE: Dynamic arrays
///
/// TODO
// NOTE: Hash tables
///

///
// NOTE: Memory arena allocator
///
typedef void *reserve_func(void *ctx, u64 size);
typedef void change_mem_func(void *ctx, void *ptr, u64 size);

typedef change_mem_func commit_func;
typedef change_mem_func decommit_func;
typedef change_mem_func release_func;

typedef struct {
	void *context;
	reserve_func *reserve;
	commit_func *commit;
	decommit_func *decommit;
	release_func *release;
} Memory_Allocator;

typedef struct {
	Memory_Allocator *allocator;
	u8 *memory;
	u64 capacity;
	u64 position;
	u64 commit_position;
} Memory_Arena;

#define ARENA_DEFAULT_RESERVE_SIZE gigabytes(1)
#define ARENA_COMMIT_BLOCK_SIZE megabytes(64)

C_API Memory_Arena make_arena_reserve(Memory_Allocator *allocator, u64 reserve_size);
C_API Memory_Arena make_arena(Memory_Allocator *allocator);
C_API void memory_arena_release(Memory_Arena *arena);
C_API void *memory_arena_push(Memory_Arena *arena, u64 size);
C_API void memory_arena_pop_to(Memory_Arena *arena, u64 position);

typedef struct {
	Memory_Arena *arena;
	u64 arena_position;
} Temp_Arena;

Temp_Arena memory_arena_begin_temp(Memory_Arena *arena);
void memory_arena_end_temp(Temp_Arena temp);

/// TODO
// NOTE: Strings
///

#endif // CORE_BASE_H
