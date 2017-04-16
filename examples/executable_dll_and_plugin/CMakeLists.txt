get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    if(DEFINED ENV{TRAVIS})
        # force 64 bit for OSX because -m32 fails >>> SOMETIMES <<< (x86_64 vs 386 - like the dll is not linked with -m32...)
        add_compiler_flags(-m64)
    endif()
endif()

doctest_add_library(implementation SHARED implementation.cpp implementation_2.cpp)
target_link_libraries(implementation doctest)

doctest_add_library(dll SHARED dll.cpp)
target_link_libraries(dll doctest)
target_link_libraries(dll implementation)

doctest_add_library(plugin SHARED plugin.cpp)
target_link_libraries(plugin doctest)
target_link_libraries(plugin implementation)

doctest_add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} doctest)
target_link_libraries(${PROJECT_NAME} dll)
target_link_libraries(${PROJECT_NAME} implementation)

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} dl)
endif()

# have the executable depend on the plugin so it gets built as well when building/starting only the executable
add_dependencies(${PROJECT_NAME} plugin)

doctest_add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
