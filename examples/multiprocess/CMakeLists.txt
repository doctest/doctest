project(multiprocess)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(multiprocess ./run.py)
