# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project (morrowtools)

enable_testing ()

# Recurse into subdirectory for Morrowind-related executables.
add_subdirectory (mw)
# Recurse into subdirectory for Skyrim-related executables.
add_subdirectory (sr)

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. While it is possible to build the test binaries,
  # it will not be possible to execute them directly on Linux. It would require
  # Wine to run the tests, but installing it and all of its dependencies would
  # be a bit too much for CI.
  #
  # In case this should be added later:
  #
  #     dpkg --add-architecture i386 && apt-get update && apt-get install wine32
  #
  message(STATUS "Skipping tests due to cross-compiling. Windows 32 test executables cannot run without wine32.")
else()
  # Recurse into subdirectory for tests.
  add_subdirectory (tests)
endif()
