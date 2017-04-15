## Roadmap

This library is free, and will stay free but needs your support to sustain its development. There are lots of [**new features**](roadmap.md) and maintenance to do. If you work for a company using **doctest** or have the means to do so, please consider financial support.

[![Pledgie](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)
[![Patreon](https://cloud.githubusercontent.com/assets/8225057/5990484/70413560-a9ab-11e4-8942-1a63607c0b00.png)](http://www.patreon.com/onqtam)
[![PayPal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3K423Q6TK48BN)

Planned features for future releases - order changes constantly...

### For 1.2:

- Value-Parameterized test cases - https://github.com/onqtam/doctest/issues/38
- decorators for test cases - like in boost test
    - description
    - disabled
    - shouldFail
    - mayFail
    - alternative mechanism to tags
    - time constraints?
    - run X times (should also multiply with the global test run times)
    - !!! and think about how these will be accessed and filtered from the command line
- time stuff
    - reporting running time of tests
    - count a test case as failed if it exceeds X ms (but no force-killing!)
- runtime performance
    - lazily stringify expressions - only when needed
    - optimize createStream/freeStream to reuse a pool of ostringstream objects
    - get rid of local statics on the hot path - like in getContextState()
    - make a pool allocator for the ```String``` class - currently very unoptimized
    - add move semantics to the ```String``` class
- mocking - interop with doctest (+ maybe send PRs with doctest integration info)
    - google mock
    - https://github.com/eranpeer/FakeIt
    - https://github.com/tpounds/mockitopp
    - https://github.com/rollbear/trompeloeil
    - https://github.com/dascandy/hippomocks
- benchmarking
    - make the bench.py script more usable - with command line arguments
    - redo the compile time ones - also look into CATCH_CONFIG_FAST_COMPILE
    - remove old benchmarks for doctest 1.0
    - add runtime benchmarks
- resolve pull/60 (merge in dev, move cmake folder in scripts, maybe change/remove some of the 2 new SKIP options)
- rework the examples folder - so the test runner is compiled only a few times - CI builds take a ton of time!
- change docs a bit - mainly what is in the landing page (add link to overload)
- address the coverage issue... look at how this project does it: https://github.com/rollbear/trompeloeil
- builds with GCC 7 when it is released (should be in late April) - also by then the precise (ubuntu 12.04) ppa for clang 4.0 should be whitelisted by travis

### For 1.3:

- reporters
    - output to file
    - a system for writing custom reporters
    - ability to use multiple reporters at once (but only 1 to stdout)
    - a compact reporter
    - a progress reporter - or maybe just an option for the console reporter
    - an xml reporter
    - xUnit reporter
    - a listener interface - similar to a reporter - look at Catch
- ability to have no output when everything succeeds
- option to output summary only
- matchers - should investigate what they are - look at google test/mock and Catch (also predicates and boost test)
- convolution support for the assertion macros (with a predicate)
- generators? - look at Catch - and investigate what they are
- look at property based testing
    - [rapidcheck](https://github.com/emil-e/rapidcheck)
    - [autocheck](https://github.com/thejohnfreeman/autocheck)
    - [CppQuickCheck](https://github.com/grogers0/CppQuickCheck)
- IDE integration
    - https://www.reddit.com/r/cpp/comments/65c0f1/run_cpp_unit_tests_from_xcode_and_visual_studio/
    - https://github.com/k-brac/CUTI
    - MSTest
        - http://accu.org/index.php/journals/1851
        - https://msdn.microsoft.com/en-us/library/hh270865.aspx
        - https://msdn.microsoft.com/en-us/library/hh598953.aspx
    - XCode - https://github.com/philsquared/Catch/pull/454
    - CLion
        - https://www.jetbrains.com/clion/features/unit-testing.html
        - https://blog.jetbrains.com/clion/2017/03/clion-2017-1-released/#catch

### For 1.4:

- running tests a [few times](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#repeating-the-tests)
- test execution in [separate processes](https://github.com/philsquared/Catch/issues/853) - ```fork()``` for UNIX and [this](https://github.com/nemequ/munit/issues/2) for Windows
- killing a test that exceeds a time limit (will perhaps require threading or processes)
- [symbolizer](https://github.com/facebook/folly/tree/master/folly/experimental/symbolizer) - for a stack trace - when an assertion fails - and it's in a user function with some deep callstack away from the current test case - how to know the exact code path that lead to the failing assert
- ability to make the framework not capture unexpected exceptions - as requested [here](https://github.com/onqtam/doctest/issues/12#issuecomment-235334585)
- add Approx ability to compare with absolute epsilon - [Catch PR](https://github.com/philsquared/Catch/pull/538)
- ability to customize the colors in the console output (may also use styles - based on [this](https://github.com/agauniyal/rang))
- implement breaking into the debugger under linux - see [here](https://github.com/philsquared/Catch/pull/585) and [here](https://github.com/scottt/debugbreak)

### For 2.0:

- remove C++98 support
    - remove the config identifiers for C++11 features
    - use variadic templates where appropriate
    - update type lists to C++11
    - update traits - use declval, etc.

### Things that are being considered but not part of the roadmap yet:

- when no assertion is encountered in a test case it should fail
- failure reporting should print out previous SECTIONs for data-driven testing - as requested [here](https://github.com/philsquared/Catch/issues/734)
- ```Bitwise()``` class that has overloaded operators for comparison - to be used to check objects bitwise against each other
- detect floating point exceptions
- checkpoint/passpoint - like in [boost test](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/boost_test/test_output/test_tools_support_for_logging/checkpoints.html) (also make all assert/subcase/logging macros to act as passpoints and print the last one on crashes or exceptions)
- log levels - like in [boost test](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/boost_test/utf_reference/rt_param_reference/log_level.html)
- integrate static analysis on the CI: **msvc**, **clang**, **cppcheck**
- queries for the current test case - name (and probably decorators)
- thread safety - asserts/subcases/captures should be safe to be used by multiple threads simultaneously
- support for running tests in parallel in multiple threads
- death tests - as in [google test](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#death-tests)
- command line
    - ability to specify ASC/DESC for the order option
    - command line error handling/reporting
    - ability for the user to extend the command line - as requested [here](https://github.com/philsquared/Catch/issues/622)
    - option to list files in which there are test cases who match the current filters
    - option for filters to switch from "match any" to "match all" mode
    - option to list test suites and test cases in a tree view
    - add a "wait key" option - as requested [here](https://github.com/philsquared/Catch/issues/477#issuecomment-256417686)
- setup / teardown support
    - global setup / teardown - can be currently achieved by providing a custom main function
    - per test suite
    - perhaps for fixtures in addition to the constructor / destructor - since throwing in the destructor might terminate the program
    - or just ignore all of this this - it would require globals or classes and inheritance - and we already have subcases
- doctest in a GUI environment? with no console? APIs for attaching a console? querying if there is one? [investigate...](https://github.com/philsquared/Catch/blob/master/docs/configuration.md#stdout)
- the set holding all registered tests should use a specialized allocator to minimize program startup time
- ability to provide a temp folder that is cleared between each test case
- make the _MESSAGE assert macros work with variadic arguments - and maybe write the ones for binary/unary/fast asserts as well
- move from operator "<<" to "<=" for capturing the left operand when decomposing binary expressions with templates

### Things that are very unlikely to enter the roadmap:

- stop using underscores for the begining of identifiers - the anonymous variables - against the standard...
- templated fixture test cases
- test with missed warning flags for GCC
    - https://github.com/Barro/compiler-warnings
    - http://stackoverflow.com/a/34971392/3162383
- utf8???
- handle ```wchar``` strings???
- hierarchical test suites - using a stack for the pushed ones
- ability to specify the width of the terminal in terms of characters (for example 60 - less than 80 - the default)
- ability to re-run only newly compiled tests based on time stamps using ```__DATE__``` and ```__TIME__``` - stored in some file
- add underscores to all preprocessor identifiers not intended for use by the user
- put everything from the ```detail``` namespace also in a nested anonymous namespace to make them with internal linkage
- ability to put everything from doctest into an anonymous namespace - to allow the use of multiple different versions of **doctest** within the same binary (executable/dll) - like the [**stb**](https://github.com/nothings/stb) libraries can

---------------

[Home](readme.md#reference)
