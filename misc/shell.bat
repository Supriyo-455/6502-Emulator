@echo off

REM Please change the visual studio directory path to the installed location on your system

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set path=w:\cpu_6502_emu\;%path%