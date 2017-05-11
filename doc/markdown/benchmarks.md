# Compile time benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake. There are 2 benchmarking scenarios:

- [the cost of including the header](#cost-of-including-the-header)
- [the cost of an assertion macro](#cost-of-an-assertion-macro)

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
| MSVC Debug          | 7.0      | 8.73        | 13.84               | 10.07      |
| MSVC Release        | 6.81     | 8.86        | 12.84               | 9.26       |
| MinGW GCC Debug     | 11.43    | 14.46       | 19.62               | 14.04      |
| MinGW GCC Release   | 11.35    | 15.11       | 20.28               | 14.92      |
| Linux GCC Debug     | 4.47     | 5.89        | 11.11               | 7.86       |
| Linux GCC Release   | 5.09     | 8.04        | 13.71               | 8.27       |
| Linux Clang Debug   | 9.33     | 10.87       | 17.13               | 12.50      |
| Linux Clang Release | 10.37    | 13.95       | 21.38               | 13.77      |

| Catch               | baseline | + implement | + header everywhere |
|---------------------|----------|-------------|---------------------|
| MSVC Debug          | 7.18     | 10.31       | 125.23              |
| MSVC Release        | 6.72     | 11.75       | 118.03              |
| MinGW GCC Debug     | 11.36    | 27.44       | 133.04              |
| MinGW GCC Release   | 11.59    | 20.85       | 120.13              |
| Linux GCC Debug     | 5.24     | 11.15       | 124.62              |
| Linux GCC Release   | 6.19     | 14.44       | 109.81              |
| Linux Clang Debug   | 9.64     | 12.88       | 76.05               |
| Linux Clang Release | 9.32     | 17.52       | 86.36               |

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
| MSVC Debug          | 3.47     | 27.42             | 19.98               | 8.65                     | 5.98    | 2.43      |
| MSVC Release        | 3.47     | 27.42             | 19.98               | 8.65                     | 5.98    | 2.43      |
| MinGW GCC Debug     | 4.36     | 96.67             | 63.72               | 26.50                    | 13.25   | 1.95      |
| MinGW GCC Release   | 4.95     | 334.70            | 223.42              | 51.60                    | 20.36   | 2.68      |
| Linux GCC Debug     | 2.23     | 88.14             | 60.09               | 21.06                    | 10.49   | 1.23      |
| Linux GCC Release   | 3.49     | 343.04            | 226.29              | 35.53                    | 21.19   | 2.24      |
| Linux Clang Debug   | 1.98     | 87.85             | 56.29               | 19.13                    | 8.22    | 1.36      |
| Linux Clang Release | 4.14     | 165.14            | 92.83               | 24.17                    | 15.05   | 1.63      |

And here is [**Catch**](https://github.com/philsquared/Catch) which only has normal ```CHECK(a==b)``` asserts:

| Catch               | baseline | ```CHECK(a==b)``` | +faster |
|---------------------|----------|-------------------|---------|
| MSVC Debug          | 10.19    | 44.83             | 41.19   |
| MSVC Release        | 11.14    | 260.07            | 93.73   |
| MinGW GCC Debug     | 22.21    | 139.46            | 115.31  |
| MinGW GCC Release   | 15.11    | 319.49            | 216.29  |
| Linux GCC Debug     | 11.05    | 129.49            | 108.81  |
| Linux GCC Release   | 14.60    | 383.89            | 272.97  |
| Linux Clang Debug   | 7.04     | 110.55            | 92.71   |
| Linux Clang Release | 12.06    | 223.84            | 176.36  |

### Conclusion

**doctest**:

- is around 25% faster than [**Catch**](https://github.com/philsquared/Catch) when using expression decomposing ```CHECK(a==b)``` asserts
- asserts of the form ```CHECK_EQ(a,b)``` with no expression decomposition - around 20% faster than ```CHECK(a==b)```
- fast asserts like ```FAST_CHECK_EQ(a,b)``` with no ```try/catch``` blocks - around 30-70% faster than ```CHECK_EQ(a,b)```
- the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) identifier which makes the fast assertions even faster by another 35-80%
- using the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier the assertions just disappear as if they were never written

----------

If you want a benchmark that is not synthetic - check out [**this blog post**](http://baptiste-wicht.com/posts/2016/09/blazing-fast-unit-test-compilation-with-doctest-11.html) of [**Baptiste Wicht**](https://github.com/wichtounet) who tested the compile times of the asserts in the 1.1 release with his [**Expression Templates Library**](https://github.com/wichtounet/etl)!

While reading the post - keep in mind that if a part of a process takes 50% of the time and is made 10000 times faster - the overall process would still be only roughly 50% faster.

---------------

[Home](readme.md#reference)
