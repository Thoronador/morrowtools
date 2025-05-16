cmake_minimum_required (VERSION 3.8...3.31)

project(sr-esm-io)

set(sr_esm_io_sources
    ../../lib/base/CompressionFunctions.cpp
    ../../lib/base/DirectoryFunctions.cpp
    ../../lib/base/FileFunctions.cpp
    ../../lib/base/RandomFunctions.cpp
    ../../lib/base/SlashFunctions.cpp
    ../../lib/base/UtilityFunctions.cpp
    ../../lib/base/lz4Compression.cpp
    ../../lib/mw/HelperIO.cpp
    ../../lib/mw/ReturnCodes.hpp
    ../../lib/sr/ESMFileContents.cpp
    ../../lib/sr/ESMReader.cpp
    ../../lib/sr/ESMReaderContentsAll.cpp
    ../../lib/sr/ESMReaderContentsBase.cpp
    ../../lib/sr/ESMWriter.cpp
    ../../lib/sr/ESMWriterContents.cpp
    ../../lib/sr/Group.cpp
    ../../lib/sr/Localization.cpp
    ../../lib/sr/StringTable.cpp
    ../../lib/sr/TableUtilities.cpp
    ../../lib/sr/bsa/BSA.cpp
    ../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../lib/sr/bsa/BSAFileRecord.cpp
    ../../lib/sr/bsa/BSAHash.hpp
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
    ../../lib/sr/records/ArtObjectRecord.cpp
    ../../lib/sr/records/AssociationTypeRecord.cpp
    ../../lib/sr/records/BasicRecord.cpp
    ../../lib/sr/records/BinarySubRecord.cpp
    ../../lib/sr/records/BinarySubRecordExtended.cpp
    ../../lib/sr/records/BookRecord.cpp
    ../../lib/sr/records/CTDAData.cpp
    ../../lib/sr/records/CameraPathRecord.cpp
    ../../lib/sr/records/CameraShotRecord.cpp
    ../../lib/sr/records/CellRecord.cpp
    ../../lib/sr/records/CharacterReferenceRecord.cpp
    ../../lib/sr/records/ClassRecord.cpp
    ../../lib/sr/records/ClimateRecord.cpp
    ../../lib/sr/records/CollisionRecord.cpp
    ../../lib/sr/records/ColourFormRecord.cpp
    ../../lib/sr/records/CombatStyleRecord.cpp
    ../../lib/sr/records/ComponentData.cpp
    ../../lib/sr/records/ContainerRecord.cpp
    ../../lib/sr/records/CraftableObjectRecord.cpp
    ../../lib/sr/records/DOBJRecord.cpp
    ../../lib/sr/records/DebrisRecord.cpp
    ../../lib/sr/records/DestructionData.cpp
    ../../lib/sr/records/DialogBranchRecord.cpp
    ../../lib/sr/records/DialogViewRecord.cpp
    ../../lib/sr/records/DialogueRecord.cpp
    ../../lib/sr/records/DoorRecord.cpp
    ../../lib/sr/records/DualCastDataRecord.cpp
    ../../lib/sr/records/EffectBlock.cpp
    ../../lib/sr/records/EffectShaderRecord.cpp
    ../../lib/sr/records/EnchantmentRecord.cpp
    ../../lib/sr/records/EncounterZoneRecord.cpp
    ../../lib/sr/records/EquipmentSlotRecord.cpp
    ../../lib/sr/records/ExplosionRecord.cpp
    ../../lib/sr/records/EyeRecord.cpp
    ../../lib/sr/records/FactionRecord.cpp
    ../../lib/sr/records/FloraRecord.cpp
    ../../lib/sr/records/FootstepRecord.cpp
    ../../lib/sr/records/FootstepSetRecord.cpp
    ../../lib/sr/records/FormListRecord.cpp
    ../../lib/sr/records/FurnitureRecord.cpp
    ../../lib/sr/records/GMSTRecord.cpp
    ../../lib/sr/records/GenericRecord.cpp
    ../../lib/sr/records/GlobalRecord.cpp
    ../../lib/sr/records/GrassRecord.cpp
    ../../lib/sr/records/GroupData.cpp
    ../../lib/sr/records/HazardRecord.cpp
    ../../lib/sr/records/HeadPartRecord.cpp
    ../../lib/sr/records/IdleAnimationRecord.cpp
    ../../lib/sr/records/ImageSpaceModifierRecord.cpp
    ../../lib/sr/records/ImageSpaceRecord.cpp
    ../../lib/sr/records/ImpactDataRecord.cpp
    ../../lib/sr/records/ImpactDataSetRecord.cpp
    ../../lib/sr/records/IngredientRecord.cpp
    ../../lib/sr/records/KeyRecord.cpp
    ../../lib/sr/records/KeywordRecord.cpp
    ../../lib/sr/records/LandscapeTextureRecord.cpp
    ../../lib/sr/records/LeveledCharacterRecord.cpp
    ../../lib/sr/records/LeveledItemRecord.cpp
    ../../lib/sr/records/LeveledListBaseRecord.cpp
    ../../lib/sr/records/LeveledSpellRecord.cpp
    ../../lib/sr/records/LightRecord.cpp
    ../../lib/sr/records/LightingTemplateRecord.cpp
    ../../lib/sr/records/LoadScreenRecord.cpp
    ../../lib/sr/records/LocalizedString.cpp
    ../../lib/sr/records/LocationRecord.cpp
    ../../lib/sr/records/LocationReferenceTypeRecord.cpp
    ../../lib/sr/records/MagicEffectRecord.cpp
    ../../lib/sr/records/MaterialObjectRecord.cpp
    ../../lib/sr/records/MaterialTypeRecord.cpp
    ../../lib/sr/records/MessageRecord.cpp
    ../../lib/sr/records/MiscObjectRecord.cpp
    ../../lib/sr/records/MovableStaticRecord.cpp
    ../../lib/sr/records/MovementTypeRecord.cpp
    ../../lib/sr/records/MusicTrackRecord.cpp
    ../../lib/sr/records/MusicTypeRecord.cpp
    ../../lib/sr/records/NAVIRecord.cpp
    ../../lib/sr/records/NPCRecord.cpp
    ../../lib/sr/records/NavMeshRecord.cpp
    ../../lib/sr/records/OutfitRecord.cpp
    ../../lib/sr/records/PerkRecord.cpp
    ../../lib/sr/records/PlacedGrenadeRecord.cpp
    ../../lib/sr/records/PlacedHazardRecord.cpp
    ../../lib/sr/records/ProjectileRecord.cpp
    ../../lib/sr/records/QuestRecord.cpp
    ../../lib/sr/records/RaceRecord.cpp
    ../../lib/sr/records/ReferenceRecord.cpp
    ../../lib/sr/records/RelationshipRecord.cpp
    ../../lib/sr/records/ReverbRecord.cpp
    ../../lib/sr/records/ScrollRecord.cpp
    ../../lib/sr/records/ShaderParticleGeometryRecord.cpp
    ../../lib/sr/records/ShoutRecord.cpp
    ../../lib/sr/records/SoulGemRecord.cpp
    ../../lib/sr/records/SoundCategoryRecord.cpp
    ../../lib/sr/records/SoundDescriptorRecord.cpp
    ../../lib/sr/records/SoundRecord.cpp
    ../../lib/sr/records/SpellItem.cpp
    ../../lib/sr/records/SpellRecord.cpp
    ../../lib/sr/records/StaticRecord.cpp
    ../../lib/sr/records/TES4HeaderRecord.cpp
    ../../lib/sr/records/TalkingActivatorRecord.cpp
    ../../lib/sr/records/TextureSetRecord.cpp
    ../../lib/sr/records/TreeRecord.cpp
    ../../lib/sr/records/VisualEffectRecord.cpp
    ../../lib/sr/records/VoiceTypeRecord.cpp
    ../../lib/sr/records/WaterTypeRecord.cpp
    ../../lib/sr/records/WeaponRecord.cpp
    ../../lib/sr/records/WordOfPowerRecord.cpp
    ../../lib/sr/records/WorldSpaceRecord.cpp
    ../../lib/sr/records/quest/AliasEntry.cpp
    ../../lib/sr/records/quest/IndexEntry.cpp
    ../../lib/sr/records/quest/QOBJEntry.cpp
    ../../lib/sr/records/quest/QSDTRecord.cpp
    ../../lib/sr/records/quest/QSTAEntry.cpp
    ../../lib/sr/ReturnCodes.hpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wshadow -fexceptions -O3)
    # Pedantic warnings are disabled, because the #warning pre-processor
    # directive is an extension before C++23, and some of the classes use it.
    # add_definitions (-Wpedantic -pedantic-errors)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET sr-esm-io PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(sr-esm-io ${sr_esm_io_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows ...")
  try_compile(BUILDS_FOR_WIN64
              ${CMAKE_CURRENT_SOURCE_DIR}/win64-test
              ${CMAKE_CURRENT_SOURCE_DIR}/../../../win64-compile-test
              sr-esm-io-win64-test)
  if (BUILDS_FOR_WIN64)
    message(STATUS "Cross-compiling for 64-bit Windows ...")
    include_directories( /usr/x86_64-w64-mingw32/include/ )
    link_directories( /usr/x86_64-w64-mingw32/lib )
  else ()
    message(STATUS "Cross-compiling for 32-bit Windows ...")
    include_directories( /usr/i686-w64-mingw32/include/ )
    link_directories( /usr/i686-w64-mingw32/lib )
  endif ()
  target_link_libraries (sr-esm-io z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (sr-esm-io ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Only use lz4 when it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (sr-esm-io ${LZ4_LIBRARIES})
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

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(sr-esm-io stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(sr-esm-io c++fs)
endif ()
