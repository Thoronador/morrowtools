# - Try to find liblz4
#
# Once done this will define
#  LZ4_FOUND - System has liblz4
#  LZ4_INCLUDE_DIRS - The liblz4 include directories
#  LZ4_LIBRARIES - The libraries needed to use liblz4

find_package(PkgConfig)
pkg_check_modules(PC_LIBLZ4 liblz4)

find_path(LZ4_INCLUDE_PATH lz4.h
          HINTS /usr/include/ /mingw64/include/ ${PC_LIBLZ4_INCLUDE_DIRS})
if (LZ4_INCLUDE_PATH)
  set(LZ4_INCLUDE_DIRS
    ${LZ4_INCLUDE_PATH}
  )
else ()
  message("Include path for liblz4 could not be determined!")
endif (LZ4_INCLUDE_PATH)

find_library(LZ4_LIBRARIES NAMES lz4 liblz4
             HINTS ${PC_LIBLZ4_LIBRARY_DIRS})

if (NOT LZ4_LIBRARIES)
   message( "Library for lz4 was not found!")
endif ()

if (LZ4_INCLUDE_DIRS AND LZ4_LIBRARIES)
   set(LZ4_FOUND TRUE)
else ()
   set(LZ4_FOUND FALSE)
endif ()

if (LZ4_FIND_REQUIRED)
  if (NOT SQLite3_FOUND)
    message(FATAL_ERROR "Could not find liblz4!")
  endif ()
endif ()

mark_as_advanced(LZ4_LIBRARIES LZ4_INCLUDE_DIRS)
