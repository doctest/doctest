Features and design goals
=======

## Why does this library exist and how is it different from all the rest?

There are many c++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [lest](https://github.com/martinmoene/lest), [bandit](http://banditcpp.org/), [igloo](http://igloo-testing.org/), [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home), [CppTest](http://cpptest.sourceforge.net/), [CppUnit](https://sourceforge.net/projects/cppunit/), [CxxTest](https://github.com/CxxTest/cxxtest), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest), [cute](https://github.com/Kosta-Github/cute) and many many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

**doctest** is much **lighter** and is **unintrusive**. It also offers a way to remove everything testing-related from the binary.
 
This allows the library to be used in more ways - tests can be written in the production code - just like with the ```unittest {}``` functionality in the **D** programming language.

- This makes the barrier for writing tests is much lower - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of the source file (or even header file).
- Also tests in the production code can be thought of as documentation or comments - showing how an API is used.
- Tests can be shipped to the customer with the software to diagnose a bug faster.

Even if you don't see the ability of writing tests in your production code as beneficial the library can still be used like any other - it is (almost) on par with the rest as far as features go and is much lighter and portable (and might have better defaults). See the [**features**](Features:)

### Unintrusive:

- everything testing-related can be removed from the binary executable with a global define - see [**configuration**](configuration.md)
- very small and easy to integrate - single header - less than 3k LOC in the implementation translation unit and less than 1k LOC everywhere else - **extremely** low footprint on compile times - see the [**benchmarks**](benchmarks.md)
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- everything is in the ```doctest``` namespace (and the implementation details are in a nested ```detail``` namespace)
- all macros have prefixes - some by default have unprefixed versions as well but that is optional - see [**configuration**](configuration.md)
- 0 warnings even with the most aggresive flags (on all tested compilers!!!)
	- ```-Weverything -pedantic``` for **clang**
	- ```-Wall -Wextra -pedantic``` and **>> over 50 <<** other warnings **not** covered by these flags for **GCC**!!! - see [**here**](../../scripts/common.cmake#L59)
	- ```/W4``` for **MSVC** (```/Wall``` is too much there - even their own headers produce **thousands** of warnings with that option)
- doesn't error on unrecognized [**command line**](commandline.md) options and supports prefixes to not clash with user defined ones
- can set options procedurally and not worry about passing ```argc```/```argv``` from the command line
- doesn't leave warnings disabled after itself

### Extremely portable:

- Standards compliant **C++98** code
- tested with **GCC**: **4.4**, **4.5**, **4.6**, **4.7**, **4.8**, **4.9**, **5.0**
- tested with **Clang**: **3.4**, **3.5**, **3.6**, **3.7**, **3.8**
- tested with **MSVC**: **2008**, **2010**, **2012**, **2013**, **2015** (and even **VC++6** - that **18 year old compiler** from 1998!)
- should work with any **C++98** compiler
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

This library was modeled after [**Catch**](https://github.com/philsquared/Catch) and [**lest**](https://github.com/martinmoene/lest) - especially the subcases and the expression decomposition macros.

##(Features:)

- can remove everything testing related from the binary with the ```DOCTEST_CONFIG_DISABLE``` macro
- can write tests in headers - they will still be registered only once
- the library doesn't use operator ```new```/```delete``` (only ```malloc```) so you can test your ```operator new()```

## TODO for release

- look at catch command line options (also lest)
- sorting the test order (also RAND! and SEED!) (by file, by test suite, or both, ASC/DESC...)

- signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/
(signals on *NIX platforms or structured exceptions on Windows)

- colors in output
- MSVC/IDE integration
- make a compact reporter (for within ide-s - just the file and line number)
- streaming reporters???
- xml reporter (jUnit compatible, etc.)

- think about the expression decomposition static asserts
- kosta - test pledgie

- the help!
- examples and test coverage
- enabling COMPARE in tests
- test for warnings with -std=c++03/11/14/1z
- benchmark (assimp and empty files - or maybe just empty files)
- documentation
- pledgie campaign - more info

## FUTURE
- gcc 6
- pool allocator for String class
- floating point comparison support
- Bitwise() class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- timing reports of tests, duration restrictions, kill of longer than (will perhaps require threading), etc...
- test execution in separate processes - UNIX only with fork() (but windows has some .dll which could help)
- matchers?
- detect floating point exceptions
- [CI] static analysis: msvc, clang, cppcheck
- [CI] mingw-w64 on appveyor

## UNSURE

- BDD based on the subtests - like Catch
- tagging? also see this: https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md#special-tags
- utf8?
- a message macro (also tracepoint/passpoint/info/context and whatever - like in boost.test) (ALSO ERROR/FAIL - like boost)
- add WARN as third option to CHECK/REQUIRE versions of assertions
- hierarchical test suites? using a stack for the pushed states - should be easy
- ability to re-run only newly compiled tests - based on timestamps of the __FILE__ in which they are - and stored in some file
- put internals in anonymous namespace (even if already in detail) - even though clang-format will make everything more indented
- wchar stuff in stringify and whatever - see <wchar.h>
- progress of tests being executed (and an option for it)
- think about adding support for std::exception and others
- think about parameterising the output alignment to 80 or some other column limit
- think about the ability to mix different versions of the library within the same executable (like stb libraries)
- ability to transfer/copy registered functions from one dll to another so they are put in one set and duplicates are filtered








- mimic catch front page - tutorial link, what is different link, documentation link.
- profile doctest vs Catch (compile/startup)
- defense of macros in testing frameworks: http://accu.org/var/uploads/journals/Overload125.pdf
- whats the library's main purpose
- warning free even with the most aggressive options for all 3 major compilers
- mocking is not included because it is orthogonal to testing and a different third party library may be used for that (google mock)
https://github.com/tpounds/mockitopp
- check what features catch/lest have to offer (and what they say they lack)
- FAQ - linker issues (_IMPLEMENT, etc)...
- property based testing - what it is and how to use it with doctest
- document how to use spaces for filters in the comma separated list (using "")
- tests are ran serially
- listing reporters or counting tests implies no_run
- document all the options
- initially was planning on a C version of the library but figured that there is no reason to choose C over C++ anywhere
- tests in headers... might end up in different test suites - and only 1 of them will get registered? or might have ifdef-ed parts that get compiled differently based on how/where the header is included...... so not a good practice to write tests in header files
- how subtests work - http://pastebin.com/rwghFzK4














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

[Home](readme.md)