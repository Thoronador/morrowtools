cmake_minimum_required (VERSION 3.8...3.31)

project(data_cleaner)

set(data_cleaner_sources
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/DepFiles.cpp
    ../../../lib/mw/ESMReader.cpp
    ../../../lib/mw/Enchantment.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/mw/IniFunctions.cpp
    ../../../lib/mw/records/AIData.cpp
    ../../../lib/mw/records/AIPackages.cpp
    ../../../lib/mw/records/ActivatorRecord.cpp
    ../../../lib/mw/records/AlchemyPotionRecord.cpp
    ../../../lib/mw/records/ApparatusRecord.cpp
    ../../../lib/mw/records/ArmourRecord.cpp
    ../../../lib/mw/records/BasicRecord.cpp
    ../../../lib/mw/records/BodyPartAssociation.cpp
    ../../../lib/mw/records/BodyPartRecord.cpp
    ../../../lib/mw/records/BookRecord.cpp
    ../../../lib/mw/records/ClothingRecord.cpp
    ../../../lib/mw/records/ContainerRecord.cpp
    ../../../lib/mw/records/CreatureRecord.cpp
    ../../../lib/mw/records/DoorRecord.cpp
    ../../../lib/mw/records/IngredientRecord.cpp
    ../../../lib/mw/records/ItemRecord.cpp
    ../../../lib/mw/records/LightRecord.cpp
    ../../../lib/mw/records/LockpickRecord.cpp
    ../../../lib/mw/records/MiscItemRecord.cpp
    ../../../lib/mw/records/NPCRecord.cpp
    ../../../lib/mw/records/PreNPCRecord.cpp
    ../../../lib/mw/records/ProbeRecord.cpp
    ../../../lib/mw/records/RepairItemRecord.cpp
    ../../../lib/mw/records/StaticRecord.cpp
    ../../../lib/mw/records/TES3Record.cpp
    ../../../lib/mw/records/TravelDestination.cpp
    ../../../lib/mw/records/WeaponRecord.cpp
    DirectoryTraversal.cpp
    ESMReaderCleaner.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE OR ENABLE_SANITIZER)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)

        set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (-DMW_UNSAVEABLE_RECORDS)

if (ENABLE_LTO)
  set(TARGET data_cleaner PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(data_cleaner ${data_cleaner_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(data_cleaner stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(data_cleaner c++fs)
endif ()
