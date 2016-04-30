VC6 subcases not working - set a bounty on this:
http://stackoverflow.com/questions/36940730/preprocessor-problems-with-vc6
VC6 - templated stringify() overloads will not compile



- look at catch command line options (also lest)
- detect floating point exceptions
- colors
- sorting the test order (also RAND! and SEED!) (by file, by test suite, or both, ASC/DESC...)

- signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/
(signals on *NIX platforms or structured exceptions on Windows)

- colors in output
- MSVC/IDE integration
- make a compact reporter (for within ide-s - just the file and line number)
- streaming reporters???
- xml reporter (jUnit compatible, etc.)

- think about the expression decomposition static asserts
- test coverage
- the help!
- fixing warnings gcc/clang/msvc (and add more stuff in the disabled example)

GCC 4.8/5.0 -Wstrict-overflow NONSENSE

gcc 4.5/4.6 under osx - weird stuff
/Users/travis/build/onqtam/doctest/examples/hello_world/main.cpp:8:1: warning: offset outside bounds of constant string

- kosta - test pledgie
- try building with VS 2008/2010/2012/2013 also on appveyor - http://www.appveyor.com/docs/installed-software
- test for warnings with -std=c++03/11/14/1z
- benchmark (assimp and empty files - or maybe just empty files)
- documentation
- CONTRIBUTING.md - pull requests, travis, formatting, etc.
- pledgie campaign - more info


## FUTURE
- gcc 6
- pool allocator for String class
- Bitwise() class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- a message macro (also tracepoint/passpoint/info/context and whatever - like in boost.test) (ALSO ERROR/FAIL - like boost)



- [core] floating point comparison support
- [core] Bitwise() class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- [core] timing reports of tests, duration restrictions, kill of longer than, etc...
- [CI] mingw-w64 on appveyor
- [CI] static analysis: msvc, clang, cppcheck
- [core] ability to stop a test after it exceeds a time limit (will perhaps require threading)
- [core] test execution in separate processes - UNIX only with fork() (but windows has some .dll which could help)
- [core] matchers?

- [unsure] add WARN as third option to CHECK/REQUIRE versions of assertions
- [unsure] BDD based on the subtests - like Catch
- [unsure] hierarchical test suites? using a stack for the pushed states - should be easy
- [unsure] ability to re-run only newly compiled tests - based on timestamps of the __FILE__ in which they are - and stored in some file
- [unsure] put internals in anonymous namespace (even if already in detail) - even though clang-format will make everything more indented
- [unsure] wchar stuff in stringify and whatever - see <wchar.h>
- [unsure] tagging? also see this: https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md#special-tags
- [unsure] utf8?
- [unsure] progress of tests being executed (and an option for it)

- [unsure] think about adding support for std::exception and others
- [unsure] think about parameterising the output alignment to 80 or some other column limit
- [unsure] think about the ability to mix different versions of the library within the same executable (like stb libraries)
- [unsure] silencing warnings - take into account when a compiler (which version) accuires a pragma/attribute/warning (for gcc use -Wno-unknown-warning)
- [unsure] ability to transfer/copy registered functions from one dll to another so they are put in one set and duplicates are filtered


## Spreading the word after 1.0.0 is released

- bringing the unittest functionality of D into C++

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
https://github.com/nothings/stb/blob/master/docs/other_libs.md

- suggest to be linked to in https://github.com/nothings/stb

- https://www.google.bg/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=list%20of%20testing%20frameworks%20c%2B%2B
- http://stackoverflow.com/a/33925554/3162383

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














