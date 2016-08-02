cmake_minimum_required(VERSION 2.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../../scripts/common.cmake)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp test.cpp)

# queries
add_test(NAME help              COMMAND $<TARGET_FILE:${PROJECT_NAME}> -h)
add_test(NAME version           COMMAND $<TARGET_FILE:${PROJECT_NAME}> -v)
add_test(NAME count             COMMAND $<TARGET_FILE:${PROJECT_NAME}> -c)
add_test(NAME list_test_cases   COMMAND $<TARGET_FILE:${PROJECT_NAME}> -ltc)
add_test(NAME list_test_suites  COMMAND $<TARGET_FILE:${PROJECT_NAME}> -lts)


add_test(NAME successful        COMMAND $<TARGET_FILE:${PROJECT_NAME}> -success=true -exit)
add_test(NAME abort_after       COMMAND $<TARGET_FILE:${PROJECT_NAME}> -aa=3)
add_test(NAME first_last        COMMAND $<TARGET_FILE:${PROJECT_NAME}> -f=2 -l=5)






