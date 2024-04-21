cmake_minimum_required (VERSION 3.8)

# #################################### #
# tests for skill_rebalance executable #
# #################################### #

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME skill_rebalance_help
         COMMAND $<TARGET_FILE:skill_rebalance> --help)

# short help parameter with question mark "-?"
add_test(NAME skill_rebalance_help_question_mark
         COMMAND $<TARGET_FILE:skill_rebalance> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME skill_rebalance_help_question_mark_windows
             COMMAND $<TARGET_FILE:skill_rebalance> /?)
endif ()

# parameter to show version number
add_test(NAME skill_rebalance_version
         COMMAND $<TARGET_FILE:skill_rebalance> --version)

# scripted tests
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# -- invalid parameter usage
add_test(NAME skill_rebalance_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:skill_rebalance>)
