@echo off
setlocal

del *.pdb

rem -DDEV_BUILD -DDEBUG_BUILD -DSLOW_BUILD

set ccflags=/nologo /I. /Zi /TC /W4 /WX /fsanitize=address /DENABLE_ASSERT=1 /fp:fast
set lflags=/incremental:no /opt:ref /opt:icf

echo.
echo - Testing with MSVC
cl /Fetest_msvc /analyze %ccflags% base/*.c test.c /link %lflags%
rem if %errorlevel% equ 0 call test_msvc.exe else goto EOF
rem del test_msvc.exe

echo.
echo - Testing with Clang
clang-cl /Fetest_clang %ccflags% base/*.c test.c /link %lflags%
rem if %errorlevel% equ 0 call test_clang.exe else goto EOF
rem del test_clang.exe

del *.obj
