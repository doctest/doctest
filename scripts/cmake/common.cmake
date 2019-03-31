include(CMakeParseArguments)

# cache this for use inside of the function
set(CURRENT_LIST_DIR_CACHED ${CMAKE_CURRENT_LIST_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

find_package(Threads)

set(DOCTEST_TEST_MODE "COMPARE" CACHE STRING "Test mode - normal/run through valgrind/collect output/compare with output")
set_property(CACHE DOCTEST_TEST_MODE PROPERTY STRINGS "NORMAL;VALGRIND;COLLECT;COMPARE")

function(doctest_add_test_impl)
    cmake_parse_arguments(ARG "NO_VALGRIND;NO_OUTPUT;XML_OUTPUT" "NAME" "COMMAND" ${ARGN})
    if(NOT "${ARG_UNPARSED_ARGUMENTS}" STREQUAL "" OR "${ARG_NAME}" STREQUAL "" OR "${ARG_COMMAND}" STREQUAL "")
        message(FATAL_ERROR "doctest_add_test() called with wrong options!")
    endif()
    
    set(the_test_mode NORMAL)
    
    # construct the command that will be called by the exec_test.cmake script
    set(the_command "")
    if(${DOCTEST_TEST_MODE} STREQUAL "VALGRIND" AND NOT ARG_NO_VALGRIND)
        set(the_test_mode VALGRIND)
        set(the_command "valgrind -v --leak-check=full --track-origins=yes --error-exitcode=1")
    endif()
    foreach(cur ${ARG_COMMAND})
        set(the_command "${the_command} ${cur}")
    endforeach()
    if(ARG_XML_OUTPUT)
        set(the_command "${the_command} --reporters=xml")
        set(ARG_NAME ${ARG_NAME}_xml)
    endif()
    # append the argument for removing paths from filenames in the output so tests give the same output everywhere
    set(the_command "${the_command} --dt-no-path-filenames=1")
    # append the argument for substituting source line numbers with 0 in the output so tests give the same output when lines change a bit
    set(the_command "${the_command} --dt-no-line-numbers=1")
    # append the argument for ignoring the exit code of the test programs because some are intended to have failing tests
    set(the_command "${the_command} --dt-no-exitcode=1")
    # append the argument for using the same line format in the output - so gcc/non-gcc builds have the same output
    set(the_command "${the_command} --dt-gnu-file-line=0")
    
    string(STRIP ${the_command} the_command)
    
    if(${DOCTEST_TEST_MODE} STREQUAL "COLLECT" OR ${DOCTEST_TEST_MODE} STREQUAL "COMPARE")
        if(NOT ARG_NO_OUTPUT)
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_output/)
            set(the_test_mode ${DOCTEST_TEST_MODE})
            list(APPEND ADDITIONAL_FLAGS -DTEST_OUTPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/test_output/${ARG_NAME}.txt)
            list(APPEND ADDITIONAL_FLAGS -DTEST_TEMP_FILE=${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/temp_test_output_${ARG_NAME}.txt)
        endif()
    endif()
    
    list(APPEND ADDITIONAL_FLAGS -DTEST_MODE=${the_test_mode})
    
    add_test(NAME ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -DCOMMAND=${the_command} ${ADDITIONAL_FLAGS} -P ${CURRENT_LIST_DIR_CACHED}/exec_test.cmake)
endfunction()

# a custom version of add_test() to suite my needs
function(doctest_add_test)
    doctest_add_test_impl(${ARGN})
    doctest_add_test_impl(${ARGN} XML_OUTPUT)
endfunction()

macro(add_compiler_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compiler_flags(-Werror)
    add_compiler_flags(-pedantic)
    add_compiler_flags(-pedantic-errors)
    add_compiler_flags(-fvisibility=hidden)
    add_compiler_flags(-fstrict-aliasing)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    #add_compiler_flags(-Wno-unknown-pragmas)
    add_compiler_flags(-Wall)
    add_compiler_flags(-Wextra)
    add_compiler_flags(-fdiagnostics-show-option)
    add_compiler_flags(-Wconversion)
    add_compiler_flags(-Wold-style-cast)
    add_compiler_flags(-Wfloat-equal)
    add_compiler_flags(-Wlogical-op)
    add_compiler_flags(-Wundef)
    add_compiler_flags(-Wredundant-decls)
    add_compiler_flags(-Wshadow)
    add_compiler_flags(-Wstrict-overflow=5)
    add_compiler_flags(-Wwrite-strings)
    add_compiler_flags(-Wpointer-arith)
    add_compiler_flags(-Wcast-qual)
    add_compiler_flags(-Wformat=2)
    add_compiler_flags(-Wswitch-default)
    add_compiler_flags(-Wmissing-include-dirs)
    add_compiler_flags(-Wcast-align)
    add_compiler_flags(-Wswitch-enum)
    add_compiler_flags(-Wnon-virtual-dtor)
    add_compiler_flags(-Wctor-dtor-privacy)
    add_compiler_flags(-Wsign-conversion)
    add_compiler_flags(-Wdisabled-optimization)
    add_compiler_flags(-Weffc++)
    add_compiler_flags(-Winline)
    add_compiler_flags(-Winvalid-pch)
    add_compiler_flags(-Wmissing-declarations)
    add_compiler_flags(-Woverloaded-virtual)
    add_compiler_flags(-Wunused-but-set-variable)
    add_compiler_flags(-Wunused-result)
    
    # add_compiler_flags(-Wsuggest-override)
    # add_compiler_flags(-Wmultiple-inheritance)
    # add_compiler_flags(-Wcatch-value)
	# add_compiler_flags(-Wsuggest-attribute=cold)
    # add_compiler_flags(-Wsuggest-attribute=const)
    # add_compiler_flags(-Wsuggest-attribute=format)
    # add_compiler_flags(-Wsuggest-attribute=malloc)
    # add_compiler_flags(-Wsuggest-attribute=noreturn)
    # add_compiler_flags(-Wsuggest-attribute=pure)
    # add_compiler_flags(-Wsuggest-final-methods)
    # add_compiler_flags(-Wsuggest-final-types)
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
        add_compiler_flags(-Wnoexcept)
    endif()
    
    # no way to silence it in the expression decomposition macros: _Pragma() in macros doesn't work for the c++ front-end of g++
    # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55578
    # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69543
    # Also the warning is completely worthless nowadays - http://stackoverflow.com/questions/14016993
    #add_compiler_flags(-Waggregate-return)
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        add_compiler_flags(-Wdouble-promotion)
        add_compiler_flags(-Wtrampolines)
        add_compiler_flags(-Wzero-as-null-pointer-constant)
        add_compiler_flags(-Wuseless-cast)
        add_compiler_flags(-Wvector-operation-performance)
    endif()
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        add_compiler_flags(-Wshift-overflow=2)
        add_compiler_flags(-Wnull-dereference)
        add_compiler_flags(-Wduplicated-cond)
    endif()
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        add_compiler_flags(-Walloc-zero)
        add_compiler_flags(-Walloca)
        add_compiler_flags(-Wduplicated-branches)
    endif()
    
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
        add_compiler_flags(-Wcast-align=strict)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compiler_flags(-Weverything)
    add_compiler_flags(-Wno-c++98-compat)
    add_compiler_flags(-Wno-c++98-compat-pedantic)
    add_compiler_flags(-Wno-c++98-compat-bind-to-temporary-copy)
    add_compiler_flags(-Wno-c++98-compat-local-type-template-args)
    add_compiler_flags(-Qunused-arguments -fcolor-diagnostics) # needed for ccache integration on travis
endif()

if(MSVC)
    add_compiler_flags(/std:c++latest) # for post c++14 updates in MSVC
    add_compiler_flags(/permissive-)   # force standard conformance - this is the better flag than /Za
    add_compiler_flags(/WX)
    add_compiler_flags(/Wall) # turns on warnings from levels 1 through 4 which are off by default - https://msdn.microsoft.com/en-us/library/23k5d385.aspx
    
    add_compiler_flags(
        /wd4514 # unreferenced inline function has been removed
        /wd4571 # SEH related
        /wd4710 # function not inlined
        /wd4711 # function 'x' selected for automatic inline expansion
        
        /wd4616 # invalid compiler warnings - https://msdn.microsoft.com/en-us/library/t7ab6xtd.aspx
        /wd4619 # invalid compiler warnings - https://msdn.microsoft.com/en-us/library/tacee08d.aspx
        
        #/wd4820 # padding in structs
        #/wd4625 # copy constructor was implicitly defined as deleted
        #/wd4626 # assignment operator was implicitly defined as deleted
        #/wd5027 # move assignment operator was implicitly defined as deleted
        #/wd5026 # move constructor was implicitly defined as deleted
        #/wd4623 # default constructor was implicitly defined as deleted
    )
endif()
