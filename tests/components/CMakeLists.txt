cmake_minimum_required (VERSION 3.8)

# test binary for components and functions
project(component_tests)

set(component_tests_sources
    ../../lib/base/ByteBuffer.cpp
    ../../lib/base/ComparisonFunctor.hpp
    ../../lib/base/CompressionFunctions.cpp
    ../../lib/base/DirectoryFunctions.cpp
    ../../lib/base/FileFunctions.cpp
    ../../lib/base/lz4Compression.cpp
    ../../lib/base/RandomFunctions.cpp
    ../../lib/base/RegistryFunctions.cpp
    ../../lib/base/SlashFunctions.cpp
    ../../lib/base/UtilityFunctions.cpp
    ../../lib/mw/DepFiles.cpp
    ../../lib/mw/Enchantment.cpp
    ../../lib/mw/ESMReader.cpp
    ../../lib/mw/ESMReaderGeneric.cpp
    ../../lib/mw/HelperIO.cpp
    ../../lib/mw/MagicEffects.cpp
    ../../lib/mw/MapBasedRecordManager.hpp
    ../../lib/mw/SetBasedRecordManager.hpp
    ../../lib/mw/Skills.cpp
    ../../lib/mw/records/ActivatorRecord.cpp
    ../../lib/mw/records/AIData.cpp
    ../../lib/mw/records/AIPackages.cpp
    ../../lib/mw/records/AlchemyPotionRecord.cpp
    ../../lib/mw/records/ApparatusRecord.cpp
    ../../lib/mw/records/ArmourRecord.cpp
    ../../lib/mw/records/BasicRecord.cpp
    ../../lib/mw/records/BodyPartAssociation.cpp
    ../../lib/mw/records/BodyPartRecord.cpp
    ../../lib/mw/records/BookRecord.cpp
    ../../lib/mw/records/CellRecord.cpp
    ../../lib/mw/records/ClothingRecord.cpp
    ../../lib/mw/records/ContainerRecord.cpp
    ../../lib/mw/records/CreatureRecord.cpp
    ../../lib/mw/records/DialogueInfoRecord.cpp
    ../../lib/mw/records/DialogueTopicRecord.cpp
    ../../lib/mw/records/DoorRecord.cpp
    ../../lib/mw/records/GenericRecord.cpp
    ../../lib/mw/records/GlobalRecord.cpp
    ../../lib/mw/records/GameSettingRecord.cpp
    ../../lib/mw/records/IngredientRecord.cpp
    ../../lib/mw/records/ItemRecord.cpp
    ../../lib/mw/records/LightRecord.cpp
    ../../lib/mw/records/LockpickRecord.cpp
    ../../lib/mw/records/MagicEffectRecord.cpp
    ../../lib/mw/records/MiscItemRecord.cpp
    ../../lib/mw/records/NPCRecord.cpp
    ../../lib/mw/records/PathGridRecord.cpp
    ../../lib/mw/records/PreNPCRecord.cpp
    ../../lib/mw/records/ProbeRecord.cpp
    ../../lib/mw/records/ReferencedObject.cpp
    ../../lib/mw/records/RegionRecord.cpp
    ../../lib/mw/records/RepairItemRecord.cpp
    ../../lib/mw/records/ScriptRecord.cpp
    ../../lib/mw/records/SkillRecord.cpp
    ../../lib/mw/records/SpellRecord.cpp
    ../../lib/mw/records/StartScriptRecord.cpp
    ../../lib/mw/records/StaticRecord.cpp
    ../../lib/mw/records/TES3Record.cpp
    ../../lib/mw/records/TravelDestination.cpp
    ../../lib/mw/records/WeaponRecord.cpp
    ../../lib/sr/DependencySolver.cpp
    ../../lib/sr/ESMFileContents.cpp
    ../../lib/sr/ESMReader.cpp
    ../../lib/sr/ESMReaderReIndex.cpp
    ../../lib/sr/ESMReaderSingleType.hpp
    ../../lib/sr/FormIDFunctions.cpp
    ../../lib/sr/Group.cpp
    ../../lib/sr/Localization.cpp
    ../../lib/sr/MapBasedRecordManager.hpp
    ../../lib/sr/StringTable.cpp
    ../../lib/sr/TableUtilities.cpp
    ../../lib/sr/bsa/BSA.cpp
    ../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../lib/sr/bsa/BSAFileRecord.cpp
    ../../lib/sr/bsa/BSAHash.cpp
    ../../lib/sr/bsa/BSAHeader.cpp
    ../../lib/sr/records/AcousticSpaceRecord.cpp
    ../../lib/sr/records/ActionRecord.cpp
    ../../lib/sr/records/ActivatorRecord.cpp
    ../../lib/sr/records/AddOnNodeRecord.cpp
    ../../lib/sr/records/AlchemyPotionRecord.cpp
    ../../lib/sr/records/AmmunitionRecord.cpp
    ../../lib/sr/records/AnimatedObjectRecord.cpp
    ../../lib/sr/records/ApparatusRecord.cpp
    ../../lib/sr/records/ArmourRecord.cpp
    ../../lib/sr/records/BasicRecord.cpp
    ../../lib/sr/records/BinarySubRecord.cpp
    ../../lib/sr/records/BinarySubRecordExtended.cpp
    ../../lib/sr/records/BookRecord.cpp
    ../../lib/sr/records/CTDAData.cpp
    ../../lib/sr/records/CameraShotRecord.cpp
    ../../lib/sr/records/CellRecord.cpp
    ../../lib/sr/records/ComponentData.cpp
    ../../lib/sr/records/ContainerRecord.cpp
    ../../lib/sr/records/DestructionData.cpp
    ../../lib/sr/records/EffectBlock.cpp
    ../../lib/sr/records/FactionRecord.cpp
    ../../lib/sr/records/FloraRecord.cpp
    ../../lib/sr/records/FormListRecord.cpp
    ../../lib/sr/records/FurnitureRecord.cpp
    ../../lib/sr/records/GenericRecord.cpp
    ../../lib/sr/records/GroupData.cpp
    ../../lib/sr/records/IngredientRecord.cpp
    ../../lib/sr/records/KeyRecord.cpp
    ../../lib/sr/records/KeywordRecord.cpp
    ../../lib/sr/records/LocalizedString.cpp
    ../../lib/sr/records/MiscObjectRecord.cpp
    ../../lib/sr/records/NAVIRecord.cpp
    ../../lib/sr/records/NPCRecord.cpp
    ../../lib/sr/records/PerkRecord.cpp
    ../../lib/sr/records/QuestRecord.cpp
    ../../lib/sr/records/RaceRecord.cpp
    ../../lib/sr/records/ScrollRecord.cpp
    ../../lib/sr/records/ShoutRecord.cpp
    ../../lib/sr/records/SimplifiedReferenceRecord.cpp
    ../../lib/sr/records/SoulGemRecord.cpp
    ../../lib/sr/records/SpellItem.cpp
    ../../lib/sr/records/SpellRecord.cpp
    ../../lib/sr/records/TalkingActivatorRecord.cpp
    ../../lib/sr/records/TES4HeaderRecord.cpp
    ../../lib/sr/records/TreeRecord.cpp
    ../../lib/sr/records/WeaponRecord.cpp
    ../../lib/sr/records/WordOfPowerRecord.cpp
    ../../lib/sr/records/WorldSpaceRecord.cpp
    ../../lib/sr/records/quest/AliasEntry.cpp
    ../../lib/sr/records/quest/IndexEntry.cpp
    ../../lib/sr/records/quest/QOBJEntry.cpp
    ../../lib/sr/records/quest/QSDTRecord.cpp
    ../../lib/sr/records/quest/QSTAEntry.cpp
    ../../sr/bsa_cli/commands/ArgumentParsingUtilities.cpp
    ../../sr/bsa_cli/commands/CheckHashes.cpp
    ../../sr/bsa_cli/commands/CommandFactory.cpp
    ../../sr/bsa_cli/commands/Commands.cpp
    ../../sr/bsa_cli/commands/Directories.cpp
    ../../sr/bsa_cli/commands/DirectoryMetadata.cpp
    ../../sr/bsa_cli/commands/ExtractAll.cpp
    ../../sr/bsa_cli/commands/ExtractDirectory.cpp
    ../../sr/bsa_cli/commands/ExtractFile.cpp
    ../../sr/bsa_cli/commands/FileMetadata.cpp
    ../../sr/bsa_cli/commands/Help.cpp
    ../../sr/bsa_cli/commands/Info.cpp
    ../../sr/bsa_cli/commands/List.cpp
    ../../sr/bsa_cli/commands/Operations.cpp
    ../../sr/formID_finder/AuxFunctions.cpp
    base/BufferStream.cpp
    base/ByteBuffer.cpp
    base/ComparisonFunctor.cpp
    base/CompressionFunctions.cpp
    base/DirectoryFunctions.cpp
    base/FileFunctions.cpp
    base/lz4Compression.cpp
    base/RandomFunctions.cpp
    base/RegistryFunctions.cpp
    base/SlashFunctions.cpp
    base/UtilityFunctions.cpp
    mw/DepFile.cpp
    mw/DepFileList.cpp
    mw/EnchantmentData.cpp
    mw/ESMReader.cpp
    mw/ESMReaderGeneric.cpp
    mw/HelperIO.cpp
    mw/MagicEffects.cpp
    mw/MapBasedRecordManager.cpp
    mw/SetBasedRecordManager.cpp
    mw/Skills.cpp
    mw/records/ActivatorRecord.cpp
    mw/records/AIData.cpp
    mw/records/AlchemyPotionRecord.cpp
    mw/records/AmbientLight.cpp
    mw/records/ApparatusRecord.cpp
    mw/records/ArmourRecord.cpp
    mw/records/BasicRecord.cpp
    mw/records/BodyPartAssociation.cpp
    mw/records/BodyPartRecord.cpp
    mw/records/BookRecord.cpp
    mw/records/CellRecord.cpp
    mw/records/ClothingRecord.cpp
    mw/records/ContainerRecord.cpp
    mw/records/CreatureRecord.cpp
    mw/records/DialogueInfoRecord.cpp
    mw/records/DialogueTopicRecord.cpp
    mw/records/DoorRecord.cpp
    mw/records/FuncVarRecord.cpp
    mw/records/GameSettingRecord.cpp
    mw/records/GenericRecord.cpp
    mw/records/GlobalRecord.cpp
    mw/records/GridConnection.cpp
    mw/records/GridPointData.cpp
    mw/records/IngredientRecord.cpp
    mw/records/ItemRecord.cpp
    mw/records/LightRecord.cpp
    mw/records/LockpickRecord.cpp
    mw/records/MagicEffectRecord.cpp
    mw/records/MiscItemRecord.cpp
    mw/records/NPC_AIActivate.cpp
    mw/records/NPC_AIEscort.cpp
    mw/records/NPC_AIFollow.cpp
    mw/records/NPC_AITravel.cpp
    mw/records/NPC_AIWander.cpp
    mw/records/NPCRecord.cpp
    mw/records/PathGridRecord.cpp
    mw/records/PreNPCRecord.cpp
    mw/records/ProbeRecord.cpp
    mw/records/RefDoorData.cpp
    mw/records/ReferencedObject.cpp
    mw/records/RegionRecord.cpp
    mw/records/RepairItemRecord.cpp
    mw/records/ScriptRecord.cpp
    mw/records/SkillRecord.cpp
    mw/records/SoundChanceRecord.cpp
    mw/records/SpellRecord.cpp
    mw/records/StartScriptRecord.cpp
    mw/records/StaticRecord.cpp
    mw/records/TES3Record.cpp
    mw/records/TravelDestination.cpp
    mw/records/WeaponRecord.cpp
    sr/DependencyElement.cpp
    sr/ESMFileContents.cpp
    sr/ESMReader.cpp
    sr/ESMReaderReIndexMod.cpp
    sr/FormIDFunctions.cpp
    sr/Localization.cpp
    sr/MapBasedRecordManager.cpp
    sr/StringTable.cpp
    sr/TableUtilities.cpp
    sr/TestFactionsReader.cpp
    sr/TestReaderReIndexMod.cpp
    sr/bsa/BSA.cpp
    sr/bsa/BSADirectoryBlock.cpp
    sr/bsa/BSADirectoryRecord.cpp
    sr/bsa/BSAFileRecord.cpp
    sr/bsa/BSAHash.cpp
    sr/bsa/BSAHeader.cpp
    sr/records/AcousticSpaceRecord.cpp
    sr/records/ActionRecord.cpp
    sr/records/ActivatorRecord.cpp
    sr/records/AddOnNodeRecord.cpp
    sr/records/AlchemyPotionRecord.cpp
    sr/records/AmmunitionRecord.cpp
    sr/records/AnimatedObjectRecord.cpp
    sr/records/ApparatusRecord.cpp
    sr/records/ArmourRecord.cpp
    sr/records/BasicRecord.cpp
    sr/records/BinarySubRecord.cpp
    sr/records/BinarySubRecordExtended.cpp
    sr/records/BookRecord.cpp
    sr/records/CTDAData.cpp
    sr/records/CTDA_CISx_compound.cpp
    sr/records/CameraShotRecord.cpp
    sr/records/CellRecord.cpp
    sr/records/CellRecord_SubrecordXCLC.cpp
    sr/records/ComponentData.cpp
    sr/records/ContainerRecord.cpp
    sr/records/DestructionData.cpp
    sr/records/DestructionStage.cpp
    sr/records/EffectBlock.cpp
    sr/records/FactionRecord.cpp
    sr/records/FactionRecord_InterfactionRelation.cpp
    sr/records/FactionRecord_RankData.cpp
    sr/records/FactionRecord_VendorData.cpp
    sr/records/FloraRecord.cpp
    sr/records/FormListRecord.cpp
    sr/records/FurnitureRecord.cpp
    sr/records/GenericRecord.cpp
    sr/records/GroupData.cpp
    sr/records/IngredientRecord.cpp
    sr/records/KeyRecord.cpp
    sr/records/KeywordRecord.cpp
    sr/records/LocalizedString.cpp
    sr/records/MiscObjectRecord.cpp
    sr/records/NAVIRecord.cpp
    sr/records/NPCRecord.cpp
    sr/records/NPCRecord_CSDXstruct.cpp
    sr/records/NPCRecord_FactionElem.cpp
    sr/records/NPCRecord_PerkElem.cpp
    sr/records/NPCRecord_TINXstructure.cpp
    sr/records/PerkRecord.cpp
    sr/records/QuestRecord.cpp
    sr/records/quest/AliasEntry.cpp
    sr/records/quest/IndexEntry.cpp
    sr/records/quest/QOBJEntry.cpp
    sr/records/quest/QSDTRecord.cpp
    sr/records/quest/QSTAEntry.cpp
    sr/records/RaceRecord.cpp
    sr/records/RaceRecord_RaceData.cpp
    sr/records/ScrollRecord.cpp
    sr/records/ShoutRecord.cpp
    sr/records/SimplifiedReferenceRecord.cpp
    sr/records/SoulGemRecord.cpp
    sr/records/SpellItem.cpp
    sr/records/SpellRecord.cpp
    sr/records/SubBlock.cpp
    sr/records/TalkingActivatorRecord.cpp
    sr/records/Tes4HeaderRecord.cpp
    sr/records/Tes4HeaderRecord__MasterFile.cpp
    sr/records/TreeRecord.cpp
    sr/records/WeaponRecord.cpp
    sr/records/WordOfPowerRecord.cpp
    sr/records/WorldSpaceRecord.cpp
    sr/records/WrongStringType.cpp
    sr/bsa_cli/commands/ArgumentParsingUtilities.cpp
    sr/bsa_cli/commands/CheckHashes.cpp
    sr/bsa_cli/commands/CommandFactory.cpp
    sr/bsa_cli/commands/Commands.cpp
    sr/bsa_cli/commands/Directories.cpp
    sr/bsa_cli/commands/DirectoryMetadata.cpp
    sr/bsa_cli/commands/ExtractAll.cpp
    sr/bsa_cli/commands/ExtractDirectory.cpp
    sr/bsa_cli/commands/ExtractFile.cpp
    sr/bsa_cli/commands/FileMetadata.cpp
    sr/bsa_cli/commands/Help.cpp
    sr/bsa_cli/commands/Info.cpp
    sr/bsa_cli/commands/List.cpp
    sr/bsa_cli/commands/Operations.cpp
    sr/formID_finder/AuxFunctions.cpp
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
add_executable(component_tests ${component_tests_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows, adjusting zlib ...")
  include_directories( /usr/i686-w64-mingw32/include/ )
  link_directories( /usr/i686-w64-mingw32/lib )
  target_link_libraries (component_tests z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (component_tests ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Only use lz4, if it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (component_tests ${LZ4_LIBRARIES})
  else ()
    message ( FATAL_ERROR "liblz4 was not found!" )
  endif (LZ4_FOUND)
else ()
  if (NOT MSVC)
    add_definitions(-DMWTP_NO_LZ4)
  else ()
    add_definitions( /DMWTP_NO_LZ4=1 )
  endif ()
endif ()

# Link to static library of Catch2 v3, if necessary.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(component_tests Catch2::Catch2WithMain)
endif ()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(component_tests stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(component_tests c++fs)
endif ()

# add test case
add_test(NAME test_components
         COMMAND $<TARGET_FILE:component_tests>)
