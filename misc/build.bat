@echo off
setlocal

:: --- Configuration ---
set code_path=..\code\
set exe_name=main.exe
set pdb_name=main.pdb
set cpu_obj=cpu.obj
set main_obj=main.obj

:: --- Compiler Flags ---
set base_compiler_flags= ^
    -nologo  &:: Suppress Startup Banner ^
    -Oi      &:: Use assembly intrinsics where possible ^
    -MTd     &:: Link static multi-threaded debug CRT library ^
    -Gm-     &:: Disable minimal rebuild ^
    -GR-     &:: Disable runtime type info (C++) ^
    -EHa-    &:: Disable C++ exception handling (no SEH assumed) ^
    -W4      &:: Display warnings up to level 4 ^
    -WX      &:: Treat all warnings as errors

:: Ignored warnings
set ignored_warnings= ^
    -wd4201 ^
    -wd4100 ^
    -wd4189 ^
    -wd4505 ^
    -wd4456  &:: Re-declaration of variables inside scope

:: Combine base flags and warnings
set compiler_flags=%base_compiler_flags% %ignored_warnings%

:: --- Optimization & Debug Flags ---
:: Optimization level (-Od = No optimizations; -O2 = Full optimizations)
set optimization_flags=-Od

:: Debug flags
:: NOTE: ZI, Zi is little different,
:: ZI for debug with edit and continue feature,
:: Zi produces pdb file but dont have this feature
set debug_flags= ^
	-Zi      &:: Produce debug info (Program database, .pdb) ^
    -FC      &:: Produce the full path of the source code file

:: --- Build Process ---

:: Create build directory if it doesn't exist
if not exist .\build mkdir .\build

:: Change directory to build directory
pushd .\build

del /f /q *.obj
del /f /q %exe_name%
del /f /q *.pdb
del /f /q *.map 

:: (-c = compile only, -Fo specifies object file name)
cl -c %optimization_flags% %compiler_flags% %debug_flags% -Fo%cpu_obj% %code_path%cpu.cpp

cl %optimization_flags% %compiler_flags% %debug_flags% -Fmmain.map %code_path%main.cpp %cpu_obj% -Fe%exe_name% -link -incremental:no

:: Return to the original directory
popd
endlocal