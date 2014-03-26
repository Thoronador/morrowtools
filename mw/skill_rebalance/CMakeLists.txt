# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(skill_rebalance)

set(skill_rebalance_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/DepFiles.cpp
    ../base/ESMReader.cpp
    ../base/ESMWriter.cpp
    ../base/GameSettings.cpp
    ../base/HelperIO.cpp
    ../base/Skills.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/GMSTRecord.cpp
    ../base/records/GenericRecord.cpp
    ../base/records/SkillRecord.cpp
    ../base/records/TES3Record.cpp
    ESMReaderSkillRebalance.cpp
    ESMWriterSkillRebalance.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(skill_rebalance ${skill_rebalance_sources})
