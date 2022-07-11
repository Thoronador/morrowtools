# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(conv_cams)

set(conv_cams_sources
    ../../base/CompressionFunctions.cpp
    ../../base/DirectoryFunctions.cpp
    ../../base/FileFunctions.cpp
    ../../base/lz4Compression.cpp
    ../../base/RandomFunctions.cpp
    ../../base/RegistryFunctions.cpp
    ../../base/SlashFunctions.cpp
    ../../base/UtilityFunctions.cpp
    ../../mw/base/HelperIO.cpp
    ../base/ESMFileContents.cpp
    ../base/ESMReader.cpp
    ../base/ESMReaderContentsBase.cpp
    ../base/ESMWriter.cpp
    ../base/ESMWriterContents.cpp
    ../base/Group.cpp
    ../base/Localization.cpp
    ../base/StringTable.cpp
    ../base/TableUtilities.cpp
    ../base/bsa/BSA.cpp
    ../base/bsa/BSADirectoryBlock.cpp
    ../base/bsa/BSADirectoryRecord.cpp
    ../base/bsa/BSAFileRecord.cpp
    ../base/bsa/BSAHeader.cpp
    ../base/records/BasicRecord.cpp
    ../base/records/BinarySubRecord.cpp
    ../base/records/CameraShotRecord.cpp
    ../base/records/GenericRecord.cpp
    ../base/records/GroupData.cpp
    ../base/records/NAVIRecord.cpp
    ../base/records/TES4HeaderRecord.cpp
    ESMReaderConvCAMS.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -fexceptions)
    if (CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ENABLE_LTO)
  set(TARGET conv_cams PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(conv_cams ${conv_cams_sources})

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
  target_link_libraries (conv_cams z)
else()
  # use default method to find zlib
  find_package (ZLIB)
  if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (conv_cams ${ZLIB_LIBRARIES})
  else ()
    message ( FATAL_ERROR "zlib was not found!" )
  endif (ZLIB_FOUND)
endif()

# Use lz4, if it is not disabled.
if (NOT DISABLE_LZ4)
  set(LZ4_DIR "../../cmake/" )

  find_package (LZ4)
  if (LZ4_FOUND)
    include_directories(${LZ4_INCLUDE_DIRS})
    target_link_libraries (conv_cams ${LZ4_LIBRARIES})
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
  target_link_libraries(conv_cams stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(conv_cams c++fs)
endif ()
