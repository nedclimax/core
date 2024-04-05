#include "base/base.h"
#include <stdio.h>
#include <stdlib.h>

#define PRINT_MACRO_RESULT(macro_def) printf("%s = %d\n", #macro_def, macro_def)
#define PRINT_MACRO_RESULT_SZ(macro_def) printf("%s = %zu\n", #macro_def, macro_def)
#define PRINT_FLOAT(f) printf("%s = %f [%e]\n", #f, (f), (f))

typedef struct {
	int a;
	int b;
	int c;
	int d;
} Test_Struct;

typedef struct Node Node;
struct Node {
	Node *next;
	Node *prev;
	int data;
};

int main(void) {
	Node nodes[10];
	for (size_t i = 0; i < array_count(nodes); i++) {
		nodes[i].data = (int)i;
	}

	Node *first = NULL;
	Node *last = NULL;
	for (size_t i = 0; i < 5; i++) {
		dll_push_back(first, last, &nodes[i]);
	}

	for (size_t i = 5; i < 10; i++) {
		dll_push_front(first, last, &nodes[i]);
	}

	for (Node *n = first; n != NULL; n = n->next) {
		printf("- %d\n", n->data);
	}

#if 0
	puts("- CPU architecture");
	PRINT_MACRO_RESULT(CPU_ARCH_X64);
	PRINT_MACRO_RESULT(CPU_ARCH_X86);
	PRINT_MACRO_RESULT(CPU_ARCH_ARM64);

	puts("\n- Compiler");
	PRINT_MACRO_RESULT(COMPILER_MSVC);
	PRINT_MACRO_RESULT(COMPILER_CLANG);
	PRINT_MACRO_RESULT(COMPILER_GCC);

	puts("\n- Operating system");
	PRINT_MACRO_RESULT(OS_WINDOWS);
	PRINT_MACRO_RESULT(OS_LINUX);
	PRINT_MACRO_RESULT(OS_MAC);
	PRINT_MACRO_RESULT(OS_FREEBSD);
	PRINT_MACRO_RESULT(OS_UNIX_LIKE);

	puts("\n- Intrinsics");
	PRINT_MACRO_RESULT(MICROSOFT_INTRINSICS);
	PRINT_MACRO_RESULT(GNU_INTRINSICS);

	puts("\n- Integer functions");
	PRINT_MACRO_RESULT(is_power_of_two(1));
	PRINT_MACRO_RESULT(is_power_of_two(2));
	PRINT_MACRO_RESULT(is_power_of_two(64));
	PRINT_MACRO_RESULT(is_power_of_two(256));
	PRINT_MACRO_RESULT(is_power_of_two(1024));
	PRINT_MACRO_RESULT(is_power_of_two(5));
	PRINT_MACRO_RESULT(is_power_of_two(3));
	PRINT_MACRO_RESULT(is_power_of_two(0));
	PRINT_MACRO_RESULT(is_power_of_two(-1));

	puts("\n- Floats");
	printf("machine_epsilon_f32 = %f [%e]\n", EPSILON32, EPSILON32);
	printf("machine_epsilon_f64 = %f [%e]\n", EPSILON64, EPSILON64);
	PRINT_FLOAT(infinity_f32());
	PRINT_FLOAT(infinity_f64());
	PRINT_FLOAT(neg_infinity_f32());
	PRINT_FLOAT(neg_infinity_f64());
	PRINT_FLOAT(F32_MIN);
	PRINT_FLOAT(F32_MAX);
	PRINT_FLOAT(F64_MIN);
	PRINT_FLOAT(F64_MAX);

	puts("\n- Float functions");
	f32 p = 5.1f;
	f64 pd = -5.5;
	PRINT_FLOAT(truncate_f32(p));
	PRINT_FLOAT(truncate_f64(pd));
	PRINT_FLOAT(floor_f32(p));
	PRINT_FLOAT(floor_f64(pd));
	PRINT_FLOAT(ceil_f32(p));
	PRINT_FLOAT(ceil_f64(pd));
	PRINT_FLOAT(round_f32(p));
	PRINT_FLOAT(round_f64(pd));

	puts("\n- Offset macros");
	PRINT_MACRO_RESULT_SZ(offset_of_member(Test_Struct, a));
	PRINT_MACRO_RESULT_SZ(offset_of_member(Test_Struct, b));
	PRINT_MACRO_RESULT_SZ(offset_of_member(Test_Struct, c));
	PRINT_MACRO_RESULT_SZ(offset_of_member(Test_Struct, d));
	PRINT_MACRO_RESULT_SZ(size_of_member(Test_Struct, d));
	PRINT_MACRO_RESULT_SZ(size_of_member(Test_Struct, a));

	puts("\n- Array count macro");
	int test_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	PRINT_MACRO_RESULT_SZ(array_count(test_array));
#endif

#if CPU_ARCH_X64 || CPU_ARCH_X86
	puts("\n- x86_64 CPU Features");

	// NOTE: Detecting cpu features at runtime
	int cpu_info[4] = {0};
	int cpuid_features[4] = {0};
	__cpuidex(cpu_info, 0, 0);

	if (cpu_info[1] >= 1) {
		__cpuidex(cpu_info, 1, 0);
		cpuid_features[0] = cpu_info[0];
		cpuid_features[1] = cpu_info[1];
		cpuid_features[2] = cpu_info[2];
		cpuid_features[3] = cpu_info[3];
	}

	b32 have_sse = cpuid_features[3] & 0x02000000;
	puts(have_sse ? "We have SSE" : "We don't have SSE");

	b32 have_sse2 = cpuid_features[3] & 0x04000000;
	puts(have_sse2 ? "We have SSE2" : "We don't have SSE2");

	b32 have_sse3 = cpuid_features[2] & 0x00000001;
	puts(have_sse3 ? "We have SSE3" : "We don't have SSE3");

	b32 have_sse4_1 = cpuid_features[2] & 0x00080000;
	puts(have_sse4_1 ? "We have SSE4.1" : "We don't have SSE4.1");

	b32 have_sse4_2 = cpuid_features[2] & 0x00100000;
	puts(have_sse4_2 ? "We have SSE4.2" : "We don't have SSE4.2");

	if (cpu_info[0] >= 7) {
		__cpuidex(cpu_info, 7, 0);
		puts((cpu_info[1] & 0x00000020) ? "We have AVX2" : "We don't have AVX2");
		puts((cpu_info[1] & 0x00010000) ? "We have AVX-512" : "We don't have AVX-512");
	}
#endif
	(void)getchar();
	return 0;
}
