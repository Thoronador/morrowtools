cmake_minimum_required (VERSION 3.8)

project(lz4frame)

set(lz4frame_sources
    ../../base/lz4Compression.cpp
    ../lz4deframe/codes.hpp
    functions.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions -O3)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET lz4frame PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(lz4frame ${lz4frame_sources})

# use default method to find lz4
set(LZ4_DIR "../../cmake/" )
find_package (LZ4)
if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (lz4frame ${LZ4_LIBRARIES})
else ()
    message ( FATAL_ERROR "liblz4 was not found!" )
endif (LZ4_FOUND)

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(lz4frame stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(lz4frame c++fs)
endif ()
