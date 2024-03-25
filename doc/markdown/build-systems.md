## Build systems

The latest released version of doctest can be obtained from here: https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h

You can substitute ```master``` with ```dev``` or a tag like ```v1.4.8``` for a specific version in the URL above.

### CMake

- **doctest** is easiest to use as a single file inside your own repository. Then the following minimal example will work:

```cmake
cmake_minimum_required(VERSION 3.0)
project(cmake_test VERSION 0.0.1 LANGUAGES CXX)

# Prepare doctest for other targets to use
find_package(doctest REQUIRED)

# Make test executable
add_executable(tests main.cpp)
target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests PRIVATE doctest::doctest)
```

- You can also use the following CMake snippet to automatically fetch the entire **doctest** repository from github and configure it as an external project:

```cmake
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
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
    - You can download and install doctest using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:
      ```sh
        git clone https://github.com/Microsoft/vcpkg.git
        cd vcpkg
        ./bootstrap-vcpkg.sh #.\bootstrap-vcpkg.bat(for windows)
        ./vcpkg integrate install
        ./vcpkg install doctest
      ```
      The doctest port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please create an issue or pull request on the [vcpkg repository](https://github.com/Microsoft/vcpkg).

- hunter
- conan
    - https://conan.io/center/doctest
    - https://github.com/conan-io/conan-center-index/tree/master/recipes/doctest
- Homebrew (`brew install doctest`)

- [build2](https://build2.org/) and [cppget.org](https://cppget.org/)
    + You can automatically download and use a specific version of **doctest** from the stable section of the [cppget.org](https://cppget.org/) package repositiory ([cppget.org/doctest](https://cppget.org/doctest)) in each of your [build2](https://build2.org) projects.
    + For that, make sure to add the stable section of the `cppget.org` repository to your project's `repositories.manifest` file to be able to fetch the package list.
        ```
        :
        role: prerequisite
        location: https://pkg.cppget.org/1/stable
        #trust: ...
        ```
    + Add the dependency on **doctest** with your optional specific version constraint to your project's `manifest` file to make the package available for import.
        ```
        depends: doctest ^ 2.4.10
        ```
    + Import **doctest** and add it to the dependencies of your application in your `buildfile`. Please note, it is still a single header-only library and nothing will be linked. Only the path of the header file will be made available to the executable.
        ```
        import doctest = doctest%lib{doctest}
        exe{my-exe}: {hxx cxx}{**} $doctest
        ```
    + For further information, please refer to [build2 | Documentation](https://build2.org/doc.xhtml), [cppget.org/doctest](https://cppget.org/doctest), or [GitHub: build2-packaging/doctest](https://github.com/build2-packaging/doctest) for bug reports.

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
