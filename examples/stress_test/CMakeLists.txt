project(stress_test)

include_directories("../../doctest/")

if(MSVC)
	add_definitions(/bigobj)
endif()

add_executable(${PROJECT_NAME} main.cpp test.cpp)

add_test(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
