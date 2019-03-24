cmake_minimum_required(VERSION 3.0)

project(example_exe VERSION 0.0.1 LANGUAGES CXX)

find_package(doctest REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} doctest::doctest)