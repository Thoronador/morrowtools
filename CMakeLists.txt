# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project (morrowtools)

# Recurse into subdirectory for Morrowind-related executables.
add_subdirectory (mw)
# Recurse into subdirectory for Skyrim-related executables.
add_subdirectory (sr)
