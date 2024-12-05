cmake_minimum_required (VERSION 3.8...3.31)

# ##################################### #
# tests for small_high_elves executable #
# ##################################### #

# add tests for --help parameters

# default help parameter "--help"
add_test(NAME small_high_elves_help
         COMMAND $<TARGET_FILE:small_high_elves> --help)

# short help parameter with question mark "-?"
add_test(NAME small_high_elves_help_question_mark
         COMMAND $<TARGET_FILE:small_high_elves> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME small_high_elves_help_question_mark_windows
             COMMAND $<TARGET_FILE:small_high_elves> /?)
endif ()


# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- invalid parameter usage
add_test(NAME small_high_elves_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:small_high_elves>)
