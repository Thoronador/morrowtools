cmake_minimum_required (VERSION 3.8)

# ########################## #
# tests for bsafs executable #
# ########################## #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME bsafs_help
         COMMAND $<TARGET_FILE:bsafs> --help)

# short help parameter with question mark "-?"
add_test(NAME bsafs_help_question_mark
         COMMAND $<TARGET_FILE:bsafs> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME bsafs_help_question_mark_windows
             COMMAND $<TARGET_FILE:bsafs> /?)
endif ()

# parameter to show version number
add_test(NAME bsafs_version
         COMMAND $<TARGET_FILE:bsafs> --version)
