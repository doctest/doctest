project(hello_world)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
