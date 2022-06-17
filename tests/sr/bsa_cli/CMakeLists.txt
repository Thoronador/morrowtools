# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ############################ #
# tests for bsa-cli executable #
# ############################ #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME bsa_cli_help
         COMMAND $<TARGET_FILE:bsa-cli> --help)

# short help parameter with question mark "-?"
add_test(NAME bsa_cli_help_question_mark
         COMMAND $<TARGET_FILE:bsa-cli> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME bsa_cli_help_question_mark_windows
             COMMAND $<TARGET_FILE:bsa-cli> /?)
endif ()

# parameter to show version number
add_test(NAME bsa_cli_version
         COMMAND $<TARGET_FILE:bsa-cli> --version)
