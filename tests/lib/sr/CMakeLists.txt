cmake_minimum_required (VERSION 3.8...3.31)

# test binary for components and functions
project(lib_sr_tests)

set(lib_sr_tests_sources
    ../../../lib/base/ByteBuffer.cpp
    ../../../lib/base/ComparisonFunctor.hpp
    ../../../lib/base/CompressionFunctions.cpp
    ../../../lib/base/DirectoryFunctions.cpp
    ../../../lib/base/FileFunctions.cpp
    ../../../lib/base/lz4Compression.cpp
    ../../../lib/base/RandomFunctions.cpp
    ../../../lib/base/RegistryFunctions.cpp
    ../../../lib/base/SlashFunctions.cpp
    ../../../lib/base/UtilityFunctions.cpp
    ../../../lib/mw/HelperIO.cpp
    ../../../lib/sr/DependencySolver.cpp
    ../../../lib/sr/ESMFileContents.cpp
    ../../../lib/sr/ESMReader.cpp
    ../../../lib/sr/ESMReaderReIndex.cpp
    ../../../lib/sr/ESMReaderSingleType.hpp
    ../../../lib/sr/FormIDFunctions.cpp
    ../../../lib/sr/Group.cpp
    ../../../lib/sr/Localization.cpp
    ../../../lib/sr/MapBasedRecordManager.hpp
    ../../../lib/sr/StringTable.cpp
    ../../../lib/sr/TableUtilities.cpp
    ../../../lib/sr/bsa/BSA.cpp
    ../../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../../lib/sr/bsa/BSAFileRecord.cpp
    ../../../lib/sr/bsa/BSAHash.cpp
    ../../../lib/sr/bsa/BSAHeader.cpp
    ../../../lib/sr/records/AcousticSpaceRecord.cpp
    ../../../lib/sr/records/ActionRecord.cpp
    ../../../lib/sr/records/ActivatorRecord.cpp
    ../../../lib/sr/records/AddOnNodeRecord.cpp
    ../../../lib/sr/records/AlchemyPotionRecord.cpp
    ../../../lib/sr/records/AmmunitionRecord.cpp
    ../../../lib/sr/records/AnimatedObjectRecord.cpp
    ../../../lib/sr/records/ApparatusRecord.cpp
    ../../../lib/sr/records/ArmourRecord.cpp
    ../../../lib/sr/records/BasicRecord.cpp
    ../../../lib/sr/records/BinarySubRecord.cpp
    ../../../lib/sr/records/BinarySubRecordExtended.cpp
    ../../../lib/sr/records/BookRecord.cpp
    ../../../lib/sr/records/CTDAData.cpp
    ../../../lib/sr/records/CameraShotRecord.cpp
    ../../../lib/sr/records/CellRecord.cpp
    ../../../lib/sr/records/ComponentData.cpp
    ../../../lib/sr/records/ContainerRecord.cpp
    ../../../lib/sr/records/DestructionData.cpp
    ../../../lib/sr/records/EffectBlock.cpp
    ../../../lib/sr/records/FactionRecord.cpp
    ../../../lib/sr/records/FloraRecord.cpp
    ../../../lib/sr/records/FormListRecord.cpp
    ../../../lib/sr/records/FurnitureRecord.cpp
    ../../../lib/sr/records/GenericRecord.cpp
    ../../../lib/sr/records/GroupData.cpp
    ../../../lib/sr/records/IngredientRecord.cpp
    ../../../lib/sr/records/KeyRecord.cpp
    ../../../lib/sr/records/KeywordRecord.cpp
    ../../../lib/sr/records/LocalizedString.cpp
    ../../../lib/sr/records/MiscObjectRecord.cpp
    ../../../lib/sr/records/NAVIRecord.cpp
    ../../../lib/sr/records/NPCRecord.cpp
    ../../../lib/sr/records/PerkRecord.cpp
    ../../../lib/sr/records/QuestRecord.cpp
    ../../../lib/sr/records/RaceRecord.cpp
    ../../../lib/sr/records/ScrollRecord.cpp
    ../../../lib/sr/records/ShoutRecord.cpp
    ../../../lib/sr/records/SimplifiedReferenceRecord.cpp
    ../../../lib/sr/records/SoulGemRecord.cpp
    ../../../lib/sr/records/SpellItem.cpp
    ../../../lib/sr/records/SpellRecord.cpp
    ../../../lib/sr/records/TalkingActivatorRecord.cpp
    ../../../lib/sr/records/TES4HeaderRecord.cpp
    ../../../lib/sr/records/TextureSetRecord.cpp
    ../../../lib/sr/records/TreeRecord.cpp
    ../../../lib/sr/records/WeaponRecord.cpp
    ../../../lib/sr/records/WordOfPowerRecord.cpp
    ../../../lib/sr/records/WorldSpaceRecord.cpp
    ../../../lib/sr/records/quest/AliasEntry.cpp
    ../../../lib/sr/records/quest/IndexEntry.cpp
    ../../../lib/sr/records/quest/QOBJEntry.cpp
    ../../../lib/sr/records/quest/QSDTRecord.cpp
    ../../../lib/sr/records/quest/QSTAEntry.cpp
     ../../../apps/sr/bsa_cli/commands/ArgumentParsingUtilities.cpp
     ../../../apps/sr/bsa_cli/commands/CheckHashes.cpp
     ../../../apps/sr/bsa_cli/commands/CommandFactory.cpp
     ../../../apps/sr/bsa_cli/commands/Commands.cpp
     ../../../apps/sr/bsa_cli/commands/Directories.cpp
     ../../../apps/sr/bsa_cli/commands/DirectoryMetadata.cpp
     ../../../apps/sr/bsa_cli/commands/ExtractAll.cpp
     ../../../apps/sr/bsa_cli/commands/ExtractDirectory.cpp
     ../../../apps/sr/bsa_cli/commands/ExtractFile.cpp
     ../../../apps/sr/bsa_cli/commands/FileMetadata.cpp
     ../../../apps/sr/bsa_cli/commands/Help.cpp
     ../../../apps/sr/bsa_cli/commands/Info.cpp
     ../../../apps/sr/bsa_cli/commands/List.cpp
     ../../../apps/sr/bsa_cli/commands/Operations.cpp
    DependencyElement.cpp
    ESMFileContents.cpp
    ESMReader.cpp
    ESMReaderReIndexMod.cpp
    FormIDFunctions.cpp
    Localization.cpp
    MapBasedRecordManager.cpp
    StringTable.cpp
    TableUtilities.cpp
    TestFactionsReader.cpp
    TestReaderReIndexMod.cpp
    bsa/BSA.cpp
    bsa/BSADirectoryBlock.cpp
    bsa/BSADirectoryRecord.cpp
    bsa/BSAFileRecord.cpp
    bsa/BSAHash.cpp
    bsa/BSAHeader.cpp
    records/AcousticSpaceRecord.cpp
    records/ActionRecord.cpp
    records/ActivatorRecord.cpp
    records/AddOnNodeRecord.cpp
    records/AlchemyPotionRecord.cpp
    records/AmmunitionRecord.cpp
    records/AnimatedObjectRecord.cpp
    records/ApparatusRecord.cpp
    records/ArmourRecord.cpp
    records/BasicRecord.cpp
    records/BinarySubRecord.cpp
    records/BinarySubRecordExtended.cpp
    records/BookRecord.cpp
    records/CTDAData.cpp
    records/CTDA_CISx_compound.cpp
    records/CameraShotRecord.cpp
    records/CellRecord.cpp
    records/CellRecord_SubrecordXCLC.cpp
    records/ComponentData.cpp
    records/ContainerRecord.cpp
    records/DestructionData.cpp
    records/DestructionStage.cpp
    records/EffectBlock.cpp
    records/FactionRecord.cpp
    records/FactionRecord_InterfactionRelation.cpp
    records/FactionRecord_RankData.cpp
    records/FactionRecord_VendorData.cpp
    records/FloraRecord.cpp
    records/FormListRecord.cpp
    records/FurnitureRecord.cpp
    records/GenericRecord.cpp
    records/GroupData.cpp
    records/IngredientRecord.cpp
    records/KeyRecord.cpp
    records/KeywordRecord.cpp
    records/LocalizedString.cpp
    records/MiscObjectRecord.cpp
    records/NAVIRecord.cpp
    records/NPCRecord.cpp
    records/NPCRecord_CSDXstruct.cpp
    records/NPCRecord_FactionElem.cpp
    records/NPCRecord_PerkElem.cpp
    records/NPCRecord_TINXstructure.cpp
    records/PerkRecord.cpp
    records/QuestRecord.cpp
    records/quest/AliasEntry.cpp
    records/quest/IndexEntry.cpp
    records/quest/QOBJEntry.cpp
    records/quest/QSDTRecord.cpp
    records/quest/QSTAEntry.cpp
    records/RaceRecord.cpp
    records/RaceRecord_RaceData.cpp
    records/ScrollRecord.cpp
    records/ShoutRecord.cpp
    records/SimplifiedReferenceRecord.cpp
    records/SoulGemRecord.cpp
    records/SpellItem.cpp
    records/SpellRecord.cpp
    records/SubBlock.cpp
    records/TalkingActivatorRecord.cpp
    records/Tes4HeaderRecord.cpp
    records/Tes4HeaderRecord__MasterFile.cpp
    records/TextureSetRecord.cpp
    records/TreeRecord.cpp
    records/WeaponRecord.cpp
    records/WordOfPowerRecord.cpp
    records/WorldSpaceRecord.cpp
    records/WrongStringType.cpp
    bsa_cli/commands/ArgumentParsingUtilities.cpp
    bsa_cli/commands/CheckHashes.cpp
    bsa_cli/commands/CommandFactory.cpp
    bsa_cli/commands/Commands.cpp
    bsa_cli/commands/Directories.cpp
    bsa_cli/commands/DirectoryMetadata.cpp
    bsa_cli/commands/ExtractAll.cpp
    bsa_cli/commands/ExtractDirectory.cpp
    bsa_cli/commands/ExtractFile.cpp
    bsa_cli/commands/FileMetadata.cpp
    bsa_cli/commands/Help.cpp
    bsa_cli/commands/Info.cpp
    bsa_cli/commands/List.cpp
    bsa_cli/commands/Operations.cpp
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
add_executable(lib_sr_tests ${lib_sr_tests_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows, adjusting zlib ...")
  include_directories( /usr/i686-w64-mingw32/include/ )
  link_directories( /usr/i686-w64-mingw32/lib )
  target_link_libraries (lib_sr_tests z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (lib_sr_tests ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Only use lz4, if it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (lib_sr_tests ${LZ4_LIBRARIES})
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
    target_link_libraries(lib_sr_tests Catch2::Catch2WithMain)
endif ()

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(lib_sr_tests stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(lib_sr_tests c++fs)
endif ()

# add test case
add_test(NAME test_library_sr
         COMMAND $<TARGET_FILE:lib_sr_tests>)
