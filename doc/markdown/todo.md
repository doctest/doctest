## TODO FOR FIRST RELEASE

- colors in output

- fix command line


add _CONFIG_ in config macros




-dt-file
-dt-file-exclude
-dt-suite
-dt-suite-exclude
-dt-name
-dt-name-exclude





look at catch command line options (also lest)
- version (also add a step in the .txt file for updating the library - or even make it automatic!)
- randomization of test order
- detect floating point exceptions
- colors


- signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/
(signals on *NIX platforms or structured exceptions on Windows)

- MSVC/IDE integration
- make a compact reporter (for within ide-s - just the file and line number)
- streaming reporters???
- xml reporter (jUnit compatible, etc.)

- think about the expression decomposition static asserts
- example of stringify-ing common stl stuff like containers
- test coverage
- fixing warnings gcc/clang/msvc (and add more stuff in the disabled example)

GCC 4.8/5.0 -Wstrict-overflow NONSENSE

gcc 4.5/4.6 under osx - weird stuff
/Users/travis/build/onqtam/doctest/examples/hello_world/main.cpp:8:1: warning: offset outside bounds of constant string

- test for warnings with -std=c++03/11/14/1z
- benchmark
- documentation
- -dt-help   !!!
- CONTRIBUTING.md - pull requests, travis, formatting, etc.
- pledgie campaign - more info

## Spreading the word after 1.0.0 is released

- bringing the unittest functionality of D into C++

- reddit cpp/programming/gamedev/other
- boost mailing list
- comp.lang.c++
- comp.std.c++
- hackernews
- hora ot cg
- bg podcast? like bobi?
- gamedev/programming/c++ forums
- https://github.com/fffaraz/awesome-cpp
- add doctest to lists of c++ testing frameworks all around the web
- research how to market a new library

for gamedev - compile time!

https://www.facebook.com/groups/IndieGameDevs/
https://github.com/nothings/stb/blob/master/docs/other_libs.md

- suggest to be linked to in https://github.com/nothings/stb

- https://www.google.bg/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=list%20of%20testing%20frameworks%20c%2B%2B
- http://stackoverflow.com/a/33925554/3162383

## FUTURE
- [core] timing reports of tests, duration restrictions, kill of longer than, etc...
- [core] test execution in separate processes - UNIX only with fork() (but windows has some .dll which could help)
- [CI] test with VC6 (and maybe get it somehow on a zip install for appveyor)
- [CI] try building with VS 2008/2010/2012/2013 also on appveyor - http://www.appveyor.com/docs/installed-software
- [CI] mingw-w64 on appveyor
- [CI] static analysis: msvc, clang, cppcheck
- [easy] hierarchical test suites? using a stack for the pushed states
- [easy] sorting the test order (by file, by test suite, or both, ASC/DESC...)
- [easy] error checking of parameters (if the range is valid (first <= last), etc)
- [easy] pool allocator for String class
- [unsure] ability to re-run only newly compiled tests - based on timestamps of the __FILE__ in which they are - and stored in some file
- [unsure] Bitwise() class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- [unsure] add WARN as third option to CHECK/REQUIRE versions of assertions
- [unsure] floating point comparison support
- [unsure] ability to stop a test after it exceeds a time limit (will perhaps require threading)
- [unsure] BDD based on the subtests - like Catch
- [unsure] matchers?
- [unsure] put internals in anonymous namespace (even if already in detail) - even though clang-format will make everything more indented
- [unsure] silencing warnings - take into account when a compiler (which version) accuires a pragma/attribute/warning (for gcc use -Wno-unknown-warning)
- [unsure] ability to transfer/copy registered functions from one dll to another so they are put in one set and duplicates are filtered
- [unsure] progress of tests being executed (and an option for it)
- [unsure] wchar stuff in stringify and whatever - see <wchar.h>
- [unsure] tagging? also see this: https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md#special-tags
- [unsure] utf8?
- [unsure] think about adding support for std::exception and others
- [unsure] think about parameterising the output alignment to 80 or some other column limit
- [unsure] a message macro (also tracepoint/passpoint/info/context and whatever - like in boost.test) (ALSO ERROR/FAIL - like boost)
- think about the ability to mix different versions of the library within the same executable (like stb libraries)

## DOCUMENTATION

"the latest and greatest in unit testing in c++"

180 builds linux/osx (+12 if clang 3.4/3.5/3.6 get the 32 bit sanitizer stuff)
4 builds windows

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

list of c++ testing libs
https://github.com/unittest-cpp/unittest-cpp
catch
lest
etc.














