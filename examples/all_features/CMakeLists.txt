################################################################################
## BUILD ALL EXAMPLE SOURCES INTO A SINGLE BINARY AND EXECUTE TESTS ON EACH FILE
################################################################################

set(files_with_output
    main.cpp
    doctest_proxy.h
    header.h
    alternative_macros.cpp
    assertion_macros.cpp
    stringification.cpp
    reporters_and_listeners.cpp
    subcases.cpp
    logging.cpp
    templated_test_cases.cpp
    test_cases_and_suites.cpp
    asserts_used_outside_of_tests.cpp
)

set(files_all
    ${files_with_output}
    concurrency.cpp
    ../../scripts/coverage_maxout.cpp
)

# add the executable
add_executable(all_features ${files_all})
target_link_libraries(all_features doctest ${CMAKE_THREAD_LIBS_INIT})

# easy way to fix test coverage - disable colors and crash handling
target_compile_definitions(all_features PRIVATE
    DOCTEST_CONFIG_COLORS_NONE
    DOCTEST_CONFIG_NO_POSIX_SIGNALS
    DOCTEST_CONFIG_NO_WINDOWS_SEH)

# omit the version and the num test cases skipped from the summary - this way the output will change less often
set(common_args COMMAND $<TARGET_FILE:all_features> --no-skipped-summary --no-version)

# add per-file tests
foreach(f ${files_with_output})
    doctest_add_test(NAME ${f} ${common_args} -sf=*${f})
endforeach()

# add this separately since it shouldn't have output compared to reference output - due to concurrency
# not adding it for MinGW since it crashes when using mingw-w64-x86_64-8.1.0-release-posix-seh-rt_v6-rev0
# (also disabled for old XCode builds on travis where there is no thread_local support and this is defined in the build matrix)
if(NOT MINGW AND NOT DEFINED DOCTEST_THREAD_LOCAL)
    doctest_add_test(NO_OUTPUT NAME concurrency.cpp ${common_args} -sf=*concurrency.cpp -d) # duration: there is no output anyway
endif()

# add this separately since the file has a non-straightforward path
doctest_add_test(NAME coverage_maxout.cpp ${common_args} -sf=*coverage_maxout.cpp)

# queries
doctest_add_test(NAME version           COMMAND $<TARGET_FILE:all_features> -v)
doctest_add_test(NAME help              ${common_args} -h)
doctest_add_test(NO_OUTPUT NAME outfile ${common_args} -c   -out=temp) # just to exercise the output option
doctest_add_test(NAME count             ${common_args} -c   -sf=*coverage*)
doctest_add_test(NAME list_test_cases   ${common_args} -ltc -sf=*coverage*)
doctest_add_test(NAME list_test_suites  ${common_args} -lts -sf=*coverage*)
doctest_add_test(NAME list_reporters    ${common_args} -lr  -sf=*coverage*)

# options
doctest_add_test(NAME all_binary  ${common_args} -tc=all?binary* -s) # print all binary asserts - for getAssertString()
doctest_add_test(NAME abort_after ${common_args} -aa=2 -e=off   -sf=*coverage*) # abort after 2 assert fails and parse a negative
doctest_add_test(NAME first_last  ${common_args} -f=2 -l=4      -sf=*coverage*) # run a range
doctest_add_test(NAME filter_1    ${common_args} -ts=none) # should filter out all
doctest_add_test(NAME filter_2    COMMAND $<TARGET_FILE:all_features>   -tse=* -nv) # should filter out all + print skipped
doctest_add_test(NAME filter_3    ${common_args} -sc=from*,sc* -sce=sc2 -sf=*subcases*) # enter a specific subcase - sc1
doctest_add_test(NAME order_1     ${common_args} -ob=suite -ns          -sf=*test_cases_and_suites*)
doctest_add_test(NAME order_2     ${common_args} -ob=name               -sf=*test_cases_and_suites*)
doctest_add_test(NAME order_3     ${common_args} -ob=rand               -sfe=*) # exclude everything for no output

################################################################################
## VARIATION OF THE BUILD WITH DOCTEST DISABLED - SHOULD STILL COMPILE
################################################################################

if(DEFINED ENV{CODE_COVERAGE})
    return() # do not continue with the disabled example
endif()

add_executable(disabled ${files_all})
target_compile_definitions(disabled PRIVATE DOCTEST_CONFIG_DISABLE)
target_link_libraries(disabled doctest ${CMAKE_THREAD_LIBS_INIT})

doctest_add_test(NAME disabled COMMAND $<TARGET_FILE:disabled>)

# TODO: think about fixing these in a different way! - see issue #61 or commit 6b61e8aa3818c5ea100cedc1bb48a60ea10df6e8
if(MSVC)
    target_compile_options(disabled PRIVATE /wd4505) # unreferenced local function has been removed
    target_compile_options(disabled PRIVATE /wd4100) # unreferenced formal parameter
    target_compile_options(disabled PRIVATE /wd4189) # local variable is initialized but not referenced
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(disabled PRIVATE -Wno-unknown-warning-option)
    target_compile_options(disabled PRIVATE -Wno-unneeded-internal-declaration)
    target_compile_options(disabled PRIVATE -Wno-unused-function)
    target_compile_options(disabled PRIVATE -Wno-unused-parameter)
    target_compile_options(disabled PRIVATE -Wno-unused-variable)
    target_compile_options(disabled PRIVATE -Wno-unused-template)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(disabled PRIVATE -Wno-unused-function)
    target_compile_options(disabled PRIVATE -Wno-unused-parameter)
    target_compile_options(disabled PRIVATE -Wno-unused-variable)
endif()
