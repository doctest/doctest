project(multiprocess)

include_directories("../../doctest/")

add_executable(${PROJECT_NAME} main.cpp)

add_test(${PROJECT_NAME} python ${CMAKE_CURRENT_SOURCE_DIR}/run.py)
