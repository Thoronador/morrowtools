# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(skill_rebalance)

set(skill_rebalance_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/DepFiles.cpp
    ../base/ESMReader.cpp
    ../base/ESMWriter.cpp
    ../base/HelperIO.cpp
    ../base/MapBasedRecordManager.hpp
    ../base/Skills.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/GameSettingRecord.cpp
    ../base/records/GenericRecord.cpp
    ../base/records/SkillRecord.cpp
    ../base/records/TES3Record.cpp
    ESMReaderSkillRebalance.cpp
    ESMWriterSkillRebalance.cpp
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
  set(TARGET skill_rebalance PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_executable(skill_rebalance ${skill_rebalance_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(skill_rebalance stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(skill_rebalance c++fs)
endif ()
