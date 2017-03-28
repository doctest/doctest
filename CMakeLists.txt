cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME doctest)
project(${PROJECT_NAME} VERSION 1.1.4)

option(DOCTEST_SKIP_COVERAGE "Skip coverage" OFF)
option(DOCTEST_SKIP_EXAMPLES "Skip examples" OFF)

include(scripts/common.cmake)

include_directories("doctest/") # needed here so the coverage tools work - otherwise the "../../doctest" relative path fucks up

if(NOT ${DOCTEST_SKIP_COVERAGE})
    # setup coverage stuff only when COVERALLS_SERVICE_NAME is set (usually on travis CI)
    if(DEFINED ENV{COVERALLS_SERVICE_NAME})
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
        include(Coveralls)
        coveralls_turn_on_coverage()
        
        coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
        
        add_subdirectory(scripts/code_coverage_source)
        
        return()
    endif()

    add_subdirectory(scripts/code_coverage_source)
endif()

if(NOT ${DOCTEST_SKIP_EXAMPLES})
    file(GLOB subdir_list "${CMAKE_SOURCE_DIR}/examples/*")
    foreach(dir ${subdir_list})
        if(IS_DIRECTORY ${dir})
            get_filename_component(DIRNAME ${dir} NAME)
            add_subdirectory(examples/${DIRNAME})
        endif()
    endforeach()
endif()

add_library(${PROJECT_NAME} INTERFACE)
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(include_install_dir "include/doctest/")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_file("cmake/Config.cmake.in" "${project_config}" @ONLY)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${targets_export_name}"
    INCLUDES DESTINATION "${include_install_dir}"
)

install(
    FILES "doctest/doctest.h"
    DESTINATION "${include_install_dir}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
