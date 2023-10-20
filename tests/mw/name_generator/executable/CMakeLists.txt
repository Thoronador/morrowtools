# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ################################### #
# tests for name_generator executable #
# ################################### #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME name_generator_help
         COMMAND $<TARGET_FILE:name_generator> --help)

# short help parameter with question mark "-?"
add_test(NAME name_generator_help_question_mark
         COMMAND $<TARGET_FILE:name_generator> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME name_generator_help_question_mark_windows
             COMMAND $<TARGET_FILE:name_generator> /?)
endif ()

# parameter to show version number
add_test(NAME name_generator_version
         COMMAND $<TARGET_FILE:name_generator> --version)


# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- invalid parameter usage
add_test(NAME name_generator_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:name_generator>)
