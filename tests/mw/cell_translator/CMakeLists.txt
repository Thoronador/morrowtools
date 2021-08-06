# We might support earlier versions, too, but try to use a more recent one.
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
add_test(NAME cell_translator_help_question_mark_windows
         COMMAND $<TARGET_FILE:cell_translator> /?)

# parameter to show version number
add_test(NAME cell_translator_version
         COMMAND $<TARGET_FILE:cell_translator> --version)
