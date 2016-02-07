project(alternative_header_inclusion)

include_directories("${CMAKE_SOURCE_DIR}/doctest/")

add_executable(${PROJECT_NAME} main.cpp file.cpp)

add_test(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
