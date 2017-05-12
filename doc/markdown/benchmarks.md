# Compile time benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake. There are 3 benchmarking scenarios:

- [the cost of including the header](#cost-of-including-the-header)
- [the cost of an assertion macro](#cost-of-an-assertion-macro)
- [runtime speed of lots of asserts](#runtime-benchmarks)

Compilers used:

- WINDOWS: Microsoft Visual Studio Community 2017 - Version 15.1.26403.7
- WINDOWS: gcc 7.1.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)
- LINUX: gcc 6.3.0 20170406 (Ubuntu 6.3.0-12ubuntu2)
- LINUX: clang 4.0.0-1 (tags/RELEASE_400/rc1) Target: x86_64-pc-linux-gnu

Environment used (Intel i7 3770k, 16g RAM):

- Windows 7 - on an SSD
- Ubuntu 17.04 in a VirtualBox VM - on a HDD

**doctest** version: 1.2.0 (released on 2017.05.12)

[**Catch**](https://github.com/philsquared/Catch) version: 1.9.3 (released on 2017.04.25)

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
| MSVC Debug          |     6.86 |    8.31 |   12.23 |    9.26 |
| MSVC Release        |     6.61 |    8.33 |   12.25 |    8.88 |
| MinGW GCC Debug     |    10.71 |   13.61 |   18.66 |   13.31 |
| MinGW GCC Release   |    10.97 |   14.54 |   19.37 |   14.36 |
| Linux GCC Debug     |     5.03 |    6.66 |   12.07 |    8.06 |
| Linux GCC Release   |     5.14 |    8.64 |   13.54 |    8.69 |
| Linux Clang Debug   |    10.03 |   10.69 |   16.64 |   12.52 |
| Linux Clang Release |    10.03 |   13.45 |   19.64 |   12.91 |

| Catch               | baseline | + implement | + header everywhere |
|---------------------|----------|-------------|---------------------|
| MSVC Debug          |     6.80 |    9.79 |  121.41 |
| MSVC Release        |     6.45 |   10.43 |  113.64 |
| MinGW GCC Debug     |    11.07 |   26.16 |  125.04 |
| MinGW GCC Release   |    10.98 |   19.81 |  112.85 |
| Linux GCC Debug     |     4.76 |   10.28 |  114.68 |
| Linux GCC Release   |     4.90 |   14.34 |  108.76 |
| Linux Clang Debug   |     8.59 |   11.49 |   71.56 |
| Linux Clang Release |     8.99 |   16.66 |   76.24 |

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
- **+faster** - will add [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) which speeds up ```FAST_CHECK_EQ(a,b)``` even more (or [**```CATCH_CONFIG_FAST_COMPILE```**](https://github.com/philsquared/Catch/blob/master/docs/configuration.md#catch_config_fast_compile) for Catch)
- **+disabled** - all test case and assert macros will be disabled with [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable)

| doctest             | baseline | ```CHECK(a==b)``` | ```CHECK_EQ(a,b)``` | ```FAST_CHECK_EQ(a,b)``` | +faster | +disabled |
|---------------------|----------|-------------------|---------------------|--------------------------|---------|-----------|
| MSVC Debug          |     3.19 |   24.16 |   18.13 |    8.16 |    5.71 |    2.27 |
| MSVC Release        |     3.49 |   40.70 |   66.52 |   39.78 |   34.78 |    2.25 |
| MinGW GCC Debug     |     4.12 |   93.42 |   62.86 |   26.73 |   13.27 |    1.94 |
| MinGW GCC Release   |     4.92 |  332.98 |  220.50 |   50.07 |   20.06 |    2.65 |
| Linux GCC Debug     |     2.08 |   81.46 |   52.38 |   18.07 |   10.13 |    1.17 |
| Linux GCC Release   |     3.24 |  272.87 |  169.95 |   33.10 |   19.72 |    2.01 |
| Linux Clang Debug   |     1.78 |   79.25 |   50.83 |   17.82 |    7.65 |    1.23 |
| Linux Clang Release |     3.75 |  143.71 |   82.81 |   22.59 |   14.01 |    1.50 |

And here is [**Catch**](https://github.com/philsquared/Catch) which only has normal ```CHECK(a==b)``` asserts:

| Catch               | baseline | ```CHECK(a==b)``` | +faster |
|---------------------|----------|-------------------|---------|
| MSVC Debug          |    10.26 |   41.55 |   38.40 |
| MSVC Release        |    11.15 |  240.24 |   86.17 |
| MinGW GCC Debug     |    21.94 |  141.73 |  122.63 |
| MinGW GCC Release   |    16.28 |  336.07 |  230.26 |
| Linux GCC Debug     |    10.12 |  115.74 |   99.07 |
| Linux GCC Release   |    13.40 |  294.40 |  216.19 |
| Linux Clang Debug   |     6.41 |  103.18 |   84.68 |
| Linux Clang Release |    11.12 |  193.37 |  154.67 |

### Conclusion

**doctest**:

- is around 25% faster than [**Catch**](https://github.com/philsquared/Catch) when using expression decomposing ```CHECK(a==b)``` asserts
- asserts of the form ```CHECK_EQ(a,b)``` with no expression decomposition - around 20% faster than ```CHECK(a==b)```
- fast asserts like ```FAST_CHECK_EQ(a,b)``` with no ```try/catch``` blocks - around 30-70% faster than ```CHECK_EQ(a,b)```
- the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) identifier which makes the fast assertions even faster by another 35-80%
- using the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier the assertions just disappear as if they were never written

## Runtime benchmarks

| doctest             | baseline | + implement |
|---------------------|----------|-------------|
| MSVC Debug          | 7.0      | 8.73        |
| MSVC Release        | 6.81     | 8.86        |
| MinGW GCC Debug     | 11.43    | 14.46       |
| MinGW GCC Release   | 11.35    | 15.11       |
| Linux GCC Debug     | 4.47     | 5.89        |
| Linux GCC Release   | 5.09     | 8.04        |
| Linux Clang Debug   | 9.33     | 10.87       |
| Linux Clang Release | 10.37    | 13.95       |

| Catch               | baseline | + implement |
|---------------------|----------|-------------|
| MSVC Debug          | 7.18     | 10.31       |
| MSVC Release        | 6.72     | 11.75       |
| MinGW GCC Debug     | 11.36    | 27.44       |
| MinGW GCC Release   | 11.59    | 20.85       |
| Linux GCC Debug     | 5.24     | 11.15       |
| Linux GCC Release   | 6.19     | 14.44       |
| Linux Clang Debug   | 9.64     | 12.88       |
| Linux Clang Release | 9.32     | 17.52       |

----------

If you want a benchmark that is not synthetic - check out [**this blog post**](http://baptiste-wicht.com/posts/2016/09/blazing-fast-unit-test-compilation-with-doctest-11.html) of [**Baptiste Wicht**](https://github.com/wichtounet) who tested the compile times of the asserts in the 1.1 release with his [**Expression Templates Library**](https://github.com/wichtounet/etl)!

While reading the post - keep in mind that if a part of a process takes 50% of the time and is made 10000 times faster - the overall process would still be only roughly 50% faster.

---------------

[Home](readme.md#reference)
