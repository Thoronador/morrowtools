cmake_minimum_required (VERSION 3.8)

include (CheckIncludeFileCXX)
check_include_file_cxx (catch2/catch_test_macros.hpp HAS_CATCH_V3_HEADER)
check_include_file_cxx (catch2/catch.hpp HAS_CATCH_V2_HEADER)

if (HAS_CATCH_V3_HEADER AND NOT HAS_CATCH_V2_HEADER)
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
