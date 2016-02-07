project(ci)

option(CI_COVERAGE "coverage" OFF)

enable_testing()

if(CI_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ci/coveralls-cmake/cmake)
	include(Coveralls)
	coveralls_turn_on_coverage()
    
	coveralls_setup("${CMAKE_SOURCE_DIR}/doctest/doctest.h" ON "${CMAKE_SOURCE_DIR}/ci/coveralls-cmake/cmake")
endif()

add_subdirectory(examples/dev_testing)
