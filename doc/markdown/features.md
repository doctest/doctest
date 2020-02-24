## Features and design goals

**doctest** has been designed from the start to be as **light** and **unintrusive** as possible. These key features should be kept.

## Unintrusive (transparent):

- everything testing-related can be removed from the binary executable by defining the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier
- very small and easy to integrate - single header
- **Extremely** low footprint on compile times - [**around 25ms**](benchmarks.md#cost-of-including-the-header) of compile time overhead for including the header in a file
- The [**fastest possible**](benchmarks.md#cost-of-an-assertion-macro) assertion macros - 50k asserts can compile for under 30 seconds (even under 10 sec)
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- everything is in the ```doctest``` namespace (and the implementation details are in a nested ```detail``` namespace)
- all macros have prefixes - some by default have unprefixed versions as well but that is optional - see [**configuration**](configuration.md)
- 0 warnings even with the most aggressive flags (on all tested compilers!!!)
	- ```-Weverything -pedantic``` for **clang**
	- ```-Wall -Wextra -pedantic``` and **>> over 35 <<** other warnings **not** covered by these flags for **GCC**!!! - see [**here**](../../scripts/cmake/common.cmake#L84)
	- ```/Wall``` for **MSVC** (except for: ```C4514```, ```C4571```, ```C4710```, ```C4711```)
- doesn't error on unrecognized [**command line**](commandline.md) options and supports prefixes for interop with client command line parsing
- can set options [**procedurally**](main.md) and not deal with passing ```argc```/```argv``` from the command line
- doesn't leave warnings disabled after itself

## Extremely portable:

- Standards compliant **C++11** code - should work with any **C++11** capable compiler (use tag [**1.2.9**](https://github.com/onqtam/doctest/tree/1.2.9) for C++98 and older compilers)
- tested with **GCC**: **4.8**, **4.9**, **5**, **6**, **7**, **8**, **9**
- tested with **Clang**: **3.5**, **3.6**, **3.7**, **3.8**, **3.9**, **4**, **5**, **6**, **7**, **8**, **9** (XCode 6+)
- tested with **MSVC**: **2015**, **2017**, **2019** (also in 32 bit mode)
- per-commit tested on [**travis**](https://travis-ci.org/onqtam/doctest) and [**appveyor**](https://ci.appveyor.com/project/onqtam/doctest) CI services
	- warnings as errors even on the most aggressive warning levels - see [**here**](../../scripts/cmake/common.cmake#L84)
	- statically analyzed on the CI - [**Cppcheck**](http://cppcheck.sourceforge.net/) / [**Clang-Tidy**](https://clang.llvm.org/extra/clang-tidy/) / [**Coverity Scan**](https://scan.coverity.com/) / [**OCLint**](http://oclint.org/) / [**Visual Studio Analyzer**](https://docs.microsoft.com/en-us/visualstudio/code-quality/analyzing-c-cpp-code-quality-by-using-code-analysis)
	- all tests have their output compared to reference output of a previous known good run
	- all tests built and ran in **Debug**/**Release** modes
	- all tests ran through **valgrind** under **Linux** (sadly [not under OSX](https://github.com/onqtam/doctest/issues/11))
	- all tests ran through **address**, **UB** and **thread** sanitizers under **Linux**/**OSX**
	- tests are ran in more than **150** different configurations on UNIX (Linux + OSX) on **travis** CI
	- tests are ran in a total of **14** different configurations on Windows on **appveyor** CI

## Other features:

- really easy to get started - it's just 1 header file - see the [**tutorial**](tutorial.md)
- **very** light, unintrusive and portable - see the sections above - and also the [**benchmarks**](benchmarks.md)
- offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) macro
- tests are registered automatically - no need to add them to a collection manually
- [**Subcases**](tutorial.md#test-cases-and-subcases) - an intuitive way to share common setup and teardown code for test cases (alternative to [**test fixtures**](testcases.md#test-fixtures) which are also supported)
- [**templated test cases**](parameterized-tests.md#templated-test-cases---parameterized-by-type) - parameterized by type
- supports [**logging macros**](logging.md) for capturing local variables and strings - as a message for when an assert fails - with lazy stringification and no allocations when possible!
- crash handling support - uses signals for UNIX and SEH for Windows
- [**thread-safe**](faq.md#is-doctest-thread-aware) - asserts (and logging) can be used from multiple threads spawned from a single test case - [**example**](../../examples/all_features/concurrency.cpp)
- an extensible [**reporter system**](reporters.md) (can be also used for implementing event listeners)
- output from all compilers on all platforms is the same - byte by byte
- binaries (exe/dll) can use the test runner of another binary - so tests end up in a single registry - [**example**](../../examples/executable_dll_and_plugin/)
- supports [**BDD style**](testcases.md) tests
- one core [**assertion macro**](assertions.md) for comparisons - standard C++ operators are used for the comparison (less than, equal, greater than...) - yet the full expression is decomposed and left and right values of the expression are logged
- asserts can be used [**outside of a testing context**](assertions.md#using-asserts-out-of-a-testing-context) - [**example**](../../examples/all_features/asserts_used_outside_of_tests.cpp)
- assertion macros for [**exceptions**](assertions.md#exceptions) - if something should or shouldn't throw
- floating point comparison support - see the [**```Approx()```**](assertions.md#floating-point-comparisons) helper
- powerful mechanism for [**stringification**](stringification.md) of user types - including [**exceptions**](stringification.md#translating-exceptions)!
- tests can be grouped in [**test suites**](testcases.md#test-suites)
- test case [**decorators**](testcases.md#decorators) such as ```description``` / ```skip``` / ```may_fail``` / ```should_fail``` / ```expected_failures``` / ```timeout```
- can be used without exceptions and rtti - checkout [**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](configuration.md#doctest_config_no_exceptions)
- powerful [**command line**](commandline.md) with lots of options
- can report the duration of test cases
- tests can be [**filtered**](commandline.md) based on their name/file/test suite using wildcards
- can [**filter**](commandline.md) subcases using wildcards and by specifying the nesting levels for which those filters should work
- failures can (optionally) break into the debugger on Windows and Mac
- integration with the output window of Visual Studio for failing tests
- a ```main()``` can be provided when implementing the library with the [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](main.md#doctest_config_implement_with_main) identifier
- can write tests in headers - they will still be registered only once in the executable/shared object
- [**range-based**](commandline.md) execution of tests - see the [**example python script**](../../examples/range_based_execution.py)
- colored output in the console
- controlling the order of test execution
- different ```doctest::Context```s can be created and ran many times within a single execution of the program
- ability to query if code is currently being ran in a test -  ```doctest::is_running_in_test```
- tests can be registered in CTest with the use of [```doctest_discover_tests(<target>)``` from scripts/cmake/doctest.cmake](../../scripts/cmake/doctest.cmake)

There is a list of planned features which are all important and big - see the [**roadmap**](roadmap.md).

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
