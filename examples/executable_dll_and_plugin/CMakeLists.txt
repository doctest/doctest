add_library(implementation SHARED implementation.cpp implementation_2.cpp)
target_link_libraries(implementation PUBLIC doctest)

add_library(dll SHARED dll.cpp)
target_link_libraries(dll implementation)

add_library(plugin SHARED plugin.cpp)
target_link_libraries(plugin implementation)

add_executable(executable_dll_and_plugin main.cpp)
target_link_libraries(executable_dll_and_plugin dll)
target_link_libraries(executable_dll_and_plugin implementation)

if(NOT WIN32)
    target_link_libraries(executable_dll_and_plugin dl)
endif()

# have the executable depend on the plugin so it gets built as well when building/starting only the executable
add_dependencies(executable_dll_and_plugin plugin)

# group them together in a single folder inside IDEs
set_target_properties(implementation PROPERTIES FOLDER executable_dll_and_plugin)
set_target_properties(dll PROPERTIES FOLDER executable_dll_and_plugin)
set_target_properties(plugin PROPERTIES FOLDER executable_dll_and_plugin)
set_target_properties(executable_dll_and_plugin PROPERTIES FOLDER executable_dll_and_plugin)

doctest_add_test(NAME executable_dll_and_plugin COMMAND $<TARGET_FILE:executable_dll_and_plugin> --no-version)
