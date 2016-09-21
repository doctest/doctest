# Compile time benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake. There are 2 benchmarking scenarios:
- [the cost of including the header](#cost-of-including-the-header)
- [the cost of an assertion macro](#cost-of-an-assertion-macro)

Compilers used:
- WINDOWS: Microsoft Visual Studio Community 2015 - Version 14.0.25431.01 Update 3
- WINDOWS: gcc 6.2.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)
- LINUX: gcc 5.2.1 20151010 (Ubuntu 5.2.1-22ubuntu2)
- LINUX: clang 3.6.2-1 (tags/RELEASE_362/final) (based on LLVM 3.6.2) Ubuntu - Target: x86_64-pc-linux-gnu

Environment used (Intel i7 3770k, 16g RAM)
- Windows 7 - on an SSD
- Ubuntu 15.10 in a VirtualBox VM - on a HDD

**doctest** version: 1.1.0 (released on 2016.09.21)

[**Catch**](https://github.com/philsquared/Catch) version: 1.5.6 (released on 2016.06.09)

## Cost of including the header

This is a benchmark that is relevant only to single header and header only frameworks - like **doctest** and [**Catch**](https://github.com/philsquared/Catch).

The script generates 201 source files and in 200 of them makes a function in the form of ```int f135() { return 135; }``` and in ```main.cpp``` it forward declares all the 200 such dummy functions and accumulates their result to return from the ```main()``` function. This is done to ensure that all source files are built and that the linker doesn't remove/optimize anything.

- **baseline** - how much time the source files need for a single threaded build with ```msbuild```/```make```
- **+ implement** - only in ```main.cpp``` the header is included with a ```#define``` before it so the test runner gets implemented:

    ```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
    ```
- **+ header everywhere** - the framework header is also included in all the other source files
- **+ disabled** - **doctest** specific - only this framework can remove everything related to it from the binary

| doctest             | baseline | + implement | + header everywhere | + disabled |
|---------------------|----------|-------------|---------------------|------------|
| MSVC Debug          | 5.9      | 7.1         | 8.3                 | 7.0        |
| MSVC Release        | 5.4      | 6.9         | 8.7                 | 6.5        |
| MinGW GCC Debug     | 9.4      | 11.7        | 14.4                | 11.1       |
| MinGW GCC Release   | 9.6      | 12.3        | 14.9                | 11.4       |
| Linux GCC Debug     | 6.3      | 7.1         | 10.2                | 7.4        |
| Linux GCC Release   | 6.5      | 8.4         | 10.8                | 7.8        |
| Linux Clang Debug   | 6.9      | 7.6         | 10.6                | 8.2        |
| Linux Clang Release | 7.2      | 8.4         | 11.4                | 8.4        |

| Catch               | baseline | + implement | + header everywhere |
|---------------------|----------|-------------|---------------------|
| MSVC Debug          | 5.9      | 8.5         | 102                 |
| MSVC Release        | 5.4      | 10.3        | 96                  |
| MinGW GCC Debug     | 9.4      | 24.5        | 125                 |
| MinGW GCC Release   | 9.6      | 18.4        | 113                 |
| Linux GCC Debug     | 6.3      | 10.4        | 59                  |
| Linux GCC Release   | 6.5      | 14.1        | 64                  |
| Linux Clang Debug   | 6.9      | 9.8         | 64                  |
| Linux Clang Release | 7.2      | 12.8        | 67                  |

### Conclusion

#### doctest

- instantiating the test runner in one source file costs ~1.5 seconds ```implement - baseline```
- the inclusion of ```doctest.h``` in one source file costs below 9ms ```(header_everywhere - implement) / 200```
- including the library everywhere but everything disabled costs less than 2 seconds ```disabled - baseline``` for 200 files

#### [Catch](https://github.com/philsquared/Catch)

- instantiating the test runner in one source file costs ~5 second ```implement - baseline```
- the inclusion of ```catch.hpp```  in one source file costs around 430ms ```(header_everywhere - implement) / 200```

----------

So if ```doctest.h``` costs 8ms and ```catch.hpp``` costs 430ms on MSVC - then the **doctest** header is >> **54** << times lighter!

----------

The results are in seconds and are in **no way** intended to bash [**Catch**](https://github.com/philsquared/Catch) - the **doctest** framework wouldn't exist without it.

The reason the **doctest** header is so light on compile times is because it forward declares everything and doesn't drag any headers in the source files (except for the source file where the test runner gets implemented). This was a key design decision.

## Cost of an assertion macro

The script generates 11 ```.cpp``` files and in 10 of them makes 50 test cases with 100 asserts in them (of the form ```CHECK(a==b)``` where ```a``` and ```b``` are always the same ```int``` variables) - **50k** asserts! The testing framework gets implemented in ```main.cpp```.

- **baseline** - how much time a single threaded build takes with the header included everywhere - no test cases or asserts!
- ```CHECK(a==b)``` - will add ```CHECK()``` asserts which decompose the expression with template machinery

**doctest** specific:

- ```CHECK_EQ(a,b)``` - will use ```CHECK_EQ(a,b)``` instead of the expression decomposing ones
- ```FAST_CHECK_EQ(a,b)``` - will use ```FAST_CHECK_EQ(a,b)``` instead of the expression decomposing ones
- **+faster** - will add [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) which speeds up ```FAST_CHECK_EQ(a,b)``` even more
- **+disabled** - all test case and assert macros will be disabled with [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable)

| doctest             | baseline | ```CHECK(a==b)``` | ```CHECK_EQ(a,b)``` | ```FAST_CHECK_EQ(a,b)``` | +faster | +disabled |
|---------------------|----------|-------------------|---------------------|--------------------------|---------|-----------|
| MSVC Debug          | 2.5      | 21                | 16.2                | 6.7                      | 4.4     | 2.2       |
| MSVC Release        | 2.6      | 64                | 55                  | 63                       | 5.3     | 1.8       |
| MinGW GCC Debug     | 3.2      | 77                | 52                  | 29.5                     | 12.2    | 1.6       |
| MinGW GCC Release   | 3.9      | 425               | 295                 | 81                       | 18.6    | 1.9       |
| Linux GCC Debug     | 1.3      | 72                | 48                  | 20.3                     | 9.5     | 0.9       |
| Linux GCC Release   | 2.3      | 339               | 210                 | 42                       | 18.3    | 1.3       |
| Linux Clang Debug   | 1.3      | 70                | 46                  | 18.8                     | 7.0     | 0.9       |
| Linux Clang Release | 1.8      | 205               | 136                 | 30                       | 10.8    | 1.1       |

And here are [**Catch**](https://github.com/philsquared/Catch) and **doctest 1.0** which only have normal ```CHECK(a==b)``` asserts:

| Catch               | baseline | ```CHECK(a==b)``` | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | doctest 1.0         | ```CHECK(a==b)``` |
|---------------------|----------|-------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|---------------------|-------------------|
| MSVC Debug          | 8.4      | 34                |                                                                                                                                                  | MSVC Debug          | 58                |
| MSVC Release        | 9.7      | 77                |                                                                                                                                                  | MSVC Release        | 367               |
| MinGW GCC Debug     | 20.5     | 115               |                                                                                                                                                  | MinGW GCC Debug     | 202               |
| MinGW GCC Release   | 15.1     | 496               |                                                                                                                                                  | MinGW GCC Release   | 1257              |
| Linux GCC Debug     | 7.3      | 101               |                                                                                                                                                  | Linux GCC Debug     | 204               |
| Linux GCC Release   | 10.3     | 435               |                                                                                                                                                  | Linux GCC Release   | 1090              |
| Linux Clang Debug   | 6.0      | 91                |                                                                                                                                                  | Linux Clang Debug   | 167               |
| Linux Clang Release | 8.5      | 159               |                                                                                                                                                  | Linux Clang Release | 494               |

### Conclusion

**doctest 1.1**:

- improves the compile time of it's ```CHECK(a==b)``` asserts by roughly 3 times compared to **doctest** 1.0 (released 2016.05.22) 
- is around 25% faster than [**Catch**](https://github.com/philsquared/Catch) when using expression decomposing ```CHECK(a==b)``` asserts
- adds asserts of the form ```CHECK_EQ(a,b)``` with no expression decomposition - around 20% faster than ```CHECK(a==b)```
- adds fast asserts like ```FAST_CHECK_EQ(a,b)``` with no ```try/catch``` blocks - around 30-70% faster than ```CHECK_EQ(a,b)```
- adds the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) identifier which makes the fast assertions even faster by another 35-80%
- using the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier the assertions just disappear as if they were never written

---------------

[Home](readme.md#reference)
