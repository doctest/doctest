## Why does this library exist and how is it different from all the rest?

There are many C++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [lest](https://github.com/martinmoene/lest), [bandit](http://banditcpp.org/), [igloo](http://igloo-testing.org/), [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home), [CppTest](http://cpptest.sourceforge.net/), [CppUnit](https://sourceforge.net/projects/cppunit/), [CxxTest](https://github.com/CxxTest/cxxtest), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest), [cute](https://github.com/Kosta-Github/cute) and many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

**doctest** is much **lighter** and is **unintrusive**. It also offers a way to remove **everything** testing-related from the binary.

This allows the library to be used in more ways - tests can be written in the production code - just like with the ```unittest {}``` functionality of the **D** programming language. The name is inspired by the **docstrings** in python.

- This makes the barrier for writing tests much lower - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of the source file (or even header file).
- Also tests in the production code can be thought of as documentation or comments - showing how an API is used.
- Tests can be shipped to the customer with the software to diagnose a bug faster.
- [TDD](https://en.wikipedia.org/wiki/Test-driven_development) in C++ has never been easier!

The library can be used like any other even if you are not fond of the idea of mixing production code and tests. It is (or will be) on par with most of the other libraries as far as features go but is much lighter, portable and clear - see below.  

### Unintrusive (transparent):

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

### Extremely portable:

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

## Features

- really easy to get started - it's just 1 header file - see the [**tutorial**](tutorial.md#tutorial)
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
- tests can be [**filtered**](commandline.md#filtering) based on their name/file/test suite using wildcards
- failures can (optionally) break into the debugger on Windows and Mac
- integration with the output window of Visual Studio for failing tests
- a ```main()``` can be provided when implementing the library with the [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](main.md) identifier
- can write tests in headers - they will still be registered only once in the executable/shared object
- [**range-based**](commandline.md#ranges) execution of tests - see the [**multiprocess**](../../examples/multiprocess/) example (the **run.py** script)
- colored output in the console
- controlling the order of test execution

There is a list of planned features which are all important and big - see the [**roadmap**](roadmap.md#roadmap).

---------------

[Home](readme.md#reference)