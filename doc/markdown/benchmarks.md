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
| MSVC Debug          |    7.30 |    8.82 |   12.79 |    9.90 | 
| MSVC Release        |    7.11 |    8.90 |   12.96 |    9.43 | 
| MinGW GCC Debug     |   11.46 |   14.40 |   19.50 |   14.59 | 
| MinGW GCC Release   |   11.84 |   15.84 |   20.98 |   15.10 | 
| Linux GCC Debug     |    4.42 |    6.00 |   10.83 |    6.98 | 
| Linux GCC Release   |    4.56 |    7.37 |   12.11 |    7.91 | 
| Linux Clang Debug   |    8.55 |    9.48 |   15.98 |   12.06 | 
| Linux Clang Release |    9.55 |   12.40 |   18.98 |   12.37 | 

| Catch               | baseline | + implement | + header everywhere |
|---------------------|----------|-------------|---------------------|
| MSVC Debug          |    7.17 |   10.14 |  124.83 | 
| MSVC Release        |    7.08 |   10.90 |  117.53 | 
| MinGW GCC Debug     |   11.69 |   27.54 |  131.79 | 
| MinGW GCC Release   |   11.61 |   21.20 |  119.72 | 
| Linux GCC Debug     |    4.58 |    9.80 |  112.87 | 
| Linux GCC Release   |    4.70 |   13.70 |  115.18 | 
| Linux Clang Debug   |    9.50 |   12.55 |   77.50 | 
| Linux Clang Release |    9.84 |   18.65 |   84.67 | 

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
| MSVC Debug          |    3.35 |   26.23 |   20.04 |    8.72 |    5.98 |    2.42 | 
| MSVC Release        |    3.73 |   47.23 |   25.68 |   11.58 |    7.11 |    2.32 | 
| MinGW GCC Debug     |    4.31 |   93.31 |   63.66 |   26.32 |   13.14 |    1.94 | 
| MinGW GCC Release   |    4.94 |  241.47 |  156.84 |   49.50 |   19.65 |    2.57 | 
| Linux GCC Debug     |    2.30 |   90.96 |   61.43 |   21.26 |   13.13 |    1.36 | 
| Linux GCC Release   |    3.79 |  267.26 |  162.74 |   38.65 |   22.08 |    2.23 | 
| Linux Clang Debug   |    1.96 |   86.46 |   55.05 |   20.07 |    8.78 |    1.32 | 
| Linux Clang Release |    4.07 |  168.70 |   98.13 |   23.12 |   14.34 |    1.82 | 

And here is [**Catch**](https://github.com/philsquared/Catch) which only has normal ```CHECK(a==b)``` asserts:

| Catch               | baseline | ```CHECK(a==b)``` | +faster |
|---------------------|----------|-------------------|---------|
| MSVC Debug          |    9.94 |   40.14 |   36.66 | 
| MSVC Release        |   10.66 |  231.60 |   81.90 | 
| MinGW GCC Debug     |   21.20 |  129.26 |  110.95 | 
| MinGW GCC Release   |   14.59 |  297.04 |  207.75 | 
| Linux GCC Debug     |   12.38 |  133.64 |  110.75 | 
| Linux GCC Release   |   15.68 |  353.03 |  291.64 | 
| Linux Clang Debug   |    7.40 |  117.03 |   94.75 | 
| Linux Clang Release |   12.21 |  221.64 |  182.57 | 

### Conclusion

**doctest**:

- is around 25% faster than [**Catch**](https://github.com/philsquared/Catch) when using expression decomposing ```CHECK(a==b)``` asserts
- asserts of the form ```CHECK_EQ(a,b)``` with no expression decomposition - around 20% faster than ```CHECK(a==b)```
- fast asserts like ```FAST_CHECK_EQ(a,b)``` with no ```try/catch``` blocks - around 30-70% faster than ```CHECK_EQ(a,b)```
- the [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) identifier which makes the fast assertions even faster by another 35-80%
- using the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier the assertions just disappear as if they were never written

## Runtime benchmarks

```c++
for(int i = 0; i < 10000000; ++i)
    CHECK(i == i);
```

```c++
for(int i = 0; i < 10000000; ++i) {
    INFO(i);
    CHECK(i == i);
}
```

| doctest             | assert  | + info  |
|---------------------|---------|---------|
| MSVC Debug          |    5.29 |   14.32 | 
| MSVC Release        |    0.77 |    1.62 | 
| MinGW GCC Debug     |    2.25 |    4.71 | 
| MinGW GCC Release   |    0.39 |    0.90 | 
| Linux GCC Debug     |    2.84 |    5.13 | 
| Linux GCC Release   |    0.30 |    0.69 | 
| Linux Clang Debug   |    2.47 |    5.02 | 
| Linux Clang Release |    0.41 |    0.75 | 

| Catch               | assert  | + info |
|---------------------|---------|---------|
| MSVC Debug          |  365.37 |  621.78 | 
| MSVC Release        |    7.04 |   18.64 | 
| MinGW GCC Debug     |    9.22 |   21.89 | 
| MinGW GCC Release   |    7.29 |   13.95 | 
| Linux GCC Debug     |   11.17 |   24.79 | 
| Linux GCC Release   |    6.45 |   12.68 | 
| Linux Clang Debug   |   10.40 |   22.64 | 
| Linux Clang Release |    5.81 |   13.83 | 





----------

If you want a benchmark that is not synthetic - check out [**this blog post**](http://baptiste-wicht.com/posts/2016/09/blazing-fast-unit-test-compilation-with-doctest-11.html) of [**Baptiste Wicht**](https://github.com/wichtounet) who tested the compile times of the asserts in the 1.1 release with his [**Expression Templates Library**](https://github.com/wichtounet/etl)!

While reading the post - keep in mind that if a part of a process takes 50% of the time and is made 10000 times faster - the overall process would still be only roughly 50% faster.

---------------

[Home](readme.md#reference)
