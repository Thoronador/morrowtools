# Recurse into the subdirectories for each Skyrim-related executable.
add_subdirectory (bsa_cli)
if (UNIX AND (NOT MINGW OR NOT CMAKE_HOST_UNIX))
  # bsafs requires FUSE, and that is not available on Windows, so only build it
  # on Unix / Linux and when not cross-compiling.
  add_subdirectory (bsafs)
endif ()
add_subdirectory (conv_cams)
add_subdirectory (formID_finder)
add_subdirectory (small_high_elves)
