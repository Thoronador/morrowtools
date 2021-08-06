# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# ############################## #
# tests for conv_cams executable #
# ############################## #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME conv_cams_help
         COMMAND $<TARGET_FILE:conv_cams> --help)

# short help parameter with question mark "-?"
add_test(NAME comv_cams_help_question_mark
         COMMAND $<TARGET_FILE:conv_cams> -?)

# Windows-style help parameter "/?"
add_test(NAME conv_cams_help_question_mark_windows
         COMMAND $<TARGET_FILE:conv_cams> /?)

# parameter to show version number
add_test(NAME comv_cams_version
         COMMAND $<TARGET_FILE:conv_cams> --version)
