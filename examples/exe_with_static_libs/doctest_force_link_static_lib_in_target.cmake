if(doctest_force_link_static_lib_in_target_included)
    return()
endif()
set(doctest_force_link_static_lib_in_target_included true)

cmake_minimum_required(VERSION 3.0)

# includes the file to the source with compiler flags
function(doctest_include_file_in_sources header sources)
    foreach(src ${sources})
        if(${src} MATCHES \\.\(cc|cp|cpp|CPP|c\\+\\+|cxx\)$)
            # get old flags
            get_source_file_property(old_compile_flags ${src} COMPILE_FLAGS)
            if(old_compile_flags STREQUAL "NOTFOUND")
                set(old_compile_flags "")
            endif()

            # update flags
            if(MSVC)
                set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS
                    "${old_compile_flags} /FI\"${header}\"")
            else()
                set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS
                    "${old_compile_flags} -include \"${header}\"")
            endif()
        endif()
    endforeach()
endfunction()

# this is the magic function - forces every object file from the library to be linked into the target (dll or executable)
# it doesn't work in 2 scenarios:
# - either the target or the library uses a precompiled header - see the end of this issue for details: https://github.com/onqtam/doctest/issues/21
# - either the target or the library is an imported target (pre-built) and not built within the current cmake tree
# Alternatives:
# - use CMake object libraries instead of static libraries - >> THIS IS ACTUALLY PREFERRED << to all this CMake trickery
# - checkout these 2 repositories:
#   - https://github.com/pthom/cmake_registertest
#   - https://github.com/pthom/doctest_registerlibrary
function(doctest_force_link_static_lib_in_target target lib)
    # check if the library has generated dummy headers
    get_target_property(DDH ${lib} DOCTEST_DUMMY_HEADER)
    get_target_property(LIB_NAME ${lib} NAME)
    if(${DDH} STREQUAL "DDH-NOTFOUND")
        # figure out the paths and names of the dummy headers - should be in the build folder for the target
        set(BD ${CMAKE_CURRENT_BINARY_DIR})
        if(NOT CMAKE_VERSION VERSION_LESS 3.4)
            get_target_property(BD ${lib} BINARY_DIR) # 'BINARY_DIR' target property unsupported before CMake 3.4 ...
        endif()
        set(dummy_dir ${BD}/${LIB_NAME}_DOCTEST_STATIC_LIB_FORCE_LINK_DUMMIES/)
        set(dummy_header ${dummy_dir}/all_dummies.h)
        file(MAKE_DIRECTORY ${dummy_dir})

        # create a dummy header for each source file, include a dummy function in it and include it in the source file
        set(curr_dummy "0")
        set(DLL_PRIVATE "#ifndef _WIN32\n#define DLL_PRIVATE __attribute__ ((visibility (\"hidden\")))\n#else\n#define DLL_PRIVATE\n#endif\n\n")
        get_target_property(lib_sources ${lib} SOURCES)
        foreach(src ${lib_sources})
            if(${src} MATCHES \\.\(cc|cp|cpp|CPP|c\\+\\+|cxx\)$)
                math(EXPR curr_dummy "${curr_dummy} + 1")

                set(curr_dummy_header ${dummy_dir}/dummy_${curr_dummy}.h)
                file(WRITE ${curr_dummy_header} "${DLL_PRIVATE}namespace doctest { namespace detail { DLL_PRIVATE int dummy_for_${LIB_NAME}_${curr_dummy}(); DLL_PRIVATE int dummy_for_${LIB_NAME}_${curr_dummy}() { return ${curr_dummy}; } } }\n")
                doctest_include_file_in_sources(${curr_dummy_header} ${src})
            endif()
        endforeach()
        set(total_dummies ${curr_dummy})

        # create the master dummy header
        file(WRITE ${dummy_header} "${DLL_PRIVATE}namespace doctest { namespace detail {\n\n")

        # forward declare the dummy functions in the master dummy header
        foreach(curr_dummy RANGE 1 ${total_dummies})
            file(APPEND ${dummy_header} "DLL_PRIVATE int dummy_for_${LIB_NAME}_${curr_dummy}();\n")
        endforeach()

        # call the dummy functions in the master dummy header
        file(APPEND ${dummy_header} "\nDLL_PRIVATE int dummies_for_${LIB_NAME}();\nDLL_PRIVATE int dummies_for_${LIB_NAME}() {\n    int res = 0;\n")
        foreach(curr_dummy RANGE 1 ${total_dummies})
            file(APPEND ${dummy_header} "    res += dummy_for_${LIB_NAME}_${curr_dummy}();\n")
        endforeach()
        file(APPEND ${dummy_header} "    return res;\n}\n\n} } // namespaces\n")

        # set the dummy header property so we don't recreate the dummy headers the next time this macro is called for this library
        set_target_properties(${lib} PROPERTIES DOCTEST_DUMMY_HEADER ${dummy_header})
        set(DDH ${dummy_header})
    endif()

    get_target_property(DFLLTD ${target} DOCTEST_FORCE_LINKED_LIBRARIES_THROUGH_DUMMIES)
    get_target_property(target_sources ${target} SOURCES)

    if("${DFLLTD}" STREQUAL "DFLLTD-NOTFOUND")
        # if no library has been force linked to this target
        foreach(src ${target_sources})
            if(${src} MATCHES \\.\(cc|cp|cpp|CPP|c\\+\\+|cxx\)$)
                doctest_include_file_in_sources(${DDH} ${src})
                break()
            endif()
        endforeach()

        # add the library as force linked to this target
        set_target_properties(${target} PROPERTIES DOCTEST_FORCE_LINKED_LIBRARIES_THROUGH_DUMMIES ${LIB_NAME})
    else()
        # if this particular library hasn't been force linked to this target
        list(FIND DFLLTD ${LIB_NAME} lib_forced_in_target)
        if(${lib_forced_in_target} EQUAL -1)
            foreach(src ${target_sources})
                if(${src} MATCHES \\.\(cc|cp|cpp|CPP|c\\+\\+|cxx\)$)
                    doctest_include_file_in_sources(${DDH} ${src})
                    break()
                endif()
            endforeach()

            # add this library to the list of force linked libraries for this target
            list(APPEND DFLLTD ${LIB_NAME})
            set_target_properties(${target} PROPERTIES DOCTEST_FORCE_LINKED_LIBRARIES_THROUGH_DUMMIES "${DFLLTD}")
        else()
            message(AUTHOR_WARNING "LIBRARY \"${lib}\" ALREADY FORCE-LINKED TO TARGET \"${target}\"!")
        endif()
    endif()
endfunction()

# a utility function to create an executable for a static library with tests - as requested by https://github.com/pthom
function(doctest_make_exe_for_static_lib exe_name lib_name)
    set(exe_dir ${CMAKE_CURRENT_BINARY_DIR}/${exe_name}_generated_sources)
    file(MAKE_DIRECTORY ${exe_dir})
    file(WRITE ${exe_dir}/main.cpp "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n#include \"doctest.h\"\n")
    add_executable(${exe_name} ${exe_dir}/main.cpp)
    target_link_libraries(${exe_name} ${lib_name})
    doctest_force_link_static_lib_in_target(${exe_name} ${lib_name})
    add_test(NAME ${exe_name} COMMAND $<TARGET_FILE:${exe_name}>)
endfunction()
