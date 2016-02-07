project(dev_testing)

add_definitions(-DDOCTEST_CONFIG_SHORT_MACRO_NAMES)

include_directories("${CMAKE_SOURCE_DIR}/doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
