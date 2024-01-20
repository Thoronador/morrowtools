# - Try to find libfuse
#
# Once done this will define
#  FUSE_FOUND - System has libfuse
#  FUSE_INCLUDE_DIRS - The libfuse include directories
#  FUSE_LIBRARIES - The libraries needed to use libfuse

find_package(PkgConfig)
pkg_check_modules(PC_LIBFUSE fuse)

find_path(FUSE_INCLUDE_PATH fuse.h
          HINTS /usr/include/ /mingw64/include/ ${PC_LIBFUSE_INCLUDE_DIRS})
if (FUSE_INCLUDE_PATH)
  set(FUSE_INCLUDE_DIRS
    ${FUSE_INCLUDE_PATH}
  )
else ()
  message("Include path for libfuse could not be determined!")
endif (FUSE_INCLUDE_PATH)

find_library(FUSE_LIBRARIES NAMES fuse libfuse
             HINTS ${PC_LIBFUSE_LIBRARY_DIRS})

if (NOT FUSE_LIBRARIES)
   message( "Library for fuse was not found!")
endif ()

if (FUSE_INCLUDE_DIRS AND FUSE_LIBRARIES)
   set(FUSE_FOUND TRUE)
else ()
   set(FUSE_FOUND FALSE)
endif ()

if (FUSE_FIND_REQUIRED)
  if (NOT FUSE_FOUND)
    message(FATAL_ERROR "Could not find libfuse!")
  endif ()
endif ()

mark_as_advanced(FUSE_LIBRARIES FUSE_INCLUDE_DIRS)
