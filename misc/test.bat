@echo off

:: PROJECT DEFINES
set test_path=..\test\

:: GENERAL COMPILER FLAGS
set compiler=               -nologo &:: Suppress Startup Banner
set compiler=%compiler%     -Oi     &:: Use assembly intrinsics where possible
set compiler=%compiler%     -MTd    &:: Include CRT library in the executable (static link)
set compiler=%compiler%     -Gm-    &:: Disable minimal rebuild
set compiler=%compiler%     -GR-    &:: Disable runtime type info (C++)
set compiler=%compiler%     -EHa-   &:: Disable exception handling (C++)
set compiler=%compiler%     -W4     &:: Display warnings up to level 4
set compiler=%compiler%     -WX     &:: Treat all warnings as errors
:: IGNORE WARNINGS
set compiler=%compiler%     -wd4201
set compiler=%compiler%     -wd4100
set compiler=%compiler%     -wd4189
set compiler=%compiler%     -wd4505
set compiler=%compiler%	 -wd4456 &:: Re-declaration of variables inside scope

if not exist .\build mkdir .\build
pushd .\build

:: No optimizations (slow): -Od; all optimizations (fast): -O2

cl -O2 %compiler% %test_path%main.cpp

popd