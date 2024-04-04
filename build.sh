#!/bin/sh

# -DDEV_BUILD -DDEBUG_BUILD -DSLOW_BUILD
ccflags="-Wall -Wextra -pedantic -std=c99 -I."
ccflags="${ccflags} -Wduplicated-cond -Wduplicated-branches -Wlogical-op"
ccflags="${ccflags} -Wuseless-cast -Wjump-misses-init -Wdouble-promotion"
ccflags="${ccflags} -Wrestrict -Wnull-dereference -Wshadow -Wformat=2"
ccflags="${ccflags} -fsanitize=address,undefined,fuzzer"
lflags="-lm -ldl -lpthread"
set -xe

echo - Testing with Clang
clang ${ccflags} base/*.c test.c -oa-clang ${lflags}

echo - Testing with GCC
gcc ${ccflags} base/*.c test.c -oa-gcc ${lflags}
