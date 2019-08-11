## Build systems

The latest released version of doctest can be obtained from here: https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h

You can substitute ```master``` with ```dev``` or a tag like ```1.2.9``` for a specific version in the URL above.

### CMake

- **doctest** is easiest to use as a single file inside your own repository. Then the following minimal example will work:

```cmake
cmake_minimum_required(VERSION 3.0)
project(cmake_test VERSION 0.0.1 LANGUAGES CXX)

# Prepare doctest for other targets to use
find_package(doctest REQUIRED)

# Make test executable
add_executable(tests main.cpp)
target_compile_features(test PRIVATE cxx_std_17)
target_link_libraries(test PRIVATE doctest::doctest)
```

- You can also use the following CMake snippet to automatically fetch the entire **doctest** repository from github and configure it as an external project:

```cmake
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")
```

And later you'll be able to use the doctest include directory like this:

```cmake
# add it globally
include_directories(${DOCTEST_INCLUDE_DIR})

# or per target
target_include_directories(my_target PUBLIC ${DOCTEST_INCLUDE_DIR})
```

- If you have the entire doctest repository available (as a submodule or just as files) you could also include it in your CMake build by using ```add_subdirectory(path/to/doctest)``` and then you could use it like this:

```cmake
add_executable(my_tests src_1.cpp src_2.cpp ...)
target_link_libraries(my_tests doctest)
```

- The ```CMakeLists.txt``` file of the doctest repository has ```install()``` commands so you could also use doctest as a package.

- To discover tests from an executable and register them in ctest you could use [```doctest_discover_tests(<target>)``` from scripts/cmake/doctest.cmake](../../scripts/cmake/doctest.cmake) - read the comments in the file on how to use it. It works just like [the same functionality in Catch](https://github.com/catchorg/Catch2/blob/master/docs/cmake-integration.md#automatic-test-registration).

### Package managers

**doctest** is available through the following package managers:

- vcpkg
- hunter
- conan
    - https://bintray.com/bincrafters/public-conan/doctest:bincrafters
    - https://bintray.com/mmha/conan/doctest%3Ammha

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
