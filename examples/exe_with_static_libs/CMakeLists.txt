# create object libraries instead of static libraries
add_library(lib_1 OBJECT lib_1_src1.cpp lib_1_src2.cpp)
add_library(lib_2 OBJECT lib_2_src.cpp)
add_executable(exe_with_static_libs main.cpp $<TARGET_OBJECTS:lib_1> $<TARGET_OBJECTS:lib_2>)
target_link_libraries(exe_with_static_libs doctest)

# object libraries cannot "link" to any target so this is how we get the INTERFACE include directories of the doctest target
get_property(doctest_include_dir TARGET doctest PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(lib_1 PRIVATE ${doctest_include_dir})
target_include_directories(lib_2 PRIVATE ${doctest_include_dir})

# alternatively we could create static libraries and use "doctest_force_link_static_lib_in_target"
#add_library(lib_1 STATIC lib_1_src1.cpp lib_1_src2.cpp)
#add_library(lib_2 STATIC lib_2_src.cpp)
#add_executable(exe_with_static_libs main.cpp)
#target_link_libraries(exe_with_static_libs lib_1)
#target_link_libraries(exe_with_static_libs lib_2)
#include(doctest_force_link_static_lib_in_target.cmake)
#doctest_force_link_static_lib_in_target(exe_with_static_libs lib_1)
#doctest_force_link_static_lib_in_target(exe_with_static_libs lib_2)

# group them together in a single folder inside IDEs
set_target_properties(lib_1 PROPERTIES FOLDER exe_with_static_libs)
set_target_properties(lib_2 PROPERTIES FOLDER exe_with_static_libs)
set_target_properties(exe_with_static_libs PROPERTIES FOLDER exe_with_static_libs)

doctest_add_test(NAME exe_with_static_libs COMMAND $<TARGET_FILE:exe_with_static_libs> --no-version)
