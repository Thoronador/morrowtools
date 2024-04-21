cmake_minimum_required (VERSION 3.8)

# ############################## #
# tests for conv_cams executable #
# ############################## #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME conv_cams_help
         COMMAND $<TARGET_FILE:conv_cams> --help)

# short help parameter with question mark "-?"
add_test(NAME conv_cams_help_question_mark
         COMMAND $<TARGET_FILE:conv_cams> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME conv_cams_help_question_mark_windows
             COMMAND $<TARGET_FILE:conv_cams> /?)
endif ()

# parameter to show version number
add_test(NAME conv_cams_version
         COMMAND $<TARGET_FILE:conv_cams> --version)


# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- request version via exit code
add_test(NAME conv_cams_version_from_exit_code
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/version-with-exitcode.${EXT} $<TARGET_FILE:conv_cams>)

# -- invalid parameter usage
add_test(NAME formID_finder_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:conv_cams>)
