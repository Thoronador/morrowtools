cmake_minimum_required (VERSION 3.8)

project(formID_finder)

add_definitions(-DSR_UNSAVEABLE_RECORDS -DSR_NO_SINGLETON_EQUALITY_CHECK -DSR_NO_RECORD_EQUALITY)

set(formID_finder_sources
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
    ../../../lib/sr/ESMReader.cpp
    ../../../lib/sr/ESMReaderReIndex.cpp
    ../../../lib/sr/FormIDFunctions.cpp
    ../../../lib/sr/Localization.cpp
    ../../../lib/sr/StringTable.cpp
    ../../../lib/sr/TableUtilities.cpp
    ../../../lib/sr/bsa/BSA.cpp
    ../../../lib/sr/bsa/BSADirectoryBlock.cpp
    ../../../lib/sr/bsa/BSADirectoryRecord.cpp
    ../../../lib/sr/bsa/BSAFileRecord.cpp
    ../../../lib/sr/bsa/BSAHeader.cpp
    ../../../lib/sr/records/ActivatorRecord.cpp
    ../../../lib/sr/records/AlchemyPotionRecord.cpp
    ../../../lib/sr/records/AmmunitionRecord.cpp
    ../../../lib/sr/records/ApparatusRecord.cpp
    ../../../lib/sr/records/ArmourRecord.cpp
    ../../../lib/sr/records/BasicRecord.cpp
    ../../../lib/sr/records/BinarySubRecord.cpp
    ../../../lib/sr/records/BinarySubRecordExtended.cpp
    ../../../lib/sr/records/BookRecord.cpp
    ../../../lib/sr/records/CTDAData.cpp
    ../../../lib/sr/records/CellRecord.cpp
    ../../../lib/sr/records/ComponentData.cpp
    ../../../lib/sr/records/ContainerRecord.cpp
    ../../../lib/sr/records/DestructionData.cpp
    ../../../lib/sr/records/EffectBlock.cpp
    ../../../lib/sr/records/FactionRecord.cpp
    ../../../lib/sr/records/FloraRecord.cpp
    ../../../lib/sr/records/FurnitureRecord.cpp
    ../../../lib/sr/records/GroupData.cpp
    ../../../lib/sr/records/IngredientRecord.cpp
    ../../../lib/sr/records/KeyRecord.cpp
    ../../../lib/sr/records/LocalizedString.cpp
    ../../../lib/sr/records/MiscObjectRecord.cpp
    ../../../lib/sr/records/NPCRecord.cpp
    ../../../lib/sr/records/PerkRecord.cpp
    ../../../lib/sr/records/QuestRecord.cpp
    ../../../lib/sr/records/quest/AliasEntry.cpp
    ../../../lib/sr/records/quest/IndexEntry.cpp
    ../../../lib/sr/records/quest/QOBJEntry.cpp
    ../../../lib/sr/records/quest/QSDTRecord.cpp
    ../../../lib/sr/records/quest/QSTAEntry.cpp
    ../../../lib/sr/records/ScrollRecord.cpp
    ../../../lib/sr/records/ShoutRecord.cpp
    ../../../lib/sr/records/SimplifiedReferenceRecord.cpp
    ../../../lib/sr/records/SoulGemRecord.cpp
    ../../../lib/sr/records/SpellItem.cpp
    ../../../lib/sr/records/SpellRecord.cpp
    ../../../lib/sr/records/TES4HeaderRecord.cpp
    ../../../lib/sr/records/TalkingActivatorRecord.cpp
    ../../../lib/sr/records/TreeRecord.cpp
    ../../../lib/sr/records/WeaponRecord.cpp
    ../../../lib/sr/records/WordOfPowerRecord.cpp
    ../../../lib/sr/records/WorldSpaceRecord.cpp
    AuxFunctions.cpp
    ESMReaderFinder.cpp
    ESMReaderFinderReferences.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_LTO)
  set(TARGET formID_finder PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(formID_finder ${formID_finder_sources})

if (MINGW AND CMAKE_HOST_UNIX)
  # If compiler is some kind of MinGW, but the CMake host is Unix, then this is
  # a cross-compiling attempt. To get the library paths more easily, we just set
  # them directly, because CMake has troubles finding them.
  message(STATUS "Cross-compiling from Linux for Windows ...")
  try_compile(BUILDS_FOR_WIN64
              ${CMAKE_CURRENT_SOURCE_DIR}/win64-test
              ${CMAKE_CURRENT_SOURCE_DIR}/../../../win64-compile-test
              formID_finder-win64-test)
  if (BUILDS_FOR_WIN64)
    message(STATUS "Cross-compiling for 64-bit Windows ...")
    include_directories( /usr/x86_64-w64-mingw32/include/ )
    link_directories( /usr/x86_64-w64-mingw32/lib )
  else ()
    message(STATUS "Cross-compiling for 32-bit Windows ...")
    include_directories( /usr/i686-w64-mingw32/include/ )
    link_directories( /usr/i686-w64-mingw32/lib )
  endif ()
  target_link_libraries (formID_finder z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (formID_finder ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Only use lz4 when it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (formID_finder ${LZ4_LIBRARIES})
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
  target_link_libraries(formID_finder stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(formID_finder c++fs)
endif ()
