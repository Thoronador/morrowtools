cmake_minimum_required (VERSION 3.8)

project(name_generator_mw_tests)

set(name_generator_mw_tests_sources
    ../../../../apps/mw/name_generator/Gender.cpp
    ../../../../apps/mw/name_generator/NameGenFunctions.cpp
    ../../../../apps/mw/name_generator/generators/ArgonianNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/BretonNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/Factory.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorArgonianBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorArgonianFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorArgonianMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorBretonBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorBretonFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorBretonMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorHighElfBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorHighElfFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorHighElfMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorImperialBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorImperialFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorImperialMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorKhajiitBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorKhajiitFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorKhajiitMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorNordBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorNordFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorNordMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorOrcBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorOrcFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorOrcMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorRedguardBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorRedguardFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorRedguardMale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorWoodElfBoth.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorWoodElfFemale.cpp
    ../../../../apps/mw/name_generator/generators/GeneratorWoodElfMale.cpp
    ../../../../apps/mw/name_generator/generators/HighElfNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/ImperialNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/KhajiitNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/NordNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/OrcNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/RedguardNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/SplitNamesBoth.cpp
    ../../../../apps/mw/name_generator/generators/SplitNamesFemale.cpp
    ../../../../apps/mw/name_generator/generators/SplitNamesMale.cpp
    ../../../../apps/mw/name_generator/generators/SplitNamesOrc.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesBoth.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesFemale.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesMale.cpp
    ../../../../apps/mw/name_generator/generators/WoodElfNamesPurge.cpp
    ../../../../lib/base/UtilityFunctions.cpp
    ../../../../lib/mw/HelperIO.cpp
    ../../../../lib/mw/records/AIData.cpp
    ../../../../lib/mw/records/AIPackages.cpp
    ../../../../lib/mw/records/BasicRecord.cpp
    ../../../../lib/mw/records/ItemRecord.cpp
    ../../../../lib/mw/records/NPCRecord.cpp
    ../../../../lib/mw/records/PreNPCRecord.cpp
    ../../../../lib/mw/records/RaceRecord.cpp
    ../../../../lib/mw/records/TravelDestination.cpp
    Gender.cpp
    NameGenFunctions.cpp
    generators/Factory.cpp
    generators/GeneratorArgonianX.cpp
    generators/GeneratorBretonX.cpp
    generators/GeneratorHighElfX.cpp
    generators/GeneratorImperialX.cpp
    generators/GeneratorKhajiitX.cpp
    generators/GeneratorNordX.cpp
    generators/GeneratorOrcX.cpp
    generators/GeneratorRedguardX.cpp
    generators/GeneratorWoodElfX.cpp
    generators/NamesPurge.cpp
    generators/SplitNamesBoth.cpp
    generators/SplitNamesFemale.cpp
    generators/SplitNamesMale.cpp
    generators/WholeNamesBoth.cpp
    generators/WholeNamesFemale.cpp
    generators/WholeNamesMale.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else()
        add_definitions (-O2)
    endif()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(name_generator_mw_tests ${name_generator_mw_tests_sources})

# Link to static library of Catch2 v3, if necessary.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(name_generator_mw_tests Catch2::Catch2WithMain)
endif ()

# add test case
add_test(NAME test_name_generator_mw_components
         COMMAND $<TARGET_FILE:name_generator_mw_tests>)
