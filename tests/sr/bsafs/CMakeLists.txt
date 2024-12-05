cmake_minimum_required (VERSION 3.8...3.31)

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

# scripted test for invalid parameter use
add_test(NAME bsafs_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.sh $<TARGET_FILE:bsafs>)

# scripted test for mounting a BSA file - but not on GitLab
# (GitLab containers may not have the FUSE kernel module, so we cannot perform
#  a mount operation using libfuse there.)
if (NOT DEFINED ENV{GITLAB_CI})
  add_test(NAME bsafs_file_mount
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mount-test.sh $<TARGET_FILE:bsafs> ${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa)
endif ()
