@echo off

:: PROJECT DEFINES
set test_path=..\test\
set gtest_path=..\third_party\googletest\googletest
set src_path=..\code\
set build_path=..\build\
set test_exe=run_tests.exe

:: SET Include Paths
set gtest_include_paths=/I"%gtest_path%" /I"%gtest_path%\include"

:: GENERAL COMPILER FLAGS
set compiler=/EHsc /MT /O2

if not exist .\build mkdir .\build
pushd .\build

del /f /q *.obj

:: Compiling Google test main
cl /nologo /std:c++17 %compiler% %gtest_include_paths% -c %gtest_path%\src\gtest-all.cc -Fogtest-all.obj

cl /nologo /std:c++17 %compiler% %gtest_include_paths% -c %gtest_path%\src\gtest_main.cc -Fogtest_main.obj

:: Compiling the emulator
cl /nologo %compiler% /I%src_path% -c %src_path%/cpu.cpp -Focpu.obj

:: Compiling test main
cl /nologo /std:c++17 %compiler% /I%src_path% %gtest_include_paths% %test_path%main_test.cpp *.obj -Fe%test_exe% -link -incremental:no

popd