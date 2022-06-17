# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(data_cleaner)

set(data_cleaner_sources
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../base/DepFiles.cpp
    ../base/ESMReader.cpp
    ../base/Enchantment.cpp
    ../base/HelperIO.cpp
    ../base/IniFunctions.cpp
    ../base/records/AIData.cpp
    ../base/records/AIPackages.cpp
    ../base/records/ActivatorRecord.cpp
    ../base/records/AlchemyPotionRecord.cpp
    ../base/records/ApparatusRecord.cpp
    ../base/records/ArmourRecord.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/BodyPartAssociation.cpp
    ../base/records/BodyPartRecord.cpp
    ../base/records/BookRecord.cpp
    ../base/records/ClothingRecord.cpp
    ../base/records/ContainerRecord.cpp
    ../base/records/CreatureRecord.cpp
    ../base/records/DoorRecord.cpp
    ../base/records/IngredientRecord.cpp
    ../base/records/ItemRecord.cpp
    ../base/records/LightRecord.cpp
    ../base/records/LockpickRecord.cpp
    ../base/records/MiscItemRecord.cpp
    ../base/records/NPCRecord.cpp
    ../base/records/PreNPCRecord.cpp
    ../base/records/ProbeRecord.cpp
    ../base/records/RepairItemRecord.cpp
    ../base/records/StaticRecord.cpp
    ../base/records/TES3Record.cpp
    ../base/records/TravelDestination.cpp
    ../base/records/WeaponRecord.cpp
    DirectoryTraversal.cpp
    ESMReaderCleaner.cpp
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
