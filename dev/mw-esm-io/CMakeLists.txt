cmake_minimum_required (VERSION 3.8...3.31)

project(mw-esm-io)

set(mw_esm_io_sources
    ../../lib/base/ByteBuffer.cpp
    ../../lib/base/UtilityFunctions.cpp
    ../../lib/mw/DepFiles.cpp
    ../../lib/mw/ESMReader.cpp
    ../../lib/mw/ESMReaderContents.cpp
    ../../lib/mw/ESMWriter.cpp
    ../../lib/mw/ESMWriterContents.cpp
    ../../lib/mw/Enchantment.cpp
    ../../lib/mw/HelperIO.cpp
    ../../lib/mw/ReturnCodes.hpp
    ../../lib/mw/records/AIData.cpp
    ../../lib/mw/records/AIPackages.cpp
    ../../lib/mw/records/ActivatorRecord.cpp
    ../../lib/mw/records/AlchemyPotionRecord.cpp
    ../../lib/mw/records/ApparatusRecord.cpp
    ../../lib/mw/records/ArmourRecord.cpp
    ../../lib/mw/records/BasicRecord.cpp
    ../../lib/mw/records/BirthSignRecord.cpp
    ../../lib/mw/records/BodyPartAssociation.cpp
    ../../lib/mw/records/BodyPartRecord.cpp
    ../../lib/mw/records/BookRecord.cpp
    ../../lib/mw/records/CellRecord.cpp
    ../../lib/mw/records/ClassRecord.cpp
    ../../lib/mw/records/ClothingRecord.cpp
    ../../lib/mw/records/ContainerRecord.cpp
    ../../lib/mw/records/CreatureRecord.cpp
    ../../lib/mw/records/DialogueInfoRecord.cpp
    ../../lib/mw/records/DialogueTopicRecord.cpp
    ../../lib/mw/records/DoorRecord.cpp
    ../../lib/mw/records/EnchantingRecord.cpp
    ../../lib/mw/records/FactionRecord.cpp
    ../../lib/mw/records/GameSettingRecord.cpp
    ../../lib/mw/records/GlobalRecord.cpp
    ../../lib/mw/records/IngredientRecord.cpp
    ../../lib/mw/records/ItemRecord.cpp
    ../../lib/mw/records/LandscapeRecord.cpp
    ../../lib/mw/records/LandscapeTextureRecord.cpp
    ../../lib/mw/records/LeveledCreatureRecord.cpp
    ../../lib/mw/records/LeveledItemRecord.cpp
    ../../lib/mw/records/LightRecord.cpp
    ../../lib/mw/records/LockpickRecord.cpp
    ../../lib/mw/records/MagicEffectRecord.cpp
    ../../lib/mw/records/MiscItemRecord.cpp
    ../../lib/mw/records/NPCRecord.cpp
    ../../lib/mw/records/PathGridRecord.cpp
    ../../lib/mw/records/PreNPCRecord.cpp
    ../../lib/mw/records/ProbeRecord.cpp
    ../../lib/mw/records/RaceRecord.cpp
    ../../lib/mw/records/ReferencedObject.cpp
    ../../lib/mw/records/RegionRecord.cpp
    ../../lib/mw/records/RepairItemRecord.cpp
    ../../lib/mw/records/ScriptRecord.cpp
    ../../lib/mw/records/SkillRecord.cpp
    ../../lib/mw/records/SoundGeneratorRecord.cpp
    ../../lib/mw/records/SoundRecord.cpp
    ../../lib/mw/records/SpellRecord.cpp
    ../../lib/mw/records/StartScriptRecord.cpp
    ../../lib/mw/records/StaticRecord.cpp
    ../../lib/mw/records/TES3Record.cpp
    ../../lib/mw/records/TravelDestination.cpp
    ../../lib/mw/records/WeaponRecord.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions -O3)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET mw-esm-io PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(mw-esm-io ${mw_esm_io_sources})

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(mw-esm-io stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(mw-esm-io c++fs)
endif ()
