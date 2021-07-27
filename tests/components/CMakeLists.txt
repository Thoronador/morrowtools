# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# test binary for components and functions
project(component_tests)

set(component_tests_sources
    ../../base/ComparisonFunctor.hpp
    ../../base/CompressionFunctions.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    base/BufferStream.cpp
    base/ComparisonFunctor.cpp
    base/CompressionFunctions.cpp
    base/DirectoryFunctions.cpp
    base/FileFunctions.cpp
    base/RegistryFunctions.cpp
    base/UtilityFunctions.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -O2 -fexceptions)
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
add_executable(component_tests ${component_tests_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows, adjusting zlib ...")
  include_directories( /usr/i686-w64-mingw32/include/ )
  link_directories( /usr/i686-w64-mingw32/lib )
  target_link_libraries (component_tests z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (component_tests ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()


# add test case
add_test(NAME test_components
         COMMAND $<TARGET_FILE:component_tests>)
