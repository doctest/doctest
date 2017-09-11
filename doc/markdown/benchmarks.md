# Benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake. There are 3 benchmarking scenarios:

- [the cost of including the header](#cost-of-including-the-header)
- [the cost of an assertion macro](#cost-of-an-assertion-macro)
- [runtime speed of lots of asserts](#runtime-benchmarks)

Compilers used:

- WINDOWS: Microsoft Visual Studio Community 2017 - Version 15.3.3+26730.12
- WINDOWS: gcc 7.1.0 (x86_64-posix-seh-rev2, Built by MinGW-W64 project)
- LINUX: gcc 6.3.0 20170406 (Ubuntu 6.3.0-12ubuntu2)
- LINUX: clang 4.0.0-1 (tags/RELEASE_400/rc1) Target: x86_64-pc-linux-gnu

Environment used (Intel i7 3770k, 16g RAM):

- Windows 7 - on an SSD
- Ubuntu 17.04 in a VirtualBox VM - on a HDD

**doctest** version: 1.2.2 (released on 2017.09.05)

[**Catch**](https://github.com/philsquared/Catch) version: 2.0.0-develop.3 (released on 2017.08.30)

# Compile time benchmarks

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
- **+ disabled** - remove everything testing-related from the binary

| doctest             | baseline | + implement | + header everywhere | + disabled |
|---------------------|----------|-------------|---------------------|------------|
| MSVC Debug          |    6.77 |    8.28 |   11.73 |    8.73 |
| MSVC Release        |    6.35 |    8.57 |   12.18 |    8.28 |
| MinGW GCC Debug     |   10.23 |   13.03 |   17.62 |   12.29 |
| MinGW GCC Release   |   10.33 |   13.68 |   17.87 |   13.11 |
| Linux GCC Debug     |    5.01 |    6.24 |   10.48 |    6.49 |
| Linux GCC Release   |    4.58 |    7.30 |   11.70 |    7.41 |
| Linux Clang Debug   |    8.80 |    9.70 |   14.92 |   10.89 |
| Linux Clang Release |    9.29 |   12.05 |   17.51 |   11.56 |

| Catch               | baseline | + implement | + header everywhere | + disabled |
|---------------------|----------|-------------|---------------------|------------|
| MSVC Debug          |    6.78 |   10.00 |  107.85 |  115.05 |
| MSVC Release        |    6.36 |   11.19 |  102.69 |  109.06 |
| MinGW GCC Debug     |   10.36 |   41.83 |  124.41 |  126.70 |
| MinGW GCC Release   |   10.49 |   21.93 |   97.81 |  105.47 |
| Linux GCC Debug     |    4.40 |   12.39 |   94.34 |   93.68 |
| Linux GCC Release   |    4.55 |   15.75 |   94.28 |   93.80 |
| Linux Clang Debug   |    9.30 |   15.00 |  105.84 |  103.05 |
| Linux Clang Release |    9.68 |   22.75 |  114.36 |  111.32 |

<img src="../../scripts/data/benchmarks/header.png" width="430" align="right">
<img src="../../scripts/data/benchmarks/implement.png" width="430">

### Conclusion

#### doctest

- instantiating the test runner in one source file costs ~1-3 seconds ```implement - baseline```
- the inclusion of ```doctest.h``` in one source file costs between 17ms - 27ms ```(header_everywhere - implement) / 200```
- including the library everywhere but everything disabled costs around 2 seconds ```disabled - baseline``` for 200 files

#### [Catch](https://github.com/philsquared/Catch)

- instantiating the test runner in one source file costs ~4-31 seconds ```implement - baseline```
- the inclusion of ```catch.hpp```  in one source file costs between 390ms - 490ms ```(header_everywhere - implement) / 200```
- using the config option to disable the library (**```CATCH_CONFIG_DISABLE```**) has no effect on the header cost

----------

So if ```doctest.h``` costs 17ms and ```catch.hpp``` costs 490ms on MSVC - then the **doctest** header is >> **27** << times lighter (for MSVC)!

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

[**Catch**](https://github.com/philsquared/Catch) specific:

- **+faster** - will add [**```CATCH_CONFIG_FAST_COMPILE```**](https://github.com/philsquared/Catch/blob/master/docs/configuration.md#catch_config_fast_compile) which speeds up the compilation of the normal asserts ```CHECK(a==b)```
- **+disabled** - all test case and assert macros will be disabled with **```CATCH_CONFIG_DISABLE```**

| doctest             | baseline | ```CHECK(a==b)``` | ```CHECK_EQ(a,b)``` | ```FAST_CHECK_EQ(a,b)``` | +faster | +disabled |
|---------------------|----------|-------------------|---------------------|--------------------------|---------|-----------|
| MSVC Debug          |    3.08 |   23.72 |   18.15 |    8.38 |    5.67 |    2.23 |
| MSVC Release        |    3.61 |   43.75 |   24.28 |   11.36 |    7.22 |    2.15 |
| MinGW GCC Debug     |    3.90 |   85.47 |   58.62 |   24.40 |   12.12 |    1.71 |
| MinGW GCC Release   |    4.51 |  224.49 |  148.84 |   47.25 |   18.73 |    2.40 |
| Linux GCC Debug     |    2.01 |   78.38 |   50.61 |   17.62 |    9.87 |    1.11 |
| Linux GCC Release   |    3.20 |  199.78 |  123.42 |   32.47 |   19.52 |    1.97 |
| Linux Clang Debug   |    1.71 |   77.39 |   49.97 |   17.60 |    7.57 |    1.18 |
| Linux Clang Release |    3.64 |  136.82 |   80.19 |   20.72 |   12.34 |    1.45 |

And here is [**Catch**](https://github.com/philsquared/Catch) which only has normal ```CHECK(a==b)``` asserts:

| Catch               | baseline | ```CHECK(a==b)``` | +faster | +disabled |
|---------------------|----------|-------------------|---------|-----------|
| MSVC Debug          |    9.58 |   37.69 |   25.21 |   10.40 |
| MSVC Release        |   10.85 |  260.55 |  121.38 |   11.56 |
| MinGW GCC Debug     |   36.24 |  159.15 |  133.98 |   33.57 |
| MinGW GCC Release   |   16.15 |  740.71 |  562.60 |   16.41 |
| Linux GCC Debug     |   12.71 |  142.92 |  108.07 |   12.05 |
| Linux GCC Release   |   15.62 |  825.42 |  612.06 |   15.51 |
| Linux Clang Debug   |   10.48 |  115.19 |   89.59 |   10.78 |
| Linux Clang Release |   18.25 |  393.31 |  316.98 |   17.19 |

<img src="../../scripts/data/benchmarks/asserts.png">

### Conclusion

**doctest**:

- is around 30% to 75% faster than [**Catch**](https://github.com/philsquared/Catch) when using normal expression decomposing ```CHECK(a==b)``` asserts
- asserts of the form ```CHECK_EQ(a,b)``` with no expression decomposition - around 25%-45% faster than ```CHECK(a==b)```
- fast asserts like ```FAST_CHECK_EQ(a,b)``` with no ```try/catch``` blocks - around 60-80% faster than ```CHECK_EQ(a,b)```
- the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) identifier which makes the fast assertions even faster by another 50-80%
- using the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier the assertions just disappear as if they were never written

[**Catch**](https://github.com/philsquared/Catch):

- using [**```CATCH_CONFIG_FAST_COMPILE```**](https://github.com/philsquared/Catch/blob/master/docs/configuration.md#catch_config_fast_compile) results in 15%-55% faster build times for asserts.
- using the **```CATCH_CONFIG_DISABLE```** identifier provides the same great benefits for assertion macros as the doctest version ([**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable)) - unlike the case for the header cost

## Runtime benchmarks

The runtime benchmarks consist of a single test case with a loop of 10 million iterations performing the task - a single normal assert (using expression decomposition) or the assert + the logging of the loop iterator ```i```:

```c++
for(int i = 0; i < 10000000; ++i)
    CHECK(i == i);
```

or

```c++
for(int i = 0; i < 10000000; ++i) {
    INFO(i);
    CHECK(i == i);
}
```

Note that the assert always passes - the goal should be to optimize for the common case - lots of passing test cases and a few that maybe fail.

| doctest             | assert  | + info  | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Catch               | assert  | + info  |
|---------------------|---------|---------|-|---------------------|---------|---------|
| MSVC Debug          |    5.04 |   13.03 | | MSVC Debug          |  101.07 |  338.41 |
| MSVC Release        |    0.73 |    1.67 | | MSVC Release        |    1.75 |   10.99 |
| MinGW GCC Debug     |    2.11 |    4.50 | | MinGW GCC Debug     |    4.76 |   18.22 |
| MinGW GCC Release   |    0.36 |    0.86 | | MinGW GCC Release   |    1.24 |    7.29 |
| Linux GCC Debug     |    2.49 |    4.97 | | Linux GCC Debug     |    5.41 |   19.01 |
| Linux GCC Release   |    0.29 |    0.66 | | Linux GCC Release   |    1.20 |    7.88 |
| Linux Clang Debug   |    2.39 |    4.76 | | Linux Clang Debug   |    5.12 |   17.66 |
| Linux Clang Release |    0.39 |    0.70 | | Linux Clang Release |    0.99 |    7.26 |

<img src="../../scripts/data/benchmarks/runtime_info.png" width="430" align="right">
<img src="../../scripts/data/benchmarks/runtime_assert.png" width="430">

### Conclusion

**doctest** is significantly faster - between 2.5 and 26 times.

In these particular cases **doctest** makes 0 allocations when the assert doesn't fail - it uses lazy stringification (meaning it stringifies the expression or the logged loop counter only if it has to) and a small-buffer optimized string class to achieve these results.

----------

The bar charts were generated using [**this google spreadsheet**](https://docs.google.com/spreadsheets/d/1p3MAURUfPzKT7gtJOVuJU2_yVKSqkoD1nbypA1K3618) by pasting the data from the tables.

If you want a benchmark that is not synthetic - check out [**this blog post**](http://baptiste-wicht.com/posts/2016/09/blazing-fast-unit-test-compilation-with-doctest-11.html) of [**Baptiste Wicht**](https://github.com/wichtounet) who tested the compile times of the asserts in the 1.1 release with his [**Expression Templates Library**](https://github.com/wichtounet/etl)!

While reading the post - keep in mind that if a part of a process takes 50% of the time and is made 10000 times faster - the overall process would still be only roughly 50% faster.

---------------

[Home](readme.md#reference)
