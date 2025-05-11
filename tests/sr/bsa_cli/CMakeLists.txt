cmake_minimum_required (VERSION 3.8...3.31)

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

# tests for running commands that do not extract files or directories
add_test(NAME bsa_cli_run_check_hashes
         COMMAND $<TARGET_FILE:bsa-cli> check-hashes "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_check_hashes_wrong_hashes
         COMMAND $<TARGET_FILE:bsa-cli> check-hashes "${CMAKE_CURRENT_SOURCE_DIR}/test_wrong_hashes.bsa")
add_test(NAME bsa_cli_run_commands
         COMMAND $<TARGET_FILE:bsa-cli> commands)
add_test(NAME bsa_cli_run_directories
         COMMAND $<TARGET_FILE:bsa-cli> directories "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_directory_metadata
         COMMAND $<TARGET_FILE:bsa-cli> directory-metadata "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_file_metadata
         COMMAND $<TARGET_FILE:bsa-cli> file-metadata "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_file_metadata_with_total
         COMMAND $<TARGET_FILE:bsa-cli> file-metadata --show-total "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_info
         COMMAND $<TARGET_FILE:bsa-cli> info "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
add_test(NAME bsa_cli_run_list
         COMMAND $<TARGET_FILE:bsa-cli> list "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")

if (NOT WIN32)
  set(EXT "sh")
else ()
  set(EXT "cmd")
endif()

# scripted test for invalid parameter use
add_test(NAME bsa_cli_parameter_misuse
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/parameter-misuse.${EXT} $<TARGET_FILE:bsa-cli> "${CMAKE_CURRENT_SOURCE_DIR}/test_v104_uncompressed.bsa")
# scripted test for umlauts in BSA file name
add_test(NAME bsa_cli_parameter_umlauts
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/umlauts.${EXT} $<TARGET_FILE:bsa-cli> "${CMAKE_CURRENT_SOURCE_DIR}/Möwe.bsa")
