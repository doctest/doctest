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
    test_suites.cpp
)

doctest_add_executable(${PROJECT_NAME} ${files})
target_compile_definitions(${PROJECT_NAME} PRIVATE DOCTEST_CONFIG_COLORS_NONE) # easy way to fix test coverage

# add per-file tests
foreach(f ${files})
    doctest_add_test(NAME ${f} COMMAND $<TARGET_FILE:${PROJECT_NAME}> --dt-no-skipped-summary=1 --source-file=*${f})
endforeach()

## queries
#doctest_add_test(NAME help              COMMAND $<TARGET_FILE:${PROJECT_NAME}> -dt-h)
#doctest_add_test(NAME version           COMMAND $<TARGET_FILE:${PROJECT_NAME}> -v)
#doctest_add_test(NAME count             COMMAND $<TARGET_FILE:${PROJECT_NAME}> -c)
#doctest_add_test(NAME list_test_cases   COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ltc)
#doctest_add_test(NAME list_test_suites  COMMAND $<TARGET_FILE:${PROJECT_NAME}> -lts)
#
## options
#doctest_add_test(NAME successful        COMMAND $<TARGET_FILE:${PROJECT_NAME}> -s=true -e=Off -sfe=*main*) # exclude main because of RawMemoryToStr
#doctest_add_test(NAME abort_after       COMMAND $<TARGET_FILE:${PROJECT_NAME}> -aa=3 -no-colors)
#doctest_add_test(NAME first_last        COMMAND $<TARGET_FILE:${PROJECT_NAME}> -f=2 -l=5)
#doctest_add_test(NAME filter_1          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -tc=assert*********)
#doctest_add_test(NAME filter_2          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -tce=???ert*)
#doctest_add_test(NAME filter_3          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -sf=asgghdgsa)
#doctest_add_test(NAME filter_4          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -sfe=*test.cpp*)
#doctest_add_test(NAME filter_5          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ts=ts1)
#doctest_add_test(NAME filter_6          COMMAND $<TARGET_FILE:${PROJECT_NAME}> -tse=ts1)
#doctest_add_test(NAME order_1           COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ob=suite)
#doctest_add_test(NAME order_2           COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ob=name)
#doctest_add_test(NAME order_3           COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ob=rand -rs=324 -sfe=*) # sfe=* to exclude all tests for no output

################################################################################
## VARIATION OF THE BUILD WITH DOCTEST DISABLED - SHOULD STILL COMPILE
################################################################################

doctest_add_executable(disabled ${files})
target_compile_definitions(disabled PRIVATE DOCTEST_CONFIG_DISABLE)

doctest_add_test(NAME disabled COMMAND $<TARGET_FILE:disabled>)

# TODO: think about fixing these in a different way! - see issue #61 or commit 6b61e8aa3818c5ea100cedc1bb48a60ea10df6e8
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(disabled PRIVATE /wd4505) # unreferenced local function has been removed
    target_compile_options(disabled PRIVATE /wd4100) # unreferenced formal parameter
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(disabled PRIVATE -Wno-unknown-warning-option)
    target_compile_options(disabled PRIVATE -Wno-unneeded-internal-declaration)
    target_compile_options(disabled PRIVATE -Wno-unused-variable)
    target_compile_options(disabled PRIVATE -Wno-unused-local-typedef)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(disabled PRIVATE -Wno-unused-local-typedefs)
    target_compile_options(disabled PRIVATE -Wno-unused-parameter)
    target_compile_options(disabled PRIVATE -Wno-unused-function)
endif()
