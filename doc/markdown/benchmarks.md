# Compile time benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake. There are 2 benchmarking scenarios:
- [the cost of including the header](#cost-of-including-the-header)
- [the cost of an assertion macro](#cost-of-an-assertion-macro)

GCC performs much better on Unix - and there the speedup ratios are similar (clang should be the similar as well).

Compilers used:
- Microsoft Visual Studio Community 2015 - Version 14.0.25431.01 Update 3
- gcc 6.2.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)
- gcc 5.2.1 20151010 (Ubuntu 5.2.1-22ubuntu2)

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

### doctest

| &nbsp;                | baseline | + implement | + header everywhere | + disabled |
|-----------------------|----------|-------------|---------------------|------------|
| MSVC Debug            | 5.9      | 7.1         | 8.3                 | 7.0        |
| MSVC Release          | 5.4      | 6.9         | 8.7                 | 6.5        |
| MinGW-w64 GCC Debug   | 9.4      | 11.7        | 14.4                | 11.1       |
| MinGW-w64 GCC Release | 9.6      | 12.3        | 14.9                | 11.4       |
| Linux GCC Debug       | 6.3      | 7.1         | 10.2                | 7.4        |
| Linux GCC Release     | 6.5      | 8.4         | 10.8                | 7.8        |

### Catch

| &nbsp;                | baseline | + implement | + header everywhere |
|-----------------------|----------|-------------|---------------------|
| MSVC Debug            | 5.9      | 8.5         | 102                 |
| MSVC Release          | 5.4      | 10.3        | 96                  |
| MinGW-w64 GCC Debug   | 9.4      | 24.5        | 125                 |
| MinGW-w64 GCC Release | 9.6      | 18.4        | 113                 |
| Linux GCC Debug       | 6.3      | 10.4        | 59                  |
| Linux GCC Release     | 6.5      | 14.1        | 64                  |

### Conclusion

So on a modern developer machine:

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

The script generates 11 source files and in 10 of them makes 50 test cases with 100 asserts in them (of the form ```CHECK(a == b);``` where a and b are always the same integer variables) - 50k asserts total. The testing framework gets implemented in ```main.cpp```.

- **baseline** - how much time a single threaded build takes with the header included everywhere - no test cases or asserts!
- **asserts** - will add ```CHECK()``` asserts which decompose the expression with template machinery

**doctest** specific:

- **+ disabled** - all test case and assert macros will be disabled with [**```DOCTEST_CONFIG_DISABLE```**](configuration.md)
- **normal** - will use ```CHECK_EQ(a, b)``` instead of the expression decomposing ones
- **fast** - will use ```FAST_CHECK_EQ(a, b)``` instead of the expression decomposing ones
- **+ extra fast** - will add [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md) which speeds up ```FAST_CHECK_EQ(a, b)``` even more

### doctest

| &nbsp;                 | baseline | asserts | doctest 1.0 | + disabled | normal | fast | + extra fast |
|------------------------|----------|---------|-------------|------------|--------|------|--------------|
| MSVC Debug             | 2.5      | 21      | 58          | 2.2        | 17.4   | 6.7  | 4.4          |
| MSVC Release           | 2.6      | 62      | 367         | 1.8        | 60     | 63   | 5.3          |
| MinGW-w64 GCC Debug    | 3.2      | 77      | 202         | 1.6        | 58     | 29.5 | 12.4         |
| MinGW-w64 GCC Release  | 3.9      | 425     | 1257        | 1.9        | 300    | 81   | 18.6         |
| Linux GCC Debug        |          |         |             |            |        |      |              |
| Linux GCC Release      |          |         |             |            |        |      |              |

### Catch

| &nbsp;                 | baseline | asserts |
|------------------------|----------|---------|
| MSVC Debug             | 8.4      | 34      |
| MSVC Release           | 9.7      | 77      |
| MinGW-w64 GCC Debug    | 20.5     | 115     |
| MinGW-w64 GCC Release  | 15.1     | 496     |
| Linux GCC Debug        |          |         |
| Linux GCC Release      |          |         |

---------------

[Home](readme.md#reference)
