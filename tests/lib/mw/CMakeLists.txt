cmake_minimum_required (VERSION 3.8...3.31)

# test binary for components and functions
project(lib_mw_tests)

set(lib_mw_tests_sources
    ../../../lib/base/ByteBuffer.cpp
    ../../../lib/base/ComparisonFunctor.hpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/DepFiles.cpp
    ../../../lib/mw/Enchantment.cpp
    ../../../lib/mw/ESMReader.cpp
    ../../../lib/mw/ESMReaderGeneric.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/mw/MagicEffects.cpp
    ../../../lib/mw/MapBasedRecordManager.hpp
    ../../../lib/mw/SetBasedRecordManager.hpp
    ../../../lib/mw/Skills.cpp
    ../../../lib/mw/records/ActivatorRecord.cpp
    ../../../lib/mw/records/AIData.cpp
    ../../../lib/mw/records/AIPackages.cpp
    ../../../lib/mw/records/AlchemyPotionRecord.cpp
    ../../../lib/mw/records/ApparatusRecord.cpp
    ../../../lib/mw/records/ArmourRecord.cpp
    ../../../lib/mw/records/BasicRecord.cpp
    ../../../lib/mw/records/BirthSignRecord.cpp
    ../../../lib/mw/records/BodyPartAssociation.cpp
    ../../../lib/mw/records/BodyPartRecord.cpp
    ../../../lib/mw/records/BookRecord.cpp
    ../../../lib/mw/records/CellRecord.cpp
    ../../../lib/mw/records/ClothingRecord.cpp
    ../../../lib/mw/records/ContainerRecord.cpp
    ../../../lib/mw/records/CreatureRecord.cpp
    ../../../lib/mw/records/DialogueInfoRecord.cpp
    ../../../lib/mw/records/DialogueTopicRecord.cpp
    ../../../lib/mw/records/DoorRecord.cpp
    ../../../lib/mw/records/GenericRecord.cpp
    ../../../lib/mw/records/GlobalRecord.cpp
    ../../../lib/mw/records/GameSettingRecord.cpp
    ../../../lib/mw/records/IngredientRecord.cpp
    ../../../lib/mw/records/ItemRecord.cpp
    ../../../lib/mw/records/LightRecord.cpp
    ../../../lib/mw/records/LockpickRecord.cpp
    ../../../lib/mw/records/MagicEffectRecord.cpp
    ../../../lib/mw/records/MiscItemRecord.cpp
    ../../../lib/mw/records/NPCRecord.cpp
    ../../../lib/mw/records/PathGridRecord.cpp
    ../../../lib/mw/records/PreNPCRecord.cpp
    ../../../lib/mw/records/ProbeRecord.cpp
    ../../../lib/mw/records/RaceRecord.cpp
    ../../../lib/mw/records/ReferencedObject.cpp
    ../../../lib/mw/records/RegionRecord.cpp
    ../../../lib/mw/records/RepairItemRecord.cpp
    ../../../lib/mw/records/ScriptRecord.cpp
    ../../../lib/mw/records/SkillRecord.cpp
    ../../../lib/mw/records/SoundRecord.cpp
    ../../../lib/mw/records/SpellRecord.cpp
    ../../../lib/mw/records/StartScriptRecord.cpp
    ../../../lib/mw/records/StaticRecord.cpp
    ../../../lib/mw/records/TES3Record.cpp
    ../../../lib/mw/records/TravelDestination.cpp
    ../../../lib/mw/records/WeaponRecord.cpp
    DepFile.cpp
    DepFileList.cpp
    EnchantmentData.cpp
    ESMReader.cpp
    ESMReaderGeneric.cpp
    HelperIO.cpp
    MagicEffects.cpp
    MapBasedRecordManager.cpp
    SetBasedRecordManager.cpp
    Skills.cpp
    records/ActivatorRecord.cpp
    records/AIData.cpp
    records/AlchemyPotionRecord.cpp
    records/AmbientLight.cpp
    records/ApparatusRecord.cpp
    records/ArmourRecord.cpp
    records/BasicRecord.cpp
    records/BirthSignRecord.cpp
    records/BodyPartAssociation.cpp
    records/BodyPartRecord.cpp
    records/BookRecord.cpp
    records/CellRecord.cpp
    records/ClothingRecord.cpp
    records/ContainerRecord.cpp
    records/CreatureRecord.cpp
    records/DialogueInfoRecord.cpp
    records/DialogueTopicRecord.cpp
    records/DoorRecord.cpp
    records/FuncVarRecord.cpp
    records/GameSettingRecord.cpp
    records/GenericRecord.cpp
    records/GlobalRecord.cpp
    records/GridConnection.cpp
    records/GridPointData.cpp
    records/IngredientRecord.cpp
    records/ItemRecord.cpp
    records/LightRecord.cpp
    records/LockpickRecord.cpp
    records/MagicEffectRecord.cpp
    records/MiscItemRecord.cpp
    records/NPC_AIActivate.cpp
    records/NPC_AIEscort.cpp
    records/NPC_AIFollow.cpp
    records/NPC_AITravel.cpp
    records/NPC_AIWander.cpp
    records/NPCRecord.cpp
    records/PathGridRecord.cpp
    records/PreNPCRecord.cpp
    records/ProbeRecord.cpp
    records/RaceRecord.cpp
    records/RefDoorData.cpp
    records/ReferencedObject.cpp
    records/RegionRecord.cpp
    records/RepairItemRecord.cpp
    records/ScriptRecord.cpp
    records/SkillRecord.cpp
    records/SoundChanceRecord.cpp
    records/SoundRecord.cpp
    records/SpellRecord.cpp
    records/StartScriptRecord.cpp
    records/StaticRecord.cpp
    records/TES3Record.cpp
    records/TravelDestination.cpp
    records/WeaponRecord.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE OR ENABLE_SANITIZER)
        add_definitions (-O0)
    else()
        add_definitions (-O2)

        set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
    endif()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(lib_mw_tests ${lib_mw_tests_sources})

# Link to static library of Catch2 v3, if necessary.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(lib_mw_tests Catch2::Catch2WithMain)
endif ()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(lib_mw_tests stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(lib_mw_tests c++fs)
endif ()

# add test case
add_test(NAME test_library_mw
         COMMAND $<TARGET_FILE:lib_mw_tests>)
