cmake_minimum_required(VERSION 2.8)

project(dev_testing)

# FOR C:
# -Wstrict-prototypes
# -Wmissing-prototypes
# -Wdeclaration-after-statement
# -Wbad-function-cast
# -Wnested-externs
# -Wold-style-definition

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_CXX_FLAGS "--save-temps -std=c++98 -pedantic -m64 -O2 -ansi -fvisibility=hidden -fstrict-aliasing -Wall -Wextra -Wconversion -Wno-missing-field-initializers -Wold-style-cast -Wfloat-equal -Wundef -Wredundant-decls -Wlogical-op -Wshadow -Wstrict-overflow=4 -Wpointer-arith -Wcast-qual -Wformat=2 -Wswitch-default -Wmissing-include-dirs -Wcast-align ${CMAKE_CXX_FLAGS}")
endif()

add_definitions(-DDOCTEST_SHORT_MACRO_NAMES)

include_directories("../../")

add_executable(exe main.cpp test.cpp)