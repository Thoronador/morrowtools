cmake_minimum_required (VERSION 3.8)

# MSYS2 / MinGW on Windows uses Catch 3.x.
if (MINGW AND CMAKE_HOST_WIN32)
  set(HAS_CATCH_V3 ON)
else ()
  set(HAS_CATCH_V3 OFF)
endif ()

# tests of application-specific code
add_subdirectory (apps)

# component and function tests
add_subdirectory (lib)

# Morrowind-related executable tests
add_subdirectory (mw)

# Skyrim-related executable tests
add_subdirectory (sr)

# add script completion benchmark, but not on MinGW builds
if (NOT MINGW)
  add_subdirectory (bench_script_compile)
endif()
