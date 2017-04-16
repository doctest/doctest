get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(files
    main.cpp
    coverage_maxout.cpp
    alternative_macros.cpp
    assertion_macros.cpp
    stringification.cpp
    subcases.cpp
)

doctest_add_executable(${PROJECT_NAME} ${files})
target_link_libraries(${PROJECT_NAME} doctest)

# add variation of the build with disabled tests - should still compile
doctest_add_executable(${PROJECT_NAME}_disabled ${files})
target_compile_definitions(trie_io_test PRIVATE DOCTEST_CONFIG_DISABLE)
doctest_add_test(NAME ${PROJECT_NAME}.disabled COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

# add per-file tests
foreach(f ${files})
    doctest_add_test(NAME ${PROJECT_NAME}.${f} COMMAND $<TARGET_FILE:${PROJECT_NAME}> --source-file=*${f})
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
