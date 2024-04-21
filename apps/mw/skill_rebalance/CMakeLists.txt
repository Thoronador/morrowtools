cmake_minimum_required (VERSION 3.8)

project(skill_rebalance)

set(skill_rebalance_sources
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/RegistryFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/DepFiles.cpp
    ../../../lib/mw/ESMReader.cpp
    ../../../lib/mw/ESMWriter.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/mw/MapBasedRecordManager.hpp
    ../../../lib/mw/Skills.cpp
    ../../../lib/mw/records/BasicRecord.cpp
    ../../../lib/mw/records/GameSettingRecord.cpp
    ../../../lib/mw/records/GenericRecord.cpp
    ../../../lib/mw/records/SkillRecord.cpp
    ../../../lib/mw/records/TES3Record.cpp
    ESMReaderSkillRebalance.cpp
    ESMWriterSkillRebalance.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
