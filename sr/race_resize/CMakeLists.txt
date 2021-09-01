# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(race_resize)

set(race_resize_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../../mw/base/HelperIO.cpp
    ../base/ESMFileContents.cpp
    ../base/ESMReader.cpp
    ../base/ESMWriter.cpp
    ../base/ESMWriterContents.cpp
    ../base/Group.cpp
    ../base/StringTable.cpp
    ../base/StringTableCompound.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/BinarySubRecord.cpp
    ../base/records/GroupData.cpp
    ../base/records/LocalizedString.cpp
    ../base/records/RaceRecord.cpp
    ../base/records/TES4HeaderRecord.cpp
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
  set(TARGET race_resize PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(race_resize ${race_resize_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(race_resize stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(race_resize c++fs)
endif ()
