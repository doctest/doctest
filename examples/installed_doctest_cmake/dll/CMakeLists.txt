cmake_minimum_required(VERSION 3.0)

project(example_dll VERSION 0.0.1 LANGUAGES CXX)

find_package(doctest REQUIRED)

add_library("dll" SHARED dll.cpp)
target_compile_features("dll" PRIVATE cxx_std_17)
target_compile_definitions("dll" PUBLIC -D_EXPORT)
target_link_libraries("dll" doctest::doctest)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} dll)