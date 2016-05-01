Features and design goals
=======

This library was **strongly** influenced by [**Catch**](https://github.com/philsquared/Catch) and [**lest**](https://github.com/martinmoene/lest).

list of c++ testing libs
https://github.com/unittest-cpp/unittest-cpp
catch
lest
etc.

###extremely portable and bug free

- c++98
- Debug/Release
- 32/64 bit
- valgrind, address sanitizer, UB sanitizer, coverity (more static analysis coming soon)
- tested with gcc under OSX/Linux
    - 4.4
    - 4.5
    - 4.6
    - 4.7
    - 4.8
    - 4.9
    - 5
- tested with clang under OSX/Linux
    - 3.4
    - 3.5
    - 3.6
    - 3.7
    - 3.8
- tested with Visual Studio
    - VC++6 (yes - that 18 year old compiler from 1998)
    - 2008
    - 2010
    - 2012
    - 2013
    - 2015

###CI

- 180 builds linux/osx
- 18 builds windows

###unintrusive:

- everything testing-related can be removed from the binary executable with a global define
- all macros have prefixes
    - some by default have unprefixed versions as well but that can be turned off
- 0 warnings even with the most aggresive flags
- doesn't leave warnings disabled after itself
- doesn't drag any headers with itself
- everything is in doctest namespace
    - implementation details are in a nested namespace
- command line
    - doesn't error on unrecognized options
    - supports prefixed options that won't clash with user defines ones
- can set options procedurally and not worry about the command line



- can register tests in headers - they still will be registered only once




- The registration of test functions can be disabled by defining ```DOCTEST_GLOBAL_DISABLE``` before the inclusion of the **doctest.h** header. For large projects with tens of thousands of tests this may reduce the link time of the production build especially if lots of tests are registered in header files and will also reduce the binary size. See **disabled** from the examples folder.

- Tests are registered from top to bottom of each processed cpp after the headers have been preprocessed and included but there is no ordering between cpp files.

- Tests are registered globally within each shared object/executable. If a test is defined in a header and that header is used in an executable and in a shared object, then the test is registered in both places. To see how to invoke tests from a shared object check out **multi_dll** from the examples folder.

- The library does not use operator new/delete (only malloc) so it's memory usage is completely transparent to the user and that makes it fit for testing even memory management.





VC6 subcases not working - set a bounty on this:
http://stackoverflow.com/questions/36940730/preprocessor-problems-with-vc6
VC6 - templated stringify() overloads will not compile





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
- CONTRIBUTING.md - pull requests, travis, formatting, etc.
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
