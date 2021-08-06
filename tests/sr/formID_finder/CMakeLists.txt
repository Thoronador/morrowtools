# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ################################## #
# tests for formID_finder executable #
# ################################## #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME formID_finder_help
         COMMAND $<TARGET_FILE:formID_finder> --help)

# short help parameter with question mark "-?"
add_test(NAME formID_finder_help_question_mark
         COMMAND $<TARGET_FILE:formID_finder> -?)

# Windows-style help parameter "/?"
add_test(NAME formID_finder_help_question_mark_windows
         COMMAND $<TARGET_FILE:formID_finder> /?)

# parameter to show version number
add_test(NAME formID_finder_version
         COMMAND $<TARGET_FILE:formID_finder> --version)
