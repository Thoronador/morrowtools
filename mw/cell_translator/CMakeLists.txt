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

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(cell_translator ${cell_translator_sources})
