# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(bsa-cli)

set(bsa_cli_sources
    ../../base/CompressionFunctions.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/FormIDFunctions.cpp
    ../base/bsa/BSA.cpp
    ../base/bsa/BSAFileRecord.cpp
    ../base/bsa/BSAFolderBlock.cpp
    ../base/bsa/BSAFolderRecord.cpp
    ../base/bsa/BSAHeader.cpp
    commands/ArgumentParsingUtilities.cpp
    commands/CommandFactory.cpp
    commands/Commands.cpp
    commands/ExtractAll.cpp
    commands/ExtractFile.cpp
    commands/ExtractFolder.cpp
    commands/Folders.cpp
    commands/Help.cpp
    commands/Info.cpp
    commands/List.cpp
    commands/Operations.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

if (ENABLE_LTO)
  set(TARGET bsa-cli PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(bsa-cli ${bsa_cli_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows ...")
  try_compile(BUILDS_FOR_WIN64
              ${CMAKE_CURRENT_SOURCE_DIR}/win64-test
              ${CMAKE_CURRENT_SOURCE_DIR}/../../win64-compile-test
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
  target_link_libraries (bsa-cli z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (bsa-cli ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(bsa-cli stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(bsa-cli c++fs)
endif ()
