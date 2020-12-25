# for more information about this example refer to the GitHub issue: https://github.com/onqtam/doctest/issues/436

add_library(default SHARED default.cpp)
target_link_libraries(default PUBLIC doctest)
set_target_properties(default PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_library(return42 SHARED return42.cpp)
target_link_libraries(return42 PUBLIC doctest)
set_target_properties(return42 PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_executable(same_tests_multiple_configurations main.cpp)
target_link_libraries(same_tests_multiple_configurations PUBLIC default return42 doctest)


# create the test runner, to which all other targets will link to
add_library(test_runner SHARED test_runner.cpp)
target_link_libraries(test_runner PUBLIC doctest)

# link all targets to the test runner
target_link_libraries(same_tests_multiple_configurations PUBLIC test_runner)
target_link_libraries(default PUBLIC test_runner)
target_link_libraries(return42 PUBLIC test_runner)

doctest_add_test(NAME same_tests_multiple_configurations COMMAND $<TARGET_FILE:same_tests_multiple_configurations> --no-version)
