# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(spell_rename)

set(spell_rename_sources ../../base/DirectoryFunctions.cpp ../../base/FileFunctions.cpp ../../base/UtilityFunctions.cpp
                         ../base/DepFiles.cpp ../base/ESMReader.cpp ../base/ESMWriter.cpp
                         ../base/Enchantment.cpp ../base/GameSettings.cpp ../base/HelperIO.cpp ../base/IniFunctions.cpp ../base/MagicEffects.cpp
                         ../base/records/BasicRecord.cpp ../base/records/GMSTRecord.cpp ../base/records/MagicEffectRecord.cpp ../base/records/SpellRecord.cpp
                         ../base/records/TES3Record.cpp 
                         ESMReaderSpells.cpp ESMWriterSpells.cpp main.cpp)

message ( "CMAKE_CXX_COMPILER is set to ${CMAKE_CXX_COMPILER}." )

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(spell_rename ${spell_rename_sources})
