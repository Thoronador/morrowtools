# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ################################# #
# tests for data_cleaner executable #
# ################################# #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME data_cleaner_help
         COMMAND $<TARGET_FILE:data_cleaner> --help)

# short help parameter with question mark "-?"
add_test(NAME data_cleaner_help_question_mark
         COMMAND $<TARGET_FILE:data_cleaner> -?)

# Windows-style help parameter "/?"
add_test(NAME data_cleaner_help_question_mark_windows
         COMMAND $<TARGET_FILE:data_cleaner> /?)

# parameter to show version number
add_test(NAME data_cleaner_version
         COMMAND $<TARGET_FILE:data_cleaner> --version)