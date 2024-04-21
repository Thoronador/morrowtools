cmake_minimum_required (VERSION 3.8)

project (morrowtools)

enable_testing ()

# If the option ENABLE_LTO is enabled (e. g. via `cmake -DENABLE_LTO=ON`)
# during the build, then all binaries will use link-time optimization (LTO).
option(ENABLE_LTO "Enable link-time optimization" OFF)
# Not all compilers support LTO / IPO, so it has to be checked.
if (ENABLE_LTO)
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT HAS_LTO_SUPPORT OUTPUT LTO_FAIL_REASON
                      LANGUAGES C CXX)
  if (NOT HAS_LTO_SUPPORT)
    message(FATAL "IPO / LTO is not supported: ${LTO_FAIL_REASON}")
  else()
    message(STATUS "IPO / LTO is supported. Using it.")
  endif()
endif(ENABLE_LTO)

# If ENABLE_STATIC_LINKING is on (e. g. via `cmake -DENABLE_STATIC_LINKING=ON`),
# then all libraries are linked statically. The option is off by default.
#
# Static linking increases the size of the binaries, but those binaries do not
# need the libraries to be present on the system.
option(ENABLE_STATIC_LINKING "Links all libraries statically" OFF)
if (ENABLE_STATIC_LINKING)
  set(CMAKE_LINK_SEARCH_START_STATIC 1)
  set(CMAKE_LINK_SEARCH_END_STATIC 1)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif ()

# option for code coverage
option(CODE_COVERAGE "Build with code coverage (GCC only)" OFF)
# If CODE_COVERAGE is on (e. g. via `cmake -DCODE_COVERAGE=ON`), then all
# binaries are built with code coverage. The option is off by default.
# Currently this only works with GCC.
if (CODE_COVERAGE)
  if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Code coverage builds need GCC compiler!")
  endif ()
  # set coverage options for GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif ()

# fix for failed MinGW detection on GitHub Actions
if (NOT MINGW AND ${CMAKE_CXX_COMPILER} MATCHES "mingw")
  message(STATUS "Note: Applying MinGW detection fix for GHA.")
  set(MINGW 1)
endif()

# option to disable LZ4 features
option(DISABLE_LZ4 "Disables any features using the lz4 library." OFF)
# If DISABLE_LZ4 is on (e. g. via `cmake -DDISABLE_LZ4=ON`),
# then all liblz4 calls are avoided, reducing BSA decompression functionality.
# The option is off by default.

# option to auto-detect liblz4
OPTION(AUTODETECT_LZ4 "Automatically detect availability of liblz4." ON)
# If AUTODETECT_LZ4 is off (e. g. via `cmake -DAUTODETECT_LZ4=OFF`), then the
# build will not try to detect liblz4 automatically.
# The option is on by default.
#
# Note that you should also explicitly set DISABLE_LZ4 to ON or OFF when
# disabling the detection mechanism.

# detect liblz4 based on the environment
if (AUTODETECT_LZ4)
    if (MINGW AND CMAKE_HOST_UNIX)
        # Cross-compiler with MinGW has not LZ4 yet, so disable it.
        set (DISABLE_LZ4 ON)
    elseif (MSVC)
        # MSVC builds have no LZ4 yet, so disable it.
        set (DISABLE_LZ4 ON)
    else ()
        # Other builds usually provide LZ4, so enable it.
        set (DISABLE_LZ4 OFF)
    endif ()
    message(STATUS "Auto-detection of lz4 set DISABLE_LZ4 to ${DISABLE_LZ4}.")
else ()
    message(STATUS "Auto-detection of lz4 is not active.")
endif (AUTODETECT_LZ4)

# Recurse into subdirectory for applications (i. e. executables).
add_subdirectory (apps)

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
