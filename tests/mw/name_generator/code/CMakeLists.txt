cmake_minimum_required (VERSION 3.8)

project(name_generator_mw_tests)

set(name_generator_mw_tests_sources
    ../../../../apps/mw/name_generator/Gender.cpp
    ../../../../apps/mw/name_generator/NameGenFunctions.cpp
    ../../../../apps/mw/name_generator/generators/ArgonianNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/BretonNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/ImperialNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/KhajiitNamesPurge.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesBoth.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesFemale.cpp
    ../../../../apps/mw/name_generator/generators/WholeNamesMale.cpp
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
    generators/NamesPurge.cpp
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
