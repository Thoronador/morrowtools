cmake_minimum_required (VERSION 3.8)

# test binary for application-related functions
project(apps_sr_tests)

set(apps_sr_tests_sources
    ../../../apps/sr/formID_finder/AuxFunctions.cpp
    ../../../lib/base/CompressionFunctions.cpp
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/lz4Compression.cpp
    ../../../lib/base/RandomFunctions.cpp
    ../../../lib/base/SlashFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/sr/FormIDFunctions.cpp
    ../../../lib/sr/StringTable.cpp
    ../../../lib/sr/bsa/BSA.cpp
    ../../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../../lib/sr/bsa/BSAFileRecord.cpp
    ../../../lib/sr/bsa/BSAHeader.cpp
    ../../../lib/sr/records/BasicRecord.cpp
    ../../../lib/sr/records/BinarySubRecord.cpp
    ../../../lib/sr/records/CellRecord.cpp
    ../../../lib/sr/records/LocalizedString.cpp
    formID_finder/AuxFunctions.cpp
    main.cpp)

if (UNIX AND (NOT MINGW OR NOT CMAKE_HOST_UNIX))
  # bsafs requires FUSE, and that is not available on Windows, so only test it
  # on Unix / Linux and when not cross-compiling.
  list(APPEND apps_sr_tests_sources
    ../../../apps/sr/bsafs/bsafs.cpp
    bsafs/bsafs.cpp)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE OR ENABLE_SANITIZER)
        add_definitions (-O0)
    else()
        add_definitions (-O2)
        set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
    endif()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET apps_sr_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(apps_sr_tests ${apps_sr_tests_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows, adjusting zlib ...")
  include_directories( /usr/i686-w64-mingw32/include/ )
  link_directories( /usr/i686-w64-mingw32/lib )
  target_link_libraries (apps_sr_tests z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (apps_sr_tests ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Only use lz4, if it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (apps_sr_tests ${LZ4_LIBRARIES})
  else ()
    message ( FATAL_ERROR "liblz4 was not found!" )
  endif (LZ4_FOUND)
else ()
  if (NOT MSVC)
    add_definitions(-DMWTP_NO_LZ4)
  else ()
    add_definitions( /DMWTP_NO_LZ4=1 )
  endif ()
endif ()

if (UNIX AND (NOT MINGW OR NOT CMAKE_HOST_UNIX))
  # bsafs requires FUSE, and that is not available on Windows, so only use it
  # on Unix / Linux and when not cross-compiling.

  # find FUSE library
  find_package(PkgConfig)
  pkg_search_module(FUSE REQUIRED fuse3)
  if (FUSE_FOUND)
    include_directories(${FUSE_INCLUDE_DIRS})
    target_link_libraries (apps_sr_tests ${FUSE_LIBRARIES})
  else ()
    message ( FATAL_ERROR "libfuse3 was not found!")
  endif ()
endif ()

# Link to static library of Catch2 v3, if necessary.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(apps_sr_tests Catch2::Catch2WithMain)
endif ()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(apps_sr_tests stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(apps_sr_tests c++fs)
endif ()

# add test case
add_test(NAME test_apps_sr
         COMMAND $<TARGET_FILE:apps_sr_tests>)
