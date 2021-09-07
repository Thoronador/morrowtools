# Test to detect 64 bit Windows

This directory contains a `main.cpp` that compiles when targeting a 64-bit
Windows system (i. e. `_WIN64` is defined) and fails otherwise. It is used to
detect the target system's bitness when cross-compiling for Windows.
Unfortunately, CMake does not provide a way to _reliably_ detect the bitness of
the target system. However, the C++ compiler can detect it easily via the set
preprocessor defines. Therefore we use CMake's
[try_compile](https://cmake.org/cmake/help/latest/command/try_compile.html)
feature to check whether we are targeting Win64 (`main.cpp` compiles) or Win32
(`main.cpp` does not compile).
