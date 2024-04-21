cmake_minimum_required (VERSION 3.8)

project(bench_script_compile)

set(bench_script_compile_sources
    ../../lib/base/ByteBuffer.cpp
    ../../lib/base/DirectoryFunctions.cpp
    ../../lib/base/RegistryFunctions.cpp
    ../../lib/base/UtilityFunctions.cpp
    ../../lib/mw/DepFiles.cpp
    ../../lib/mw/ESMReader.cpp
    ../../lib/mw/ESMReaderScriptCompiler.cpp
    ../../lib/mw/HelperIO.cpp
    ../../lib/mw/MagicEffects.cpp
    ../../lib/mw/RegistryFunctions.hpp
    ../../lib/mw/ReturnCodes.hpp
    ../../lib/mw/Scripts.hpp
    ../../lib/mw/records/AIData.cpp
    ../../lib/mw/records/AIPackages.cpp
    ../../lib/mw/records/ActivatorRecord.cpp
    ../../lib/mw/records/BasicRecord.cpp
    ../../lib/mw/records/ContainerRecord.cpp
    ../../lib/mw/records/CreatureRecord.cpp
    ../../lib/mw/records/GlobalRecord.cpp
    ../../lib/mw/records/ItemRecord.cpp
    ../../lib/mw/records/MagicEffectRecord.cpp
    ../../lib/mw/records/NPCRecord.cpp
    ../../lib/mw/records/PreNPCRecord.cpp
    ../../lib/mw/records/ScriptRecord.cpp
    ../../lib/mw/records/StaticRecord.cpp
    ../../lib/mw/records/TES3Record.cpp
    ../../lib/mw/records/TravelDestination.cpp
    ../../lib/mw/script_compiler/CompiledChunk.cpp
    ../../lib/mw/script_compiler/CompilerCodes.hpp
    ../../lib/mw/script_compiler/ParserNode.cpp
    ../../lib/mw/script_compiler/ScriptCompiler.cpp
    CompileStatistics.cpp
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
