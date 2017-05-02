get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

################################################################################
## BUILD ALL EXAMPLE SOURCES INTO A SINGLE BINARY AND EXECUTE TESTS ON EACH FILE
################################################################################

set(files
    main.cpp
    doctest_proxy.h
    header.h
    coverage_maxout.cpp
    alternative_macros.cpp
    assertion_macros.cpp
    stringification.cpp
    subcases.cpp
    logging.cpp
    templated_test_cases.cpp
    test_cases_and_suites.cpp
)

doctest_add_executable(${PROJECT_NAME} ${files})
# easy way to fix test coverage - disable colors and signals
target_compile_definitions(${PROJECT_NAME} PRIVATE DOCTEST_CONFIG_COLORS_NONE DOCTEST_CONFIG_NO_POSIX_SIGNALS)

set(common_test_args COMMAND $<TARGET_FILE:${PROJECT_NAME}> --no-skipped-summary --no-version)

# add per-file tests
foreach(f ${files})
    doctest_add_test(NAME ${f} ${common_test_args} -sf=*${f})
endforeach()

set(common_cov_args ${common_test_args} -sf=*coverage*)

# queries
doctest_add_test(NAME version COMMAND $<TARGET_FILE:${PROJECT_NAME}> -v)
doctest_add_test(NAME help               ${common_cov_args} -h)
doctest_add_test(NAME count              ${common_cov_args} -c)
doctest_add_test(NAME list_test_cases    ${common_cov_args} -ltc)
doctest_add_test(NAME list_test_suites   ${common_cov_args} -lts)

# get all the binary asserts to be printed - to exercise all cases in getAssertString()
doctest_add_test(NAME all_binary_success ${common_test_args} -tc=all?binary* -s)

# options
doctest_add_test(NAME abort_after        ${common_cov_args} -aa=2 -e=off)    # abort after 2 failed asserts (and parse negative)
doctest_add_test(NAME first_last         ${common_cov_args} -f=2 -l=4)       # run a range
doctest_add_test(NAME filter_1           ${common_cov_args} -ts=nonexistent) # should filter out all
doctest_add_test(NAME filter_2           ${common_cov_args} -tse=*)          # should filter out all
doctest_add_test(NAME order_1            ${common_test_args} -ob=suite -sf=*test_cases_and_suites*)
doctest_add_test(NAME order_2            ${common_test_args} -ob=name  -sf=*test_cases_and_suites*)
doctest_add_test(NAME order_3            ${common_test_args} -ob=rand  -sfe=*) # exclude everything for no output

################################################################################
## VARIATION OF THE BUILD WITH DOCTEST DISABLED - SHOULD STILL COMPILE
################################################################################

return() # temp hack

doctest_add_executable(disabled ${files})
target_compile_definitions(disabled PRIVATE DOCTEST_CONFIG_DISABLE)

doctest_add_test(NAME disabled COMMAND $<TARGET_FILE:disabled>)

# TODO: think about fixing these in a different way! - see issue #61 or commit 6b61e8aa3818c5ea100cedc1bb48a60ea10df6e8
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(disabled PRIVATE /wd4505) # unreferenced local function has been removed
    target_compile_options(disabled PRIVATE /wd4100) # unreferenced formal parameter
    target_compile_options(disabled PRIVATE /wd4189) # local variable is initialized but not referenced
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(disabled PRIVATE -Wno-unknown-warning-option)
    target_compile_options(disabled PRIVATE -Wno-unneeded-internal-declaration)
    target_compile_options(disabled PRIVATE -Wno-unused-parameter)
    target_compile_options(disabled PRIVATE -Wno-unused-variable)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(disabled PRIVATE -Wno-unused-function)
    target_compile_options(disabled PRIVATE -Wno-unused-parameter)
    target_compile_options(disabled PRIVATE -Wno-unused-variable)
endif()
