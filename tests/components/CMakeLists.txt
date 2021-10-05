# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# test binary for components and functions
project(component_tests)

set(component_tests_sources
    ../../base/ComparisonFunctor.hpp
    ../../base/CompressionFunctions.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../../mw/base/HelperIO.cpp
    ../../sr/base/FormIDFunctions.cpp
    ../../sr/base/MapBasedRecordManager.hpp
    ../../sr/base/StringTable.cpp
    ../../sr/base/records/AcousticSpaceRecord.cpp
    ../../sr/base/records/ActionRecord.cpp
    ../../sr/base/records/ActivatorRecord.cpp
    ../../sr/base/records/AddOnNodeRecord.cpp
    ../../sr/base/records/AlchemyPotionRecord.cpp
    ../../sr/base/records/AmmunitionRecord.cpp
    ../../sr/base/records/ApparatusRecord.cpp
    ../../sr/base/records/ArmourRecord.cpp
    ../../sr/base/records/BasicRecord.cpp
    ../../sr/base/records/BinarySubRecord.cpp
    ../../sr/base/records/BinarySubRecordExtended.cpp
    ../../sr/base/records/BookRecord.cpp
    ../../sr/base/records/CTDAData.cpp
    ../../sr/base/records/CameraShotRecord.cpp
    ../../sr/base/records/CellRecord.cpp
    ../../sr/base/records/ComponentData.cpp
    ../../sr/base/records/ContainerRecord.cpp
    ../../sr/base/records/DestructionData.cpp
    ../../sr/base/records/EffectBlock.cpp
    ../../sr/base/records/FactionRecord.cpp
    ../../sr/base/records/FloraRecord.cpp
    ../../sr/base/records/FurnitureRecord.cpp
    ../../sr/base/records/GenericRecord.cpp
    ../../sr/base/records/GroupData.cpp
    ../../sr/base/records/IngredientRecord.cpp
    ../../sr/base/records/KeyRecord.cpp
    ../../sr/base/records/KeywordRecord.cpp
    ../../sr/base/records/LocalizedString.cpp
    ../../sr/base/records/MiscObjectRecord.cpp
    ../../sr/base/records/NAVIRecord.cpp
    ../../sr/base/records/PerkRecord.cpp
    ../../sr/base/records/ScrollRecord.cpp
    ../../sr/base/records/ShoutRecord.cpp
    ../../sr/base/records/SimplifiedReferenceRecord.cpp
    ../../sr/base/records/SoulGemRecord.cpp
    ../../sr/base/records/SpellItem.cpp
    ../../sr/base/records/SpellRecord.cpp
    ../../sr/base/records/TalkingActivatorRecord.cpp
    ../../sr/base/records/TES4HeaderRecord.cpp
    ../../sr/base/records/TreeRecord.cpp
    ../../sr/base/records/WordOfPowerRecord.cpp
    base/BufferStream.cpp
    base/ComparisonFunctor.cpp
    base/CompressionFunctions.cpp
    base/DirectoryFunctions.cpp
    base/FileFunctions.cpp
    base/RegistryFunctions.cpp
    base/UtilityFunctions.cpp
    mw/base/HelperIO.cpp
    sr/base/FormIDFunctions.cpp
    sr/base/MapBasedRecordManager.cpp
    sr/base/StringTable.cpp
    sr/base/records/AcousticSpaceRecord.cpp
    sr/base/records/ActionRecord.cpp
    sr/base/records/ActivatorRecord.cpp
    sr/base/records/AddOnNodeRecord.cpp
    sr/base/records/AlchemyPotionRecord.cpp
    sr/base/records/AmmunitionRecord.cpp
    sr/base/records/ApparatusRecord.cpp
    sr/base/records/ArmourRecord.cpp
    sr/base/records/BasicRecord.cpp
    sr/base/records/BinarySubRecord.cpp
    sr/base/records/BinarySubRecordExtended.cpp
    sr/base/records/BookRecord.cpp
    sr/base/records/CTDAData.cpp
    sr/base/records/CTDA_CISx_compound.cpp
    sr/base/records/CameraShotRecord.cpp
    sr/base/records/CellRecord.cpp
    sr/base/records/CellRecord_SubrecordXCLC.cpp
    sr/base/records/ComponentData.cpp
    sr/base/records/ContainerRecord.cpp
    sr/base/records/DestructionData.cpp
    sr/base/records/DestructionStage.cpp
    sr/base/records/EffectBlock.cpp
    sr/base/records/FactionRecord.cpp
    sr/base/records/FactionRecord_InterfactionRelation.cpp
    sr/base/records/FactionRecord_RankData.cpp
    sr/base/records/FactionRecord_VendorData.cpp
    sr/base/records/FloraRecord.cpp
    sr/base/records/FurnitureRecord.cpp
    sr/base/records/GenericRecord.cpp
    sr/base/records/GroupData.cpp
    sr/base/records/IngredientRecord.cpp
    sr/base/records/KeyRecord.cpp
    sr/base/records/KeywordRecord.cpp
    sr/base/records/LocalizedString.cpp
    sr/base/records/MiscObjectRecord.cpp
    sr/base/records/NAVIRecord.cpp
    sr/base/records/PerkRecord.cpp
    sr/base/records/ScrollRecord.cpp
    sr/base/records/ShoutRecord.cpp
    sr/base/records/SimplifiedReferenceRecord.cpp
    sr/base/records/SoulGemRecord.cpp
    sr/base/records/SpellItem.cpp
    sr/base/records/SpellRecord.cpp
    sr/base/records/TalkingActivatorRecord.cpp
    sr/base/records/Tes4HeaderRecord.cpp
    sr/base/records/TreeRecord.cpp
    sr/base/records/WordOfPowerRecord.cpp
    sr/base/records/WrongStringType.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -fexceptions)
endif ()
if (CODE_COVERAGE)
    add_definitions (-O0)
else()
    add_definitions (-O2)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
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
