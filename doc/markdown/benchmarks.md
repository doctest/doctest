## Benchmarks

Compilers used:

- Microsoft Visual Studio Community 2015 - Version 14.0.25123.00 Update 2
- gcc 5.3.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 

Environment used - Windows 7 on an SSD, Intel i7 3770k, 16g RAM

Room temperature: 18

The benchmark is done with [**this**](../../scripts/bench/bench.py) script only under Windows (but can be adapted for Unix). 

The script generates 501 source files and in 500 of them makes a function in the form of ```int f135() { return 42; }``` and in ```main.cpp``` it forward declares all the 500 such dummy functions and accumulates their result to return from the ```main()``` function. This is done to ensure that all source files are built and that the linker doesn't remove something.

- **with implementation** - only in ```main.cpp``` the header is included with a ```#define``` before it so it gets implemented:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

- **+ header everywhere** - the framework header is also included in all the other source files
- **+ a test everywhere** - in all the source files there is 1 test case with 1 assertions defined
- **+ disabled** -

## doctest

| &nbsp; | Baseline | with implementation | + header everywhere | + a test everywhere | + disabled |
|--------------|----------|---------------------|------------------------------|---------------------|-------------------------|
| MSVC Debug |  |  |  |  |  |
| MSVC Release |  |  |  |  |  |
| GCC Debug |  |  |  |  |  |
| GCC Release |  |  |  |  |  |

## Catch

## Conclusion

