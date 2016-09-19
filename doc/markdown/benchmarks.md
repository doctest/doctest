# Benchmarks

The benchmarks are done with [**this**](../../scripts/bench/bench.py) script using CMake.

It should be noted that GCC performs much better on Unix - but even there the speedup ratios are similar (clang should be the similar).

## Cost of including the header

This is a benchmark that is relevant only to single header and header only frameworks - like **doctest** and [**Catch**](https://github.com/philsquared/Catch).

The script generates 201 source files and in 200 of them makes a function in the form of ```int f135() { return 135; }``` and in ```main.cpp``` it forward declares all the 200 such dummy functions and accumulates their result to return from the ```main()``` function. This is done to ensure that all source files are built and that the linker doesn't remove/optimize anything.

- **baseline** - how much time the source files need for a single threaded build with ```msbuild```/```mingw32-make``` 
- **+ implementation** - only in ```main.cpp``` the header is included with a ```#define``` before it so the test runner gets instantiated:

    ```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
    ```
- **+ header everywhere** - the framework header is also included in all the other source files
- **+ disabled** - **doctest** specific - only this framework can remove everything related to it from the binary

### doctest

| &nbsp;                 | baseline | + implementation | + header everywhere | + disabled |
|------------------------|----------|------------------|---------------------|------------|
| MSVC Debug             | 5.9      | 7.1              | 8.3                 | 7.0        |
| MSVC Release           | 5.4      | 6.9              | 8.7                 | 6.5        |
| MinGW-w64 GCC Debug    | 9.4      | 11.7             | 14.4                | 11.1       |
| MinGW-w64 GCC Release  | 9.6      | 12.3             | 14.9                | 11.4       |

### Catch

| &nbsp;                 | baseline | + implementation | + header everywhere | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|------------------------|----------|------------------|---------------------|------------|
| MSVC Debug             | 5.9      | 8.5              | 102                 |            |
| MSVC Release           | 5.4      | 10.3             | 96                  |            |
| MinGW-w64 GCC Debug    | 9.4      | 24.5             | 125                 |            |
| MinGW-w64 GCC Release  | 9.6      | 18.4             | 113                 |            |

### Conclusion

So on a modern developer machine:

#### doctest

- instantiating the test runner in one source file costs ~1.5 seconds ```implementation - baseline```
- the inclusion of ```doctest.h``` in one source file costs below 9ms ```(header_everywhere - implementation) / 200```
- including the library everywhere - but everything disabled - costs less than 2 seconds ```disabled - baseline```

#### [Catch](https://github.com/philsquared/Catch)

- instantiating the test runner in one source file costs ~5 second ```implementation - baseline```
  (~12 seconds for MinGW-w64)
- the inclusion of ```catch.hpp```  in one source file costs around 430ms ```(header_everywhere - implementation) / 200```
  (below 280ms for MinGW-w64 which is really odd)

----------

So if ```doctest.h``` costs 8ms and ```catch.hpp``` costs 430ms on MSVC - then the **doctest** header is >> **54** << times lighter! 

----------

The results are in seconds and are in **no way** intended to bash [**Catch**](https://github.com/philsquared/Catch) - the **doctest** framework wouldn't exist without it.

The reason the **doctest** header is so light on compile times is because it forward declares everything and doesn't drag any standard headers in the source files (except for the source file where the test runner gets implemented). This was a key design decision.

## Cost of an assertion macro

trololo

# Details

Compilers used:

- Microsoft Visual Studio Community 2015 - Version 14.0.25431.01 Update 3
- gcc 6.2.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)

Environment used - Windows 7 on an SSD, Intel i7 3770k, 16g RAM

Date: 2016.09.19

**doctest** version: 1.1.0

[**Catch**](https://github.com/philsquared/Catch) version: 1.5.6

---------------

[Home](readme.md#reference)
