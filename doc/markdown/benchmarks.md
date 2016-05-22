## Benchmarks

This is a benchmark that is relevant only to single header and header only frameworks - like **doctest** and [**Catch**](https://github.com/philsquared/Catch).

The benchmark is done with [**this**](../../scripts/bench/bench.py) script only under Windows (but can be adapted for Unix) using CMake. 

It should be noted that GCC performs much better under Unix.

The script generates 501 source files and in 500 of them makes a function in the form of ```int f135() { return 135; }``` and in ```main.cpp``` it forward declares all the 500 such dummy functions and accumulates their result to return from the ```main()``` function. This is done to ensure that all source files are built and that the linker doesn't remove/optimize anything.

- **baseline** - how much time the source files need for a single threaded build with ```msbuild```/```mingw32-make``` 
- **+ implementation** - only in ```main.cpp``` the header is included with a ```#define``` before it so the test runner gets instantiated:

    ```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
    ```
- **+ header everywhere** - the framework header is also included in all the other source files
- **+ a test everywhere** - in all the source files there is 1 test case with 1 assertions defined
- **+ disabled** - **doctest** specific - only this framework can remove everything related to it from the binary

## doctest

| &nbsp; | baseline | + implementation | + header everywhere | + a test everywhere | + disabled |
|--------------|----------|---------------------|------------------------------|---------------------|-------------------------|
| MSVC Debug | 14.2 | 15.1 | 18.3 | 21.4 | 15.6 |
| MSVC Release | 13.4 | 14.5 | 18.7 | 23.2 | 15.2 |
| GCC Debug | 22.7 | 24.1 | 29.2 | 38 | 25 |
| GCC Release | 22.9 | 25 | 30 | 45 | 25 |

## Catch

| &nbsp; | baseline | + implementation | + header everywhere | + a test everywhere | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|--------------|----------|---------------------|------------------------------|---------------------|-------------------------|
| MSVC Debug | 14.2 | 17 | 242 | 249 |  |
| MSVC Release | 13.4 | 17.7 | 231 | 264 |  |
| GCC Debug | 22.7 | 36.7 | 178 | 189 |  |
| GCC Release | 22.9 | 31.5 | 163 | 193 |  |

## Conclusion

So on a modern developer machine:

### doctest

- instantiating the test runner in one source file costs ~1 second ```implementation - baseline```
- the inclusion of ```doctest.h``` in one source file costs below 9ms ```(header_everywhere - implementation) / 500```
- the addition of a test costs below 10ms ```(a_test_everywhere - header_everywhere) / 500```
  (below 18ms for MinGW-w64 but Linux GCC will be much faster)
- using the library everywhere with tests - but everything disabled - costs less than 2 seconds ```disabled - baseline```

### [**Catch**](https://github.com/philsquared/Catch)

- instantiating the test runner in one source file costs ~4 second ```implementation - baseline```
  (~12 seconds for MinGW-w64 but Linux GCC will be much faster)
- the inclusion of ```catch.hpp```  in one source file costs around 430ms ```(header_everywhere - implementation) / 500```
  (below 280ms for MinGW-w64 which is really odd)
- the addition of a test costs between 20ms and 60ms ```(a_test_everywhere - header_everywhere) / 500```

----------

So if ```doctest.h``` costs 8ms and ```catch.hpp``` costs 430ms on MSVC - then **doctest** is >> **54** << times lighter! 

----------

The results are in seconds and are in **no way** intended to bash [**Catch**](https://github.com/philsquared/Catch) - the **doctest** framework wouldn't exist without it.

The reason **doctest** is so light on compile times is because it forward declares everything and doesn't drag any standard headers in the source files (except for the source file where the test runner gets instantiated). This was a design decision from day 1.

## Details

Compilers used:

- Microsoft Visual Studio Community 2015 - Version 14.0.25123.00 Update 2
- gcc 5.3.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 

Environment used - Windows 7 on an SSD, Intel i7 3770k, 16g RAM

Date: 2016.05.22

**doctest** version: 1.0.0

[**Catch**](https://github.com/philsquared/Catch) version: 1.5.4

Room temperature: 18°C

---------------

[Home](readme.md#reference)
