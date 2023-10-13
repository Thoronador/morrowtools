# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(name_generator)

set(name_generator_sources
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/RegistryFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/DepFiles.cpp
    ../../../lib/mw/ESMReader.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/mw/MapBasedRecordManager.hpp
    ../../../lib/mw/SetBasedRecordManager.hpp
    ../../../lib/mw/records/AIData.cpp
    ../../../lib/mw/records/AIPackages.cpp
    ../../../lib/mw/records/BasicRecord.cpp
    ../../../lib/mw/records/ItemRecord.cpp
    ../../../lib/mw/records/NPCRecord.cpp
    ../../../lib/mw/records/PreNPCRecord.cpp
    ../../../lib/mw/records/RaceRecord.cpp
    ../../../lib/mw/records/TES3Record.cpp
    ../../../lib/mw/records/TravelDestination.cpp
    ESMReaderNameGen.cpp
    Gender.cpp
    NameGenFunctions.cpp
    generators/ArgonianNamesPurge.cpp
    generators/BretonNamesPurge.cpp
    generators/Factory.cpp
    generators/GeneratorArgonianBoth.cpp
    generators/GeneratorArgonianFemale.cpp
    generators/GeneratorArgonianMale.cpp
    generators/GeneratorBretonBoth.cpp
    generators/GeneratorBretonFemale.cpp
    generators/GeneratorBretonMale.cpp
    generators/GeneratorHighElfBoth.cpp
    generators/GeneratorHighElfFemale.cpp
    generators/GeneratorHighElfMale.cpp
    generators/GeneratorImperialBoth.cpp
    generators/GeneratorImperialFemale.cpp
    generators/GeneratorImperialMale.cpp
    generators/GeneratorKhajiitBoth.cpp
    generators/GeneratorKhajiitFemale.cpp
    generators/GeneratorKhajiitMale.cpp
    generators/GeneratorOrcBoth.cpp
    generators/GeneratorOrcFemale.cpp
    generators/GeneratorOrcMale.cpp
    generators/GeneratorRedguardBoth.cpp
    generators/GeneratorRedguardFemale.cpp
    generators/GeneratorRedguardMale.cpp
    generators/GeneratorWoodElfBoth.cpp
    generators/GeneratorWoodElfFemale.cpp
    generators/GeneratorWoodElfMale.cpp
    generators/HighElfNamesPurge.cpp
    generators/ImperialNamesPurge.cpp
    generators/KhajiitNamesPurge.cpp
    generators/OrcNamesPurge.cpp
    generators/RedguardNamesPurge.cpp
    generators/SplitNamesBoth.cpp
    generators/SplitNamesFemale.cpp
    generators/SplitNamesMale.cpp
    generators/SplitNamesOrc.cpp
    generators/WholeNamesBoth.cpp
    generators/WholeNamesFemale.cpp
    generators/WholeNamesMale.cpp
    generators/WoodElfNamesPurge.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    add_definitions (-DMW_UNSAVEABLE_RECORDS)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET name_generator PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

add_executable(name_generator ${name_generator_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(name_generator stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(name_generator c++fs)
endif ()
