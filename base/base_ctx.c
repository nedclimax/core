#include "base.h"

const char *string_from_os_enum(OS_Enum os) {
	switch (os) {
		case OS_Enum_NULL:    return "NULL";
		case OS_Enum_Windows: return "Windows";
		case OS_Enum_Linux:   return "Linux";
		case OS_Enum_Mac:     return "Mac OS";
		case OS_Enum_FreeBSD: return "Free BSD";
		default: return "NULL";
	}
}

const char *string_from_cpu_arch_enum(CPU_Arch_Enum arch) {
	switch (arch) {
		case CPU_Arch_Enum_NULL:  return "NULL";
		case CPU_Arch_Enum_x86:   return "x86";
		case CPU_Arch_Enum_x64:   return "x64";
		case CPU_Arch_Enum_Arm64: return "arm64";
		default: return "NULL";
	}
}

OS_Enum get_os_from_context(void) {
	OS_Enum result = OS_Enum_NULL;
#if OS_WINDOWS
	result = OS_Enum_Windows;
#elif OS_LINUX
	result = OS_Enum_Linux;
#elif OS_MAC
	result = OS_Enum_Mac;
#elif OS_FREEBSD
	result = OS_Enum_FreeBSD;
#endif
	return result;
}

CPU_Arch_Enum get_cpu_arch_context(void) {
	CPU_Arch_Enum result = CPU_Arch_Enum_NULL;
#if CPU_ARCH_X64
	result = CPU_Arch_Enum_x64;
#elif CPU_ARCH_X86
	result = CPU_Arch_Enum_x86;
#elif CPU_ARCH_ARM64
	result = CPU_Arch_Enum_Arm64;
#endif
	return result;
}
