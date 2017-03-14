## Features and design goals

**doctest** has been designed from the start to be as **light** and **unintrusive** as possible. These key features should be kept.

It is modeled after [**Catch**](https://github.com/philsquared/Catch) which is currently the most popular alternative for testing in C++ - check out [**the differences**](faq.md#how-is-doctest-different-from-catch).

## Unintrusive (transparent):

- everything testing-related can be removed from the binary executable by defining the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier
- very small and easy to integrate - single header
- **Extremely** low footprint on compile times - [**below 10ms**](benchmarks.md#cost-of-including-the-header) of compile time overhead for including the header in a file
- The [**fastest possible**](benchmarks.md#cost-of-an-assertion-macro) assertion macros - 50k asserts can compile for under 30 seconds (even under 10 sec)
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- everything is in the ```doctest``` namespace (and the implementation details are in a nested ```detail``` namespace)
- all macros have prefixes - some by default have unprefixed versions as well but that is optional - see [**configuration**](configuration.md)
- 0 warnings even with the most aggresive flags (on all tested compilers!!!)
	- ```-Weverything -pedantic``` for **clang**
	- ```-Wall -Wextra -pedantic``` and **>> over 50 <<** other warnings **not** covered by these flags for **GCC**!!! - see [**here**](../../scripts/common.cmake#L77)
	- ```/W4``` for **MSVC** (```/Wall``` is too much there - even their own headers produce **thousands** of warnings with that option)
- doesn't error on unrecognized [**command line**](commandline.md) options and supports prefixes for interop with client command line parsing
- can set options [**procedurally**](main.md) and not deal with passing ```argc```/```argv``` from the command line
- doesn't leave warnings disabled after itself

## Extremely portable:

- Standards compliant **C++98** code - should work with any **C++98** compiler
- tested with **GCC**: **4.4**, **4.5**, **4.6**, **4.7**, **4.8**, **4.9**, **5**, **6**
- tested with **Clang**: **3.4**, **3.5**, **3.6**, **3.7**, **3.8**, **3.9**
- tested with **MSVC**: **2008**, **2010**, **2012**, **2013**, **2015**
- per-commit tested on **travis** and **appveyor** CI services
	- warnings as errors even on the most aggressive warning levels - see [**here**](../../scripts/common.cmake#L71)
	- all tests have their output compared to reference output of a previous known good run
	- all tests built and ran in **Debug**/**Release** and also in **32**/**64** bit modes
	- all tests ran through **valgrind** under **Linux**/**OSX**
	- all tests ran through **address** and **UB** sanitizers under **Linux**/**OSX**
	- tests are ran in more than **200** different configurations on UNIX (Linux + OSX) on **travis** CI
	- tests are ran in a total of **20** different configurations on Windows on **appveyor** CI

---------------

This allows the library to be used in more ways than any other - tests can be written directly in the production code!

- This makes the barrier for writing tests **much lower** - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of its source file - or even header file!
- Tests in the production code can be thought of as documentation or up-to-date comments - showing how an API is used
- Testing internals that are not exposed through the public API and headers becomes easier!
- [**Test-driven development**](https://en.wikipedia.org/wiki/Test-driven_development) in C++ has never been easier!

The library can be used like any other if you don't like the idea of mixing production code and tests.

## Other features:

- really easy to get started - it's just 1 header file - see the [**tutorial**](tutorial.md)
- **very** light, unintrusive and portable - see the sections above - and also the [**benchmarks**](benchmarks.md)
- offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) macro
- tests are registered automatically - no need to add them to a collection manually
- supports [**subcases**](testcases.md) for easy setup/teardown of tests (also supports the retro [**test fixtures**](testcases.md#) with classes)
- output from all compilers on all platforms is the same - byte by byte
- binaries (exe/dll) can use the test runner of another binary - so tests end up in a single registry - [**example**](../../examples/dll_and_executable/)
- supports [**BDD style**](testcases.md) tests
- only one core [**assertion macro**](assertions.md) for comparisons - standard C++ operators are used for the comparison (less than, equal, greater than...) - yet the full expression is decomposed and left and right values of the expression are logged
- assertion macros for [**exceptions**](assertions.md) - if something should or shouldn't throw
- floating point comparison support - see the [**```Approx()```**](assertions.md#floating-point-comparisons) helper
- powerful mechanism for [**stringification**](stringification.md) of user types
- tests can be grouped in [**test suites**](testcases.md#test-suites)
- can be used without exceptions and rtti - checkout [**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](configuration.md#doctest_config_no_exceptions)
- powerful [**command line**](commandline.md) with lots of options
- tests can be [**filtered**](commandline.md) based on their name/file/test suite using wildcards
- failures can (optionally) break into the debugger on Windows and Mac
- integration with the output window of Visual Studio for failing tests
- a ```main()``` can be provided when implementing the library with the [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](main.md#doctest_config_implement_with_main) identifier
- can write tests in headers - they will still be registered only once in the executable/shared object
- [**range-based**](commandline.md) execution of tests - see the [**range_based_execution**](../../examples/range_based_execution/) example (the **run.py** script)
- colored output in the console
- controlling the order of test execution

There is a list of planned features which are all important and big - see the [**roadmap**](roadmap.md).

---------------

[Home](readme.md#reference)
