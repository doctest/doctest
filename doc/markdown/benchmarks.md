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

**doctest** version: 1.1.0 (released on 2016.09.20)

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

| doctest               | baseline | + implement | + header everywhere | + disabled |
|-----------------------|----------|-------------|---------------------|------------|
| MSVC Debug            | 5.9      | 7.1         | 8.3                 | 7.0        |
| MSVC Release          | 5.4      | 6.9         | 8.7                 | 6.5        |
| MinGW-w64 GCC Debug   | 9.4      | 11.7        | 14.4                | 11.1       |
| MinGW-w64 GCC Release | 9.6      | 12.3        | 14.9                | 11.4       |
| Linux GCC Debug       | 6.3      | 7.1         | 10.2                | 7.4        |
| Linux GCC Release     | 6.5      | 8.4         | 10.8                | 7.8        |
| Linux Clang Debug     | 6.9      | 7.6         | 10.6                | 8.2        |
| Linux Clang Release   | 7.2      | 8.4         | 11.4                | 8.4        |

| Catch                 | baseline | + implement | + header everywhere |
|-----------------------|----------|-------------|---------------------|
| MSVC Debug            | 5.9      | 8.5         | 102                 |
| MSVC Release          | 5.4      | 10.3        | 96                  |
| MinGW-w64 GCC Debug   | 9.4      | 24.5        | 125                 |
| MinGW-w64 GCC Release | 9.6      | 18.4        | 113                 |
| Linux GCC Debug       | 6.3      | 10.4        | 59                  |
| Linux GCC Release     | 6.5      | 14.1        | 64                  |
| Linux Clang Debug     | 6.9      | 9.8         | 64                  |
| Linux Clang Release   | 7.2      | 12.8        | 67                  |

### Conclusion

#### doctest

- instantiating the test runner in one source file costs ~1.5 seconds ```implement - baseline```
- the inclusion of ```doctest.h``` in one source file costs below 9ms ```(header_everywhere - implement) / 200```
- including the library everywhere - but everything disabled - costs less than 2 seconds ```disabled - baseline```

#### [Catch](https://github.com/philsquared/Catch)

- instantiating the test runner in one source file costs ~5 second ```implement - baseline```
  (~12 seconds for MinGW-w64)
- the inclusion of ```catch.hpp```  in one source file costs around 430ms ```(header_everywhere - implement) / 200```
  (below 280ms for MinGW-w64 which is really odd)

----------

So if ```doctest.h``` costs 8ms and ```catch.hpp``` costs 430ms on MSVC - then the **doctest** header is >> **54** << times lighter!

----------

The results are in seconds and are in **no way** intended to bash [**Catch**](https://github.com/philsquared/Catch) - the **doctest** framework wouldn't exist without it.

The reason the **doctest** header is so light on compile times is because it forward declares everything and doesn't drag any headers in the source files (except for the source file where the test runner gets implemented). This was a key design decision.

## Cost of an assertion macro

The script generates 11 source files and in 10 of them makes 50 test cases with 100 asserts in them (of the form ```CHECK(a==b)``` where ```a``` and ```b``` are always the same ```int``` variables) - 50k asserts! The testing framework gets implemented in ```main.cpp```.

- **baseline** - how much time a single threaded build takes with the header included everywhere - no test cases or asserts!
- ```CHECK(a==b)``` - will add ```CHECK()``` asserts which decompose the expression with template machinery

**doctest** specific:

- **+ disabled** - all test case and assert macros will be disabled with [**```DOCTEST_CONFIG_DISABLE```**](configuration.md)
- ```CHECK_EQ(a,b)``` - will use ```CHECK_EQ(a,b)``` instead of the expression decomposing ones
- ```CHECK_EQ_FAST(a,b)``` - will use ```FAST_CHECK_EQ(a,b)``` instead of the expression decomposing ones
- **+ extra fast** - will add [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md) which speeds up ```FAST_CHECK_EQ(a,b)``` even more

| doctest               | baseline | ```CHECK(a==b)``` | + disabled | ```CHECK_EQ(a,b)``` | ```CHECK_EQ_FAST(a,b)``` | + extra fast |
|-----------------------|----------|-------------------|------------|---------------------|--------------------------|--------------|
| MSVC Debug            | 2.5      | 21                | 2.2        | 16.2                | 6.7                      | 4.4          |
| MSVC Release          | 2.6      | 64                | 1.8        | 55                  | 63                       | 5.3          |
| MinGW-w64 GCC Debug   | 3.2      | 77                | 1.6        | 52                  | 29.5                     | 12.2         |
| MinGW-w64 GCC Release | 3.9      | 425               | 1.9        | 295                 | 81                       | 18.6         |
| Linux GCC Debug       | 1.3      | 72                | 0.9        | 48                  | 20.3                     | 9.5          |
| Linux GCC Release     | 2.3      | 339               | 1.3        | 210                 | 42                       | 18.3         |
| Linux Clang Debug     | 1.3      | 70                | 0.9        | 46                  | 18.8                     | 7.0          |
| Linux Clang Release   | 1.8      | 205               | 1.1        | 136                 | 30                       | 10.8         |

| Catch                 | baseline | ```CHECK(a==b)``` |
|-----------------------|----------|-------------------|
| MSVC Debug            | 8.4      | 34                |
| MSVC Release          | 9.7      | 77                |
| MinGW-w64 GCC Debug   | 20.5     | 115               |
| MinGW-w64 GCC Release | 15.1     | 496               |
| Linux GCC Debug       | 7.3      | 101               |
| Linux GCC Release     | 10.3     | 435               |
| Linux Clang Debug     | 6.0      | 91                |
| Linux Clang Release   | 8.5      | 159               |

The following table is with normal ```CHECK(a==b)``` asserts using ```doctest 1.0``` (it didn't have any other) which was released on 2016.05.22.

| doctest 1.0           | ```CHECK(a==b)``` |
|-----------------------|-------------------|
| MSVC Debug            | 58                |
| MSVC Release          | 367               |
| MinGW-w64 GCC Debug   | 202               |
| MinGW-w64 GCC Release | 1257              |
| Linux GCC Debug       | 204               |
| Linux GCC Release     | 1090              |
| Linux Clang Debug     | 167               |
| Linux Clang Release   | 494               |

### Conclusion

**doctest 1.1**:

- compared to 1.0 it improves the compile time of it's expression decomposing ```CHECK(a==b)``` macros by roughly 3 times - making it faster rather and not slower than [**Catch**](https://github.com/philsquared/Catch)
- adds alternative macros of the form ```CHECK_EQ(a,b)``` which don't decompose the expression - and they are around 20% faster than ```CHECK(a==b)```!
- adds even faster asserts like ```FAST_CHECK_EQ(a,b)``` which don't capture exceptions thrown while evaluating the assert - and they are around 30-70% faster than ```CHECK_EQ(a,b)```!
- adds the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md) identifier which makes the fast assertions even faster by another 35-80% (with the change that attached debuggers will break inside a doctest function when an assertion fails instead of at the exact line it was written - and going 1 level up in the callstack will be enough to find the actual assertion)

---------------

[Home](readme.md#reference)
