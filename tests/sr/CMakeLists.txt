cmake_minimum_required (VERSION 3.8...3.31)

# binary executable tests
add_subdirectory (bsa_cli)
if (UNIX AND (NOT MINGW OR NOT CMAKE_HOST_UNIX))
  # bsafs requires FUSE, and that is not available on Windows, so only test it
  # on Unix / Linux and when not cross-compiling.
  add_subdirectory (bsafs)
endif ()
add_subdirectory (conv_cams)
add_subdirectory (formID_finder)
add_subdirectory (small_high_elves)
