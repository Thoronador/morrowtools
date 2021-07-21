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
    add_definitions (-Wall -O3 -fexceptions)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (-DMW_UNSAVEABLE_RECORDS)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(data_cleaner ${data_cleaner_sources})
