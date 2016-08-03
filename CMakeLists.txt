cmake_minimum_required(VERSION 2.8)

project(all)

include(scripts/common.cmake)

# when on Travis CI force 64 bit for gcc 4.4 under OSX because -m32 fails
# saying that it cannot find 'std::exception' as a symbol (or others) for i386
if(DEFINED ENV{TRAVIS} AND APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5)
    add_compiler_flags(-m64)
endif()

include_directories("doctest/") # needed here so the coverage tools work - otherwise the "../../doctest" relative path fucks up

add_subdirectory(scripts/code_coverage_source)

# setup coverage stuff only when COVERALLS_SERVICE_NAME is set (usually on travis CI)
if(DEFINED ENV{COVERALLS_SERVICE_NAME})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
    include(Coveralls)
    coveralls_turn_on_coverage()
    
    coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
    
    return()
endif()

file(GLOB subdir_list "${CMAKE_SOURCE_DIR}/examples/*")
foreach(dir ${subdir_list})
    if(IS_DIRECTORY ${dir})
        get_filename_component(DIRNAME ${dir} NAME)
        add_subdirectory(examples/${DIRNAME})
    endif()
endforeach()
