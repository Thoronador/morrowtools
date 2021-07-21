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
    ../base/GameSettings.cpp
    ../base/HelperIO.cpp
    ../base/IniFunctions.cpp
    ../base/MagicEffects.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/GMSTRecord.cpp
    ../base/records/MagicEffectRecord.cpp
    ../base/records/SpellRecord.cpp
    ../base/records/TES3Record.cpp
    ESMReaderSpells.cpp
    ESMWriterSpells.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -O3 -fexceptions)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(spell_rename ${spell_rename_sources})
