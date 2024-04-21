cmake_minimum_required (VERSION 3.8)

# ############################ #
# tests for bsa-cli executable #
# ############################ #

# add tests for --version and --help parameters

# default help parameter "--help"
add_test(NAME bsa_cli_help
         COMMAND $<TARGET_FILE:bsa-cli> --help)

# short help parameter with question mark "-?"
add_test(NAME bsa_cli_help_question_mark
         COMMAND $<TARGET_FILE:bsa-cli> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
    add_test(NAME bsa_cli_help_question_mark_windows
             COMMAND $<TARGET_FILE:bsa-cli> /?)
endif ()

# parameter to show version number
add_test(NAME bsa_cli_version
         COMMAND $<TARGET_FILE:bsa-cli> --version)

# tests for help of commands
add_test(NAME bsa_cli_help_check_hashes
         COMMAND $<TARGET_FILE:bsa-cli> help check-hashes)
add_test(NAME bsa_cli_help_commands
         COMMAND $<TARGET_FILE:bsa-cli> help commands)
add_test(NAME bsa_cli_help_directories
         COMMAND $<TARGET_FILE:bsa-cli> help directories)
add_test(NAME bsa_cli_help_directory_metadata
         COMMAND $<TARGET_FILE:bsa-cli> help directory-metadata)
add_test(NAME bsa_cli_help_extract_all
         COMMAND $<TARGET_FILE:bsa-cli> help extract-all)
add_test(NAME bsa_cli_help_extract_directory
         COMMAND $<TARGET_FILE:bsa-cli> help extract-directory)
add_test(NAME bsa_cli_help_extract_file
         COMMAND $<TARGET_FILE:bsa-cli> help extract-file)
add_test(NAME bsa_cli_help_file_metadata
         COMMAND $<TARGET_FILE:bsa-cli> help file-metadata)
add_test(NAME bsa_cli_help_help
         COMMAND $<TARGET_FILE:bsa-cli> help help)
add_test(NAME bsa_cli_help_info
         COMMAND $<TARGET_FILE:bsa-cli> help info)
add_test(NAME bsa_cli_help_list
         COMMAND $<TARGET_FILE:bsa-cli> help list)

# scripted test for invalid parameter use
if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

add_test(NAME bsa_cli_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:bsa-cli>)
