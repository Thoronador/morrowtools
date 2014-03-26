# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(formID_finder)

add_definitions(-DSR_UNSAVEABLE_RECORDS -DSR_NO_SINGLETON_EQUALITY_CHECK -DSR_NO_RECORD_EQUALITY)

set(formID_finder_sources
    ../../base/CompressionFunctions.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../../mw/base/HelperIO.cpp
    ../base/DependencySolver.cpp
    ../base/ESMReader.cpp
    ../base/ESMReaderReIndex.cpp
    ../base/FormIDFunctions.cpp
    ../base/StringTable.cpp
    ../base/records/ActivatorRecord.cpp
    ../base/records/AlchemyPotionRecord.cpp
    ../base/records/AmmunitionRecord.cpp
    ../base/records/ApparatusRecord.cpp
    ../base/records/ArmourRecord.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/BinarySubRecord.cpp
    ../base/records/BinarySubRecordExtended.cpp
    ../base/records/BookRecord.cpp
    ../base/records/CTDAData.cpp
    ../base/records/CellRecord.cpp
    ../base/records/ComponentData.cpp
    ../base/records/ContainerRecord.cpp
    ../base/records/DestructionData.cpp
    ../base/records/EffectBlock.cpp
    ../base/records/FactionRecord.cpp
    ../base/records/FloraRecord.cpp
    ../base/records/FurnitureRecord.cpp
    ../base/records/GroupData.cpp
    ../base/records/IngredientRecord.cpp
    ../base/records/KeyRecord.cpp
    ../base/records/LocalizedString.cpp
    ../base/records/MiscObjectRecord.cpp
    ../base/records/NPCRecord.cpp
    ../base/records/PerkRecord.cpp
    ../base/records/QuestRecord.cpp
    ../base/records/ScrollRecord.cpp
    ../base/records/ShoutRecord.cpp
    ../base/records/SimplifiedReferenceRecord.cpp
    ../base/records/SoulGemRecord.cpp
    ../base/records/SpellRecord.cpp
    ../base/records/TES4HeaderRecord.cpp
    ../base/records/TalkingActivatorRecord.cpp
    ../base/records/TreeRecord.cpp
    ../base/records/WeaponRecord.cpp
    ../base/records/WordOfPowerRecord.cpp
    ../base/records/WorldSpaceRecord.cpp
    ESMReaderFinder.cpp
    ESMReaderFinderReferences.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(formID_finder ${formID_finder_sources})

find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (formID_finder ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)
