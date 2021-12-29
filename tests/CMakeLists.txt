# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

# component and function tests
add_subdirectory (components)

# Morrowind-related executable tests
add_subdirectory (mw)

# Skyrim-related executable tests
add_subdirectory (sr)


# add script completion benchmark, but not on MinGW builds
if (NOT MINGW)
  add_subdirectory (bench_script_compile)
endif()
