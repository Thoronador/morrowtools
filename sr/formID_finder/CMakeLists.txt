# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

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
    ../base/records/quest/AliasEntry.cpp
    ../base/records/quest/IndexEntry.cpp
    ../base/records/quest/QOBJEntry.cpp
    ../base/records/quest/QSDTRecord.cpp
    ../base/records/quest/QSTAEntry.cpp
    ../base/records/ScrollRecord.cpp
    ../base/records/ShoutRecord.cpp
    ../base/records/SimplifiedReferenceRecord.cpp
    ../base/records/SoulGemRecord.cpp
    ../base/records/SpellItem.cpp
    ../base/records/SpellRecord.cpp
    ../base/records/TES4HeaderRecord.cpp
    ../base/records/TalkingActivatorRecord.cpp
    ../base/records/TreeRecord.cpp
    ../base/records/WeaponRecord.cpp
    ../base/records/WordOfPowerRecord.cpp
    ../base/records/WorldSpaceRecord.cpp
    AuxFunctions.cpp
    ESMReaderFinder.cpp
    ESMReaderFinderReferences.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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
              ${CMAKE_CURRENT_SOURCE_DIR}/../../win64-compile-test
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

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(formID_finder stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(formID_finder c++fs)
endif ()
