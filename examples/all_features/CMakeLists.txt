get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(files
    main.cpp
    doctest_proxy.h
    coverage_maxout.cpp
    alternative_macros.cpp
    assertion_macros.cpp
    stringification.cpp
    subcases.cpp
)

# add the normal build
doctest_add_executable(${PROJECT_NAME} ${files})
target_compile_definitions(${PROJECT_NAME} PRIVATE DOCTEST_CONFIG_COLORS_NONE) # easy way to fix test coverage
target_link_libraries(${PROJECT_NAME} doctest)

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

# add variation of the build with disabled tests - should still compile
doctest_add_executable(disabled ${files})
target_link_libraries(disabled doctest)
if(MSVC)
    target_compile_options(disabled PRIVATE /wd4505) # unreferenced local function has been removed
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(disabled PRIVATE -Wno-unneeded-internal-declaration)
    target_compile_options(disabled PRIVATE -Wno-unused-variable) # TODO: think about fixing this in a different way!
endif()
target_compile_definitions(disabled PRIVATE DOCTEST_CONFIG_DISABLE)
doctest_add_test(NAME disabled COMMAND $<TARGET_FILE:disabled>)
