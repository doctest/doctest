## Features and design goals

**doctest** has been designed from the start to be as **light** and **unintrusive** as possible. These key features should be kept.

## Unintrusive (transparent):

- everything testing-related can be removed from the binary executable by defining the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md) identifier
- very small and easy to integrate - single header - less than 3k LOC in the implementation translation unit and less than 1.5k LOC everywhere else - **extremely** low footprint on compile times - see the [**benchmarks**](benchmarks.md)
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- everything is in the ```doctest``` namespace (and the implementation details are in a nested ```detail``` namespace)
- all macros have prefixes - some by default have unprefixed versions as well but that is optional - see [**configuration**](configuration.md)
- 0 warnings even with the most aggresive flags (on all tested compilers!!!)
	- ```-Weverything -pedantic``` for **clang**
	- ```-Wall -Wextra -pedantic``` and **>> over 50 <<** other warnings **not** covered by these flags for **GCC**!!! - see [**here**](../../scripts/common.cmake#L59)
	- ```/W4``` for **MSVC** (```/Wall``` is too much there - even their own headers produce **thousands** of warnings with that option)
- doesn't error on unrecognized [**command line**](commandline.md) options and supports prefixes for interop with client command line parsing
- can set options [**procedurally**](main.md) and not deal with passing ```argc```/```argv``` from the command line
- doesn't leave warnings disabled after itself

## Extremely portable:

- Standards compliant **C++98** code - should work with any **C++98** compiler
- tested with **GCC**: **4.4**, **4.5**, **4.6**, **4.7**, **4.8**, **4.9**, **5.0**
- tested with **Clang**: **3.4**, **3.5**, **3.6**, **3.7**, **3.8**
- tested with **MSVC**: **2008**, **2010**, **2012**, **2013**, **2015** (and even **VC++6** - that **18 year old compiler** from 1998!)
- per-commit tested on **travis** and **appveyor** CI services
	- warnings as errors even on the most aggressive warning levels - see [**here**](../../scripts/common.cmake#L59)
	- all tests have their output compared to reference output of a previous known good run
	- all tests built and ran in **Debug**/**Release** and also in **32**/**64** bit modes
	- all tests ran through **valgrind** under **Linux**/**OSX**
	- all tests ran through **address** and **UB** sanitizers under **Linux**/**OSX**
	- tests are ran in a total of **180** different configurations on UNIX (Linux + OSX) on **travis** CI
	- tests are ran in a total of **18** different configurations on Windows on **appveyor** CI

## Other features:

- really easy to get started - it's just 1 header file - see the [**tutorial**](tutorial.md)
- **very** light, unintrusive and portable - see the sections above - and also the [**benchmarks**](benchmarks.md)
- offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md) macro
- tests are registered automatically - no need to add them to a collection manually
- supports [**subcases**](testcases.md#subcases) for easy setup/teardown of tests (also supports the retro [**test fixtures**](testcases.md#test-fixtures) with classes)
- supports [**BDD style**](testcases.md#bdd-style) tests
- only one core [**assertion macro**](assertions.md) for comparisons - standard C++ operators are used for the comparison (less than, equal, greater than...) - yet the full expression is decomposed and left and right values of the expression are logged
- assertion macros for [**exceptions**](assertions.md) - if something should or shouldn't throw
- floating point comparison support - see the [**```Approx()```**](assertions.md) helper
- powerful mechanism for [**```stringification```**](stringification.md) of user types
- tests can be grouped in [**test suites**](testcases.md#test-suites)
- powerful [**command line**](commandline.md) with lots of options
- tests can be [**filtered**](commandline.md) based on their name/file/test suite using wildcards
- failures can (optionally) break into the debugger on Windows and Mac
- integration with the output window of Visual Studio for failing tests
- a ```main()``` can be provided when implementing the library with the [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](main.md) identifier
- can write tests in headers - they will still be registered only once in the executable/shared object
- [**range-based**](commandline.md) execution of tests - see the [**range_based_execution**](../../examples/range_based_execution/) example (the **run.py** script)
- colored output in the console
- controlling the order of test execution

There is a list of planned features which are all important and big - see the [**roadmap**](roadmap.md).

---------------

[Home](readme.md#reference)
