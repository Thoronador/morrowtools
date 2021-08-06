# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ################################# #
# tests for spell_rename executable #
# ################################# #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME spell_rename_help
         COMMAND $<TARGET_FILE:spell_rename> --help)

# short help parameter with question mark "-?"
add_test(NAME spell_rename_help_question_mark
         COMMAND $<TARGET_FILE:spell_rename> -?)

# Windows-style help parameter "/?"
add_test(NAME spell_rename_help_question_mark_windows
         COMMAND $<TARGET_FILE:spell_rename> /?)

# parameter to show version number
add_test(NAME spell_rename_version
         COMMAND $<TARGET_FILE:spell_rename> --version)
