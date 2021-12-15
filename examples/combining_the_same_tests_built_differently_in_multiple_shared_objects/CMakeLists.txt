# for more information about this example refer to the GitHub issue: https://github.com/doctest/doctest/issues/436

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

# the output on a GitHub Actions Windows build with cl.exe when printing __FILE__ in the fileOrderComparator is the following:
#
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# d:\a\doctest\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# d:\a\doctest\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# ..\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# d:\a\doctest\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
#
# and the output from cl.exe on AppVeyor or on my local Windows setup is the following:
#
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\test_runner.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# C:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\main.cpp
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
# c:\projects\doctest\examples\combining_the_same_tests_built_differently_in_multiple_shared_objects\foo.h
#
# as you can see on GitHub Actions we get a full path for the header and a relative for the sources and
# regardless of case sensitivity (for which there is a difference in fileOrderComparator) we will always
# get a different sorting of the test cases and thus we need NO_OUTPUT on this test so that the CI passes
#
# otherwise the output from test_runner.cpp and main.cpp gets reordered before/after the output from the header
# TODO: maybe we should simply remove the output from those 2 .cpp files? then there won't be a problem...
#
doctest_add_test(NO_OUTPUT NAME same_tests_multiple_configurations COMMAND $<TARGET_FILE:same_tests_multiple_configurations> --no-version)
