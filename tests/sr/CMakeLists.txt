# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

# binary executable tests
add_subdirectory (conv_cams)
add_subdirectory (formID_finder)
# Not testable without a Skyrim installation:
# add_subdirectory (race_resize)
