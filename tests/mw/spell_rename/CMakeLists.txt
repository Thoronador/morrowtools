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
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME spell_rename_help_question_mark_windows
             COMMAND $<TARGET_FILE:spell_rename> /?)
endif ()

# parameter to show version number
add_test(NAME spell_rename_version
         COMMAND $<TARGET_FILE:spell_rename> --version)

# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- request version via exit code
add_test(NAME spell_rename_version_from_exit_code
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/version-with-exitcode.${EXT} $<TARGET_FILE:spell_rename>)

# -- invalid parameter usage
add_test(NAME spell_rename_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:spell_rename>)
