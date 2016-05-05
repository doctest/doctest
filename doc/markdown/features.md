## Why does this library exist and how is it different from all the rest?

There are many C++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [lest](https://github.com/martinmoene/lest), [bandit](http://banditcpp.org/), [igloo](http://igloo-testing.org/), [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home), [CppTest](http://cpptest.sourceforge.net/), [CppUnit](https://sourceforge.net/projects/cppunit/), [CxxTest](https://github.com/CxxTest/cxxtest), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest), [cute](https://github.com/Kosta-Github/cute) and many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

**doctest** is much **lighter** and is **unintrusive**. It also offers a way to remove **everything** testing-related from the binary.

This allows the library to be used in more ways - tests can be written in the production code - just like with the ```unittest {}``` functionality of the **D** programming language. The name is inspired by the **docstrings** in python.

- This makes the barrier for writing tests much lower - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of the source file (or even header file).
- Also tests in the production code can be thought of as documentation or comments - showing how an API is used.
- Tests can be shipped to the customer with the software to diagnose a bug faster.

The library can be used like any other even if you are not fond of the idea of mixing production code and tests. It is (or will be) on par with most of the other libraries as far as features go but is much lighter, portable and clear - see the [**features**](#features).  

### Unintrusive (transparent):

- everything testing-related can be removed from the binary executable by defining the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md) identifier
- very small and easy to integrate - single header - less than 3k LOC in the implementation translation unit and less than 1k LOC everywhere else - **extremely** low footprint on compile times - see the [**benchmarks**](benchmarks.md)
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- everything is in the ```doctest``` namespace (and the implementation details are in a nested ```detail``` namespace)
- all macros have prefixes - some by default have unprefixed versions as well but that is optional - see [**configuration**](configuration.md)
- 0 warnings even with the most aggresive flags (on all tested compilers!!!)
	- ```-Weverything -pedantic``` for **clang**
	- ```-Wall -Wextra -pedantic``` and **>> over 50 <<** other warnings **not** covered by these flags for **GCC**!!! - see [**here**](../../scripts/common.cmake#L59)
	- ```/W4``` for **MSVC** (```/Wall``` is too much there - even their own headers produce **thousands** of warnings with that option)
- doesn't error on unrecognized [**command line**](commandline.md) options and supports prefixes to not clash with user defined ones
- can set options [**procedurally**](main.md) and not worry about passing ```argc```/```argv``` from the command line
- doesn't leave warnings disabled after itself

### Extremely portable:

- Standards compliant **C++98** code - should work with any **C++98** compiler
- tested with **GCC**: **4.4**, **4.5**, **4.6**, **4.7**, **4.8**, **4.9**, **5.0**
- tested with **Clang**: **3.4**, **3.5**, **3.6**, **3.7**, **3.8**
- tested with **MSVC**: **2008**, **2010**, **2012**, **2013**, **2015** (and even **VC++6** - that **18 year old compiler** from 1998!)
- per-commit tested on **travis** and **appveyor** CI services
	- warnings as errors even on the most aggressive warning levels - see [**here**](../../scripts/common.cmake#L59)
	- all tests have their output compared to reference output of a previous known good run
	- all tests built and ran in **Debug**/**Release**
	- all tests built and ran in **32**/**64** bit modes
	- all tests ran through **valgrind** under **Linux**/**OSX**
	- all tests ran through **address** and **UB** sanitizers under **Linux**/**OSX**
	- passes coverity static analysis (soon to integrate msvc/clang/cppcheck analysis)
	- tests are ran in a total of **180** different configurations on UNIX (Linux + OSX) on **travis** CI
	- tests are ran in a total of **18** different configurations on Windows on **appveyor** CI

## Features

- really easy to get started - it's just 1 header file - see the [**tutorial**](tutorial.md#tutorial)
- very light, unintrusive and portable - see the sections above
- tests are registered automatically - no need to add them to a collection manually
- supports [**sub cases**](testcases.md#subcases) for easy setup/teardown of tests (also supports the retro [**test fixtures**](testcases.md#test-fixtures) with classes)
- only one core [**assertion macro**](assertions.md) for comparisons - standard C++ operators are used for the comparison (less than, equal, greater than...) - yet the full expression is decomposed and left and right values of the expression are logged
- assertion macros for [**exceptions**](assertions.md) - if something should or shouldn't throw
- tests can be grouped in [**testsuites**](testcases.md#testsuites)
- tests can be [**filtered**](commandline.md#filtering) based on their name/file/testsuite using wildcards
- failures can (optionally) break into the debugger on Windows and Mac
- integration with the output window of Visual Studio for failing tests
- a ```main()``` can be provided when implementing the library with the [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](main.md) identifier
- can write tests in headers - they will still be registered only once in the executable/shared object
- the library doesn't use operator ```new```/```delete``` (only ```malloc```) so you can test your ```operator new()``` usage
- [**range-based**](commandline.md#ranges) execution of tests - see the [**multiprocess**](../../examples/multiprocess/) example (the **run.py** script)
- colored output in the console
- controlling the order of test execution

## TODO for release

[5/5/2016 4:50:41 PM | Edited 4:51:14 PM] Bobi: освен това CHECK_CLOSE или APPROX или нещо подобно за floats
[5/5/2016 4:51:01 PM] Bobi: или zip
[5/5/2016 4:51:05 PM] Bobi: или други функции
[5/5/2016 4:51:51 PM | Edited 4:52:38 PM] Bobi: и освен това custom messages - да се закачи допълнителна информация към check или warn
[5/5/2016 4:51:54 PM] Bobi: и WARN

bountysource/patreon

- colors in output

- print sections when asserts fail (look if catch does that)

- look at catch command line options (also lest)
- sorting the test order (also RAND! and SEED!) (by file, by test suite, or both, ASC/DESC...)
- running tests a few times
- disabling exceptions!!!
- the library should print its version before running tests
- print the test name on the first failed assert (or atleast the position) - like catch

- signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/
(signals on *NIX platforms or structured exceptions on Windows)

- think about the expression decomposition static asserts

- examples and test coverage
- enabling COMPARE in tests
- benchmark (assimp and empty files - or maybe just empty files)
- documentation

## FUTURE
- make a compact reporter
- output to file
- xml reporter (jUnit/xUnit compatible, etc.)
- ability for users to write their own reporters
- gcc 6
- pool allocator for String class
- floating point comparison support
- timing reports of tests, duration restrictions, kill of longer than (will perhaps require threading), etc...
- a message macro (also tracepoint/passpoint/info/context and whatever - like in boost.test) (ALSO ERROR/FAIL - like boost)
- add WARN as third option to CHECK/REQUIRE versions of assertions
- marking a test as "may fail"
- marking a test as (run X times (should also multiply with the global test run times))
- test execution in separate processes - UNIX only with fork() (but windows has some .dll which could help)
- matchers?
- detect floating point exceptions
- [CI] static analysis: msvc, clang, cppcheck
- [CI] mingw-w64 on appveyor

## UNSURE

- BDD based on the sub cases - like Catch
- Bitwise() class that has overloaded operators for comparison - to be used to check objects bitwise against each other (or maybe not - because of packing!)
- tagging? also see this: https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md#special-tags
- add the ability to query if code is currently being ran in a test - some global of doctest...
- utf8?
- hierarchical test suites? using a stack for the pushed states - should be easy
- ability to re-run only newly compiled tests - based on timestamps of the __FILE__ in which they are - and stored in some file
- ability to provide a temp folder to tests that is cleared between them
- put internals in anonymous namespace (even if already in detail) - even though clang-format will make everything more indented
- ability to check for memory leaks - comparing memory usage before and after the test with some API
- wchar stuff in stringify and whatever - see <wchar.h>
- progress of tests being executed (and an option for it)
- think about adding support for std::exception and others (mainly catching them so the .what() method can be called)
- think about parameterising the output alignment to 80 or some other column limit
- think about the ability to mix different versions of the library within the same executable (like stb libraries)














## Spreading the word after 1.0.0 is released

- reddit cpp/programming/gamedev/other
- boost mailing list
- comp.lang.c++
- comp.std.c++
- hackernews
- hora ot cg
- code project site!!! :D
- bg podcast? like bobi?
- gamedev/programming/c++ forums
- https://github.com/fffaraz/awesome-cpp
- add doctest to lists of c++ testing frameworks all around the web
- research how to market a new library

for gamedev - compile time!

https://www.facebook.com/groups/IndieGameDevs/

facebook fmi group

https://github.com/nothings/stb/blob/master/docs/other_libs.md

- suggest to be linked to in https://github.com/nothings/stb

- https://www.google.bg/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=list%20of%20testing%20frameworks%20c%2B%2B
- http://stackoverflow.com/a/33925554/3162383

---------------

[Home](readme.md#reference)