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
    add_definitions (-Wall -O3 -fexceptions)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(race_resize ${race_resize_sources})
