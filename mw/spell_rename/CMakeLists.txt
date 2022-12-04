# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(spell_rename)

set(spell_rename_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/DepFiles.cpp
    ../base/ESMReader.cpp
    ../base/ESMWriter.cpp
    ../base/Enchantment.cpp
    ../base/HelperIO.cpp
    ../base/IniFunctions.cpp
    ../base/MagicEffects.cpp
    ../base/MapBasedRecordManager.hpp
    ../base/records/BasicRecord.cpp
    ../base/records/GameSettingRecord.cpp
    ../base/records/MagicEffectRecord.cpp
    ../base/records/SpellRecord.cpp
    ../base/records/TES3Record.cpp
    ESMReaderSpells.cpp
    ESMWriterSpells.cpp
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
  set(TARGET spell_rename PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_executable(spell_rename ${spell_rename_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(spell_rename stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(spell_rename c++fs)
endif ()
