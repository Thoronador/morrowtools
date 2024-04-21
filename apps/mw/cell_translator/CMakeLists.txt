cmake_minimum_required (VERSION 3.8)

project(cell_translator)

set(cell_translator_sources
    ../../../lib/base/ByteBuffer.cpp
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/RegistryFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/DepFiles.cpp
    ../../../lib/mw/ESMReader.cpp
    ../../../lib/mw/ESMReaderGeneric.cpp
    ../../../lib/mw/ESMReaderScriptCompiler.cpp
    ../../../lib/mw/ESMWriter.cpp
    ../../../lib/mw/ESMWriterGeneric.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/mw/MagicEffects.cpp
    ../../../lib/mw/records/AIData.cpp
    ../../../lib/mw/records/AIPackages.cpp
    ../../../lib/mw/records/ActivatorRecord.cpp
    ../../../lib/mw/records/BasicRecord.cpp
    ../../../lib/mw/records/CellRecord.cpp
    ../../../lib/mw/records/ContainerRecord.cpp
    ../../../lib/mw/records/CreatureRecord.cpp
    ../../../lib/mw/records/DialogueInfoRecord.cpp
    ../../../lib/mw/records/GenericRecord.cpp
    ../../../lib/mw/records/GlobalRecord.cpp
    ../../../lib/mw/records/ItemRecord.cpp
    ../../../lib/mw/records/MagicEffectRecord.cpp
    ../../../lib/mw/records/NPCRecord.cpp
    ../../../lib/mw/records/PathGridRecord.cpp
    ../../../lib/mw/records/PreNPCRecord.cpp
    ../../../lib/mw/records/ReferencedObject.cpp
    ../../../lib/mw/records/ScriptRecord.cpp
    ../../../lib/mw/records/StaticRecord.cpp
    ../../../lib/mw/records/TES3Record.cpp
    ../../../lib/mw/records/TravelDestination.cpp
    ../../../lib/mw/script_compiler/CompiledChunk.cpp
    ../../../lib/mw/script_compiler/ParserNode.cpp
    ../../../lib/mw/script_compiler/ScriptCompiler.cpp
    ESMReaderTranslator.cpp
    ESMReaderTranslator.hpp
    TranslateRecords.cpp
    TranslatorXML.cpp
    XMLParser.cpp
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
