# We might support earlier versions, too, but try to use a more recent one.
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
add_test(NAME skill_rebalance_help_question_mark_windows
         COMMAND $<TARGET_FILE:skill_rebalance> /?)

# parameter to show version number
add_test(NAME skill_rebalance_version
         COMMAND $<TARGET_FILE:skill_rebalance> --version)
