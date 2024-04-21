cmake_minimum_required (VERSION 3.8)

project(bsafs)

set(bsafs_sources
    ../../../lib/base/CompressionFunctions.cpp
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/lz4Compression.cpp
    ../../../lib/base/RandomFunctions.cpp
    ../../../lib/base/SlashFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/sr/bsa/BSA.cpp
    ../../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../../lib/sr/bsa/BSAFileRecord.cpp
    ../../../lib/sr/bsa/BSAHash.cpp
    ../../../lib/sr/bsa/BSAHeader.cpp
    bsafs.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE OR ENABLE_SANITIZER)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
        set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET bsafs PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(bsafs ${bsafs_sources})

# find zlib
find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (bsafs ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)

# Only use lz4, if it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (bsafs ${LZ4_LIBRARIES})
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

# find FUSE library
find_package(PkgConfig)
pkg_search_module(FUSE REQUIRED fuse3)
if (FUSE_FOUND)
  include_directories(${FUSE_INCLUDE_DIRS})
  target_link_libraries (bsafs ${FUSE_LIBRARIES})
else ()
  message ( FATAL_ERROR "libfuse3 was not found!")
endif ()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(bsafs stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(bsafs c++fs)
endif ()
