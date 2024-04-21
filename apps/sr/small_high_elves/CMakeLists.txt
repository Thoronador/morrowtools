cmake_minimum_required (VERSION 3.8)

project(small_high_elves)

set(small_high_elves_sources
    ../../../lib/base/CompressionFunctions.cpp
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/lz4Compression.cpp
    ../../../lib/base/RandomFunctions.cpp
    ../../../lib/base/RegistryFunctions.cpp
    ../../../lib/base/SlashFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/sr/ESMFileContents.cpp
    ../../../lib/sr/ESMReader.cpp
    ../../../lib/sr/ESMWriter.cpp
    ../../../lib/sr/ESMWriterContents.cpp
    ../../../lib/sr/Group.cpp
    ../../../lib/sr/Localization.cpp
    ../../../lib/sr/StringTable.cpp
    ../../../lib/sr/StringTableCompound.cpp
    ../../../lib/sr/TableUtilities.cpp
    ../../../lib/sr/bsa/BSA.cpp
    ../../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../../lib/sr/bsa/BSAFileRecord.cpp
    ../../../lib/sr/bsa/BSAHeader.cpp
    ../../../lib/sr/records/BasicRecord.cpp
    ../../../lib/sr/records/BinarySubRecord.cpp
    ../../../lib/sr/records/GroupData.cpp
    ../../../lib/sr/records/LocalizedString.cpp
    ../../../lib/sr/records/RaceRecord.cpp
    ../../../lib/sr/records/TES4HeaderRecord.cpp
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
  set(TARGET small_high_elves PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(small_high_elves ${small_high_elves_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows ...")
  try_compile(BUILDS_FOR_WIN64
              ${CMAKE_CURRENT_SOURCE_DIR}/win64-test
              ${CMAKE_CURRENT_SOURCE_DIR}/../../../win64-compile-test
              formID_finder-win64-test)
  if (BUILDS_FOR_WIN64)
    message(STATUS "Cross-compiling for 64-bit Windows ...")
    include_directories( /usr/x86_64-w64-mingw32/include/ )
    link_directories( /usr/x86_64-w64-mingw32/lib )
  else ()
    message(STATUS "Cross-compiling for 32-bit Windows ...")
    include_directories( /usr/i686-w64-mingw32/include/ )
    link_directories( /usr/i686-w64-mingw32/lib )
  endif ()
  target_link_libraries (small_high_elves z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (small_high_elves ${ZLIB_LIBRARIES})
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
    target_link_libraries (small_high_elves ${LZ4_LIBRARIES})
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

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(small_high_elves stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(small_high_elves c++fs)
endif ()
