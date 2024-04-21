cmake_minimum_required (VERSION 3.8)

# #################################### #
# tests for cell_translator executable #
# #################################### #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME cell_translator_help
         COMMAND $<TARGET_FILE:cell_translator> --help)

# short help parameter with question mark "-?"
add_test(NAME cell_translator_help_question_mark
         COMMAND $<TARGET_FILE:cell_translator> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME cell_translator_help_question_mark_windows
             COMMAND $<TARGET_FILE:cell_translator> /?)
endif ()

# parameter to show version number
add_test(NAME cell_translator_version
         COMMAND $<TARGET_FILE:cell_translator> --version)

# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- request version via exit code
add_test(NAME cell_translator_version_from_exit_code
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/version-with-exitcode.${EXT} $<TARGET_FILE:cell_translator>)

# -- invalid parameter usage
add_test(NAME cell_translator_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:cell_translator>)
