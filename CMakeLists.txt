cmake_minimum_required(VERSION 3.0)

################################################################################
## DOCTEST
################################################################################

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/scripts/version.txt ver)
project(doctest VERSION ${ver})

option(DOCTEST_WITH_TESTS "Build tests/examples" ON)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/doctest/>)

################################################################################
## TESTS/EXAMPLES
################################################################################

if(${DOCTEST_WITH_TESTS})
    include(scripts/cmake/common.cmake)
    
    # setup coverage stuff only when COVERALLS_SERVICE_NAME is set (usually on travis CI)
    if(DEFINED ENV{COVERALLS_SERVICE_NAME})
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/coveralls-cmake/cmake)
        include(Coveralls)
        coveralls_turn_on_coverage()
        
        coveralls_setup("${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_CURRENT_SOURCE_DIR}/scripts/coveralls-cmake/cmake")
        
        add_subdirectory(examples/all_features)
        
        return()
    endif()
    
    file(GLOB subdir_list "${CMAKE_CURRENT_SOURCE_DIR}/examples/*")
    foreach(dir ${subdir_list})
        if(IS_DIRECTORY ${dir})
            get_filename_component(DIRNAME ${dir} NAME)
            add_subdirectory(examples/${DIRNAME})
        endif()
    endforeach()
endif()

################################################################################
## PACKAGE SUPPORT
################################################################################

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(include_install_dir "include/doctest/")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
)

configure_file("scripts/cmake/Config.cmake.in" "${project_config}" @ONLY)

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
