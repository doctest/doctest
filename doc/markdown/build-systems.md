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
include(FetchContent)
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest
  GIT_TAG master # or any commit SHA, branch name or tag like v2.4.11
)
FetchContent_MakeAvailable(doctest)

# Include doctest_discover_tests(<target>) function
include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")
```

The target `doctest::doctest` is now exposed at the global scope and later you'll be able to use the doctest target like this:

```cmake
# Link doctest to a target
target_link_libraries(my_target PUBLIC doctest::doctest)

# Or link it globally
link_libraries(doctest::doctest)
```

Access the doctest headers with a scoped `#include` directive:

```c++
#include "doctest/doctest.h"
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

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
