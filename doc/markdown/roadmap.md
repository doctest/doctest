## Roadmap

Support the development of the project with donations! I work on this project in my spare time and every cent is a big deal.

[![Donate to support](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)

This is a list of planned features for future releases (maybe in the given order).

- the set holding all registered tests should use a specialized allocator to minimize program startup time
- ability to have no output when everything succeeds
- a mechanism for translating exceptions - users should be able to teach the framework about their types (look at Catch for this)
- support for ```std::exception``` and derivatives (mainly for calling the ```.what()``` method when caught unexpectedly)
- test with missed warning flags for GCC - look into https://github.com/Barro/compiler-warnings
- crash handling: signals on UNIX platforms or structured exceptions on Windows
- support for tags
    - may fail tag
    - invisible tag
    - look at Catch - https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md#special-tags
- output to file
- reporters
    - a system for writing custom reporters
    - ability to use multiple reporters at once (but only 1 to stdout)
    - a compact reporter
    - an xml reporter
    - jUnit/xUnit reporters
- add the ability to query if code is currently being ran in a test -  ```doctest::isRunningInTest()```
- convolution support for the assertion macros (with a predicate)
- time stuff
    - reporting running time of tests
    - restricting duration of test cases
    - killing a test that exceeds a time limit (will perhaps require threading or processes)
- adding contextual info to asserts (logging) - with an ```INFO```/```CONTEXT``` /```TRACEPOINT``` macro
- add ```ERROR```/```FAIL``` macros
- running tests a few times
- marking a test to run X times (should also multiply with the global test run times)
- test execution in separate processes - ```fork()``` for UNIX and [this](https://github.com/nemequ/munit/issues/2) for Windows
- ability to provide a temp folder that is cleared between each test case
- detect floating point exceptions
- ```Bitwise()``` class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- integrate static analysis on the CI: **msvc**, **clang**, **cppcheck**

And here is a list of things that are being considered but not part of the roadmap yet:

- look into MSTest integration
    - http://accu.org/index.php/journals/1851
    - https://msdn.microsoft.com/en-us/library/hh270865.aspx
    - also look into similar Xcode integration
- matchers - should investigate what they are :D
- generators? - look at Catch - and investigate what they are (also in [boost](http://www.boost.org/doc/libs/1_61_0/libs/test/doc/html/boost_test/tests_organization/test_cases/test_case_generation.html)
- option to list files in which there are test cases who match the current filters
- handle ```wchar``` strings
- explore ```extern template``` from C++11 - maybe some templates can be externed - for basic types like int/char/float...
- a progress reporter
- doctest in a GUI environment? with no console? APIs for attaching a console? querying if there is one? should investigate...
- ability to customize the colors in the console output
- pool allocator for the ```String``` class - currently very unoptimized
- ability to specify ASC/DESC for the order option
- drop support for VC6 - _MSC_VER == 1400 for VS 2005
- command line error handling/reporting
- utf8? not sure if this should be here
- print a warning when no assertion is encountered in a test case
- hierarchical test suites - using a stack for the pushed ones - should be easy
- put everything from the ```detail``` namespace also in a nested anonymous namespace to make them with internal linkage
- ability to re-run only newly compiled tests based on time stamps using ```__DATE__``` and ```__TIME__``` - stored in some file
- submit to [boost](http://www.boost.org/development/requirements.html)?
- fix all the hacks in the cmake scripts for OSX (they are mainly for gcc 4.4)
- the [range_based_execution](../../examples/range_based_execution/) example sometimes leaves one subprocess running and hangs ctest - should fix probably the python script

---------------

[Home](readme.md#reference)
