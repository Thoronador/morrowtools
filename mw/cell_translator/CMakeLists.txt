# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(cell_translator)

set(cell_translator_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/DepFiles.cpp
    ../base/ESMReader.cpp
    ../base/ESMReaderGeneric.cpp
    ../base/ESMReaderScriptCompiler.cpp
    ../base/ESMWriter.cpp
    ../base/ESMWriterGeneric.cpp
    ../base/HelperIO.cpp
    ../base/MagicEffects.cpp
    ../base/records/AIData.cpp
    ../base/records/AIPackages.cpp
    ../base/records/ActivatorRecord.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/CellRecord.cpp
    ../base/records/ContainerRecord.cpp
    ../base/records/CreatureRecord.cpp
    ../base/records/DialogueInfoRecord.cpp
    ../base/records/GenericRecord.cpp
    ../base/records/GlobalRecord.cpp
    ../base/records/ItemRecord.cpp
    ../base/records/MagicEffectRecord.cpp
    ../base/records/NPCRecord.cpp
    ../base/records/PathGridRecord.cpp
    ../base/records/PreNPCRecord.cpp
    ../base/records/ScriptRecord.cpp
    ../base/records/StaticRecord.cpp
    ../base/records/TES3Record.cpp
    ../base/records/TravelDestination.cpp
    ../base/script_compiler/CompiledChunk.cpp
    ../base/script_compiler/ParserNode.cpp
    ../base/script_compiler/ScriptCompiler.cpp
    ESMReaderTranslator.cpp
    ESMReaderTranslator.hpp
    TranslateRecords.cpp
    TranslatorXML.cpp
    XMLParser.cpp
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
  set(TARGET cell_translator PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_executable(cell_translator ${cell_translator_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(cell_translator stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(cell_translator c++fs)
endif ()
