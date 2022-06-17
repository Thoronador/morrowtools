# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(bench_script_compile)

set(bench_script_compile_sources
    ../../base/ByteBuffer.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../../mw/base/DepFiles.cpp
    ../../mw/base/ESMReader.cpp
    ../../mw/base/ESMReaderScriptCompiler.cpp
    ../../mw/base/HelperIO.cpp
    ../../mw/base/MagicEffects.cpp
    ../../mw/base/RegistryFunctions.hpp
    ../../mw/base/ReturnCodes.hpp
    ../../mw/base/Scripts.hpp
    ../../mw/base/records/AIData.cpp
    ../../mw/base/records/AIPackages.cpp
    ../../mw/base/records/ActivatorRecord.cpp
    ../../mw/base/records/BasicRecord.cpp
    ../../mw/base/records/ContainerRecord.cpp
    ../../mw/base/records/CreatureRecord.cpp
    ../../mw/base/records/GlobalRecord.cpp
    ../../mw/base/records/ItemRecord.cpp
    ../../mw/base/records/MagicEffectRecord.cpp
    ../../mw/base/records/NPCRecord.cpp
    ../../mw/base/records/PreNPCRecord.cpp
    ../../mw/base/records/ScriptRecord.cpp
    ../../mw/base/records/StaticRecord.cpp
    ../../mw/base/records/TES3Record.cpp
    ../../mw/base/records/TravelDestination.cpp
    ../../mw/base/script_compiler/CompiledChunk.cpp
    ../../mw/base/script_compiler/CompilerCodes.hpp
    ../../mw/base/script_compiler/ParserNode.cpp
    ../../mw/base/script_compiler/ScriptCompiler.cpp
    CompileStatistics.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -fexceptions)
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
  set(TARGET bench_script_compile PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_executable(bench_script_compile ${bench_script_compile_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(bench_script_compile stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(bench_script_compile c++fs)
endif ()
