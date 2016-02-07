project(multi_dll)

include_directories("${CMAKE_SOURCE_DIR}/doctest/")

add_library(dll_1 SHARED dll.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} dll_1)

add_test(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
