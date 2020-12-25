# for more information about this example refer to the GitHub issue: https://github.com/onqtam/doctest/issues/436

# create the test runner, to which all other targets will link to
add_library(test_runner SHARED test_runner.cpp)
target_link_libraries(test_runner PUBLIC doctest)

add_library(default SHARED default.cpp)
target_link_libraries(default PUBLIC doctest test_runner)
set_target_properties(default PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_library(return42 SHARED return42.cpp)
target_link_libraries(return42 PUBLIC doctest test_runner)
set_target_properties(return42 PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_executable(same_tests_multiple_configurations main.cpp)
target_link_libraries(same_tests_multiple_configurations PUBLIC default return42 doctest test_runner)

doctest_add_test(NAME same_tests_multiple_configurations COMMAND $<TARGET_FILE:same_tests_multiple_configurations> --no-version)
