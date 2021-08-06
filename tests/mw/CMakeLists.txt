# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

# binary executable tests
add_subdirectory (cell_translator)
add_subdirectory (data_cleaner)
add_subdirectory (skill_rebalance)
add_subdirectory (spell_rename)
