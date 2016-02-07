Coveralls Generator for CMake
=============================

This is a set of CMake scripts that are meant to be used to generate
and upload coverage data to http://coveralls.io/.

This is achieved by specifying which source files that you want to
gather the coverage data for and then running your unit tests, which
then will generate a coveralls compatible JSON file.

> **Note:** This was designed for use with http://travis-ci.org and has
> not been tested with any other Continous Integration service.

Dependencies
------------
For these scripts to work, the following dependencies are required:

* `gcov`- Needed to generate coverage data.
* `gcc` or `clang` - A compiler that supports adding coverage data
  that can be read by gcov.
* `curl` - Needed to upload the generated json file to coveralls.

Example project
---------------

Here an example CMake project can be found that shows how these
scripts can be used:
https://github.com/JoakimSoderberg/coveralls-cmake-example

Usage
-----

To use the script in your CMake project you need to do three things:

**1. Include the Coveralls.cmake script in CMakeLists.txt**

Place the files under your projects CMakeModules directory.
For example `${PROJECT_SOURCE_DIR}/cmake`, and include them.

```cmake
cmake_minimum_required(VERSION 2.8)
project(myproject)

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# We probably don't want this to run on every build.
option(COVERALLS "Generate coveralls data" OFF)

if (COVERALLS)
    include(Coveralls)
endif()

... # Setup the rest of your project

```

**2. Add coverage settings to your compile flags**

Either by using the supplied helper function.

```cmake
if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()
```

or you can add it yourself:

```cmake
if (COVERALLS)
    include(Coveralls)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

```

**3. Setup your target**

Explicitly tell what files that you want to gather the
coverage for (usually this is all your code files).

```cmake
if (COVERALLS)
    set(COVERAGE_SRCS awesome.c code.c files.c)

    # Create the coveralls target.
    coveralls_setup(
        "${COVERAGE_SRCS}" # The source files.
        ON                 # If we should upload.
        "${PROJECT_SOURCE_DIR}/CMakeModules/") # (Optional) Alternate project cmake module path.
endif()
```

That's it! If you did everything right, you can do:

> (Note build type must be Debug to get proper coverage data!).

```bash
$ mkdir build && cd build
$ cmake -DCOVERALLS=ON -DCMAKE_BUILD_TYPE=Debug ..
$ make
$ make coveralls
```

FAQ
---

**Q**: After I've setup everything according to the guide I get this message when running the script on Travis-ci:

    {"message":"Couldn't find a repository matching this job.","error":true}
    
What is wrong?

**A**: Not sure why this happens sometimes. But one thing to try is to try the solution mentioned in this ticket: https://github.com/lemurheavy/coveralls-public/issues/279

That is, adding this to your `.travis.yml` before `cmake` is run

    - export COVERALLS_SERVICE_NAME=travis-ci
    - export COVERALLS_REPO_TOKEN=abc12345

