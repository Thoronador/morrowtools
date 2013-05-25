# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(race_resize)

set(race_resize_sources ../../base/DirectoryFunctions.cpp ../../base/FileFunctions.cpp ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp ../../mw/base/HelperIO.cpp ../base/ESMFileContents.cpp
    ../base/ESMReader.cpp ../base/ESMWriter.cpp ../base/ESMWriterContents.cpp
    ../base/Group.cpp ../base/StringTable.cpp ../base/StringTableCompound.cpp
    ../base/records/BasicRecord.cpp ../base/records/BinarySubRecord.cpp ../base/records/GroupData.cpp
    ../base/records/LocalizedString.cpp ../base/records/RaceRecord.cpp ../base/records/TES4HeaderRecord.cpp
    main.cpp)

message ( "Info: CMAKE_CXX_COMPILER is set to ${CMAKE_CXX_COMPILER}." )

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(race_resize ${race_resize_sources})
