## TODO FOR FIRST RELEASE

- CHECK/REQUIRE macros
- debugger present detection (and break when a CHECK/REQUIRE macro fails)
- a message macro
- look at catch command line options (also lest)
- ability to not run tests at all
- signal handling for unix: http://www.cplusplus.com/reference/csignal/signal/
- error reporting
- colors in output
- MSVC/IDE integration
- make a compact reporter (for within ide-s - just the file and line number)
- streaming reporters???
- xml reporter (jUnit compatible, etc.)
- test coverage
- fixing warnings gcc/clang/msvc (and add more stuff in the disabled example)
- benchmark
- documentation
- pledgie campaign - more info

## Spreading the word after 1.0.0 is released

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

- https://www.google.bg/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=list%20of%20testing%20frameworks%20c%2B%2B
- http://stackoverflow.com/a/33925554/3162383

## FUTURE
- [core] timing reports of tests, duration restrictions, kill of longer than, etc...
- [core] test execution in separate processes - UNIX only with fork()
- [CI] test with VC6 (and maybe get it somehow on a zip install for appveyor)
- [CI] try building with VS 2008/2010/2012/2013 also on appveyor - http://www.appveyor.com/docs/installed-software
- [CI] mingw-w64 on appveyor
- [CI] static analysis: msvc, clang, cppcheck
- [easy] hierarchical test suites? using a stack for the pushed states
- [easy] sorting the test order (by file, by test suite, or both, ASC/DESC...)
- [easy] error checking of parameters (if the range is valid (first <= last), etc)
- [unsure] ability to re-run only newly compiled tests - based on timestamps of the __FILE__ in which they are - and stored in some file
- [unsure] floating point comparison support
- [unsure] BDD based on the subtests - like Catch
- [unsure] matchers?
- [unsure] put internals in anonymous namespace (even if already in detail) - even though clang-format will make everything more indented
- [unsure] count the number of times TESTSUITE and TESTSUITE_END have been called and issue a warning when the tests are getting ran if there is a mismatch (also each TESTSUITE should check if it's called directly after a TESTSUITE without a TESTSUITE_END in between)
- [unsure] silencing warnings - take into account when a compiler (which version) accuires a pragma/attribute/warning (for gcc use -Wno-unknown-warning)
- [unsure] ability to transfer/copy registered functions from one dll to another so they are put in one set and duplicates are filtered
- [unsure] wchar stuff in stringify and whatever - see <wchar.h>

## DOCUMENTATION
- mimic catch front page - tutorial link, what is different link, documentation link.
- profile doctest vs Catch (compile/startup)
- defense of macros in testing frameworks: http://accu.org/var/uploads/journals/Overload125.pdf
- whats the library's main purpose
- warning free even with the most aggressive options for all 3 major compilers
- mocking is not included because it is orthogonal to testing and a different third party library may be used for that (google mock)
- check what features catch/lest have to offer (and what they say they lack)
- property based testing - what it is and how to use it with doctest
- document how to use spaces for filters in the comma separated list (using "")
- tests are ran serially
- document all the options
- initially was planning on a C version of the library but figured that there is no reason to choose C over C++ anywhere
- tests in headers... might end up in different test suites - and only 1 of them will get registered? or might have ifdef-ed parts that get compiled differently based on how/where the header is included...... so not a good practice to write tests in header files
- how subtests work - http://pastebin.com/rwghFzK4

















