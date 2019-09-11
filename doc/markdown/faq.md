## FAQ

- [**How is doctest different from Catch?**](#how-is-doctest-different-from-catch)
- [**How is doctest different from Google Test?**](#how-is-doctest-different-from-google-test)
- [**How to get the best compile-time performance with the framework?**](#how-to-get-the-best-compile-time-performance-with-the-framework)
- [**Is doctest thread-aware?**](#is-doctest-thread-aware)
- [**Is mocking supported?**](#is-mocking-supported)
- [**Why are my tests in a static library not getting registered?**](#why-are-my-tests-in-a-static-library-not-getting-registered)
- [**Why is comparing C strings (```char*```) actually comparing pointers?**](#why-is-comparing-c-strings-char-actually-comparing-pointers)
- [**How to write tests in header-only libraries?**](#how-to-write-tests-in-header-only-libraries)
- [**Does the framework use exceptions?**](#does-the-framework-use-exceptions)
- [**Why do I get compiler errors in STL headers when including the doctest header?**](#why-do-i-get-compiler-errors-in-stl-headers-when-including-the-doctest-header)
- [**Can different versions of the framework be used within the same binary (executable/dll)?**](#can-different-versions-of-the-framework-be-used-within-the-same-binary-executabledll)
- [**Why is doctest using macros?**](#why-is-doctest-using-macros)

### How is **doctest** different from Catch?

Pros of **doctest**:

- **doctest** is [**thread-safe**](faq.md#is-doctest-thread-aware)
- asserts can be used [**outside of a testing context**](assertions.md#using-asserts-out-of-a-testing-context)
- including the **doctest** header is [**over 20 times lighter**](benchmarks.md#cost-of-including-the-header) on compile times than that of [**Catch**](https://github.com/catchorg/Catch2)
- the asserts in **doctest** can be [**many times lighter**](benchmarks.md#cost-of-an-assertion-macro) on compile times than those of [**Catch**](https://github.com/catchorg/Catch2)
- **doctest** executes tests [**many times faster**](benchmarks.md#runtime-benchmarks) than [**Catch**](https://github.com/catchorg/Catch2)
- everything testing-related can be removed from the binary by defining the [**```DOCTEST_CONFIG_DISABLE```**](configuration.md#doctest_config_disable) identifier
- doesn't drag any headers when included (except for in the translation unit where the library gets implemented)
- 0 warnings even on the [**most aggressive**](../../scripts/cmake/common.cmake#L84) warning levels for MSVC/GCC/Clang
- per commit tested with 180+ builds on [**much more compilers**](features.md#extremely-portable) - and through valgrind/sanitizers/analyzers
- test cases can be written in headers - the framework will still register the tests only once - no duplicates
- binaries (exe/dll) can use the test runner of another binary - so tests end up in a single registry - [**example**](../../examples/executable_dll_and_plugin/)

Aside from everything mentioned so far doctest has some [**features**](features.md#other-features) (like [**test suites**](testcases.md#test-suites) and [**decorators**](testcases.md#decorators)) which [**Catch**](https://github.com/catchorg/Catch2) doesn't.

Missing stuff:

- matchers and generators
- micro benchmarking support - nonius is used in [**Catch**](https://github.com/catchorg/Catch2)
- other small stuff

But these things (and more!) are planned in the [**roadmap**](roadmap.md)!

**doctest** can be thought of as a very polished, light, stable and clean subset (or reimplementation) of [**Catch**](https://github.com/catchorg/Catch2) but this might change in the future as more features are added.

Also checkout [this table](https://github.com/martinmoene/catch-lest-other-comparison) that compares **doctest** / [**Catch**](https://github.com/catchorg/Catch2) / [**lest**](https://github.com/martinmoene/lest).

A quick and easy way to migrate most of your Catch tests to doctest is to change the ```TEST_CASE``` (if using tags) and ```SECTION``` macros as follows:

```c++
#include "path/to/doctest.h"

#define SECTION(name) DOCTEST_SUBCASE(name)

// only if tags are used: will concatenate them to the test name string literal
#undef TEST_CASE
#define TEST_CASE(name, tags) DOCTEST_TEST_CASE(tags " " name)

// catch exposes this by default outside of its namespace
using doctest::Approx;
```

### How is **doctest** different from Google Test?

Here are a couple of differences:

- the main one is that only doctest from the C++ frameworks is usable next to your production code (speed of compilation, ability to remove the tests from the binary, ability to execute tests/code/both, ability to have tests in multiple shared objects and still a single registry for all of them)
- doctest is a single header - Google Test has to be built as a separate static library and linked against.
- doctest has the concept of [**Subcases**](https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md#test-cases-and-subcases) which is a much cleaner way to share setup and teardown code between tests compared to fixtures and class inheritance - Google Test is quite verbose!
- doctest compiles faster and probably runs faster (although the runtime becomes an issue only when you have millions of asserts)
- doctest asserts are thread-safe even on Windows (Google Test uses pthreads so thread-safe asserts are available only on UNIX)
- doctest overall has a simpler API

but there are also some areas in which doctest is lacking:

- value-parameterized tests
- death tests (where you check if calling a certain function doesn’t simply throw but if it crashes the process)
- doctest has some integration with mocking libraries but Google Test works perfectly with Google Mock (although doctest should in theory work with it as well)

The areas where doctest is behind are planned for improvement in the future. There are many other smaller differences - it would be impractical to cover them all.

### How to get the best compile-time performance with the framework?

The [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts) config option yields the [**fastest possible**](benchmarks.md#cost-of-an-assertion-macro) compile times (up to 31-91%). Also the expression-decomposing template machinery can be skipped by using the [**binary**](assertions.md#binary-and-unary-asserts) asserts.

There are only 2 tiny drawbacks of using this config option:

- there is no ```try/catch``` block in each assert so if an expression is thrown the whole test case ends (but is still caught and reported).
- when an assert fails and a debugger is present - the framework will break inside a doctest function so the user will have to go 1 level up in the callstack to see where the actual assert is in the source code.

These 2 things can be considered negligible and totally worth it if you are dealing mainly with expressions unlikely to throw exceptions and all the tests usually pass (you don't need to navigate often to a failing assert with a debugger attached).

### Is doctest thread-aware?

Most macros/functionality is safe to use in a multithreaded context: [**assertion**](assertions.md) and [**logging**](logging.md) macros can be safely used from multiple threads spawned from a single test case. This however does not mean that multiple test cases can be ran in parallel - test cases are still ran serially. [**Subcases**](tutorial.md#test-cases-and-subcases) should also be used only from the test runner thread - not following these instructions will lead to crashes (example in [**here**](../../examples/all_features/concurrency.cpp)). Also note that logged context in one thread will not be used/printed when asserts from another thread fail - logged context is thread-local.

There is also an option to run a [**range**](commandline.md) of tests from an executable - so tests can be ran in parallel by invoking the process multiple times with different ranges - see [**the example python script**](../../examples/range_based_execution.py).

### Is mocking supported?

**doctest** doesn't support mocking but should be easy to integrate with third-party libraries such as:

- [trompeloeil](https://github.com/rollbear/trompeloeil) - integration shown [here](https://github.com/rollbear/trompeloeil/blob/master/docs/CookBook.md#adapt_doctest)
- [googlemock](https://github.com/google/googletest/tree/master/googlemock) - for integration check [this](https://github.com/google/googletest/blob/master/googlemock/docs/ForDummies.md#using-google-mock-with-any-testing-framework)
- [FakeIt](https://github.com/eranpeer/FakeIt) - integration might be similar to that of [catch](https://github.com/eranpeer/FakeIt/tree/master/config/catch) but this has not been looked into

by using the [**logging**](logging.md#messages-which-can-optionally-fail-test-cases) macros such as ```ADD_FAIL_AT(file, line, message)```

<!--
Not sure how to integrate with these:
https://github.com/dascandy/hippomocks
https://github.com/tpounds/mockitopp
-->

### Why are my tests in a static library not getting registered?

This is a [**common problem among libraries with self-registering code**](https://groups.google.com/forum/#!msg/catch-forum/FV0Qo62DvgY/jxEO6c9_q3kJ) and it affects all modern compilers on all platforms.

The problem is that when a static library is being linked to a binary (executable or dll) - only object files from the static library that define a symbol being required from the binary will get pulled in (this is a linker/dependency optimization).

A way to solve this in CMake is to use object libraries instead of static libraries - like this:

```cmake
add_library(with_tests OBJECT src_1.cpp src_2.cpp src_3.cpp ...)

add_library(dll SHARED $<TARGET_OBJECTS:with_tests> dll_src_1.cpp ...)
add_executable(exe $<TARGET_OBJECTS:with_tests> exe_src_1.cpp ...)
```

Thanks to [pthom](https://github.com/pthom) for suggesting this.

As an alternative I have created a CMake function that forces every object file from a static library to be linked into a binary target - it is called [**```doctest_force_link_static_lib_in_target()```**](../../examples/exe_with_static_libs/doctest_force_link_static_lib_in_target.cmake). It is unintrusive - no source file gets changed - everything is done with compiler flags per source files. An example project using it can be found [**here**](../../examples/exe_with_static_libs) - the commented part of the CMakeLists.txt file.

It doesn't work in 2 scenarios:

- either the target or the library uses a precompiled header - see [**this**](https://github.com/onqtam/doctest/issues/21#issuecomment-247001423) issue for details
- either the target or the library is an imported target (pre-built) and not built within the current cmake tree

You can also checkout this repository for a different solution: [**pthom/doctest_registerlibrary**](https://github.com/pthom/doctest_registerlibrary).

A compiler-specific solution for MSVC is to use the [```/OPT:NOREF```](https://msdn.microsoft.com/en-us/library/bxwfs976.aspx) linker flag (thanks to [lectem](https://github.com/Lectem) for [reporting](https://github.com/onqtam/doctest/issues/106) it!). Another option is to look at [```/wholearchive```](https://docs.microsoft.com/en-us/cpp/build/reference/wholearchive-include-all-library-object-files?view=vs-2019) for MSVC.

### Why is comparing C strings (```char*```) actually comparing pointers?

**doctest** by default treats ```char*``` as normal pointers. Using the [**```DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING```**](configuration.md#doctest_config_treat_char_star_as_string) changes that.

### How to write tests in header-only libraries?

There are 2 options:

- just include the doctest header in your headers and write the tests - the doctest header should be shipped with your headers and the user will have to implement the doctest runner in one of his source files.
- don't include the doctest header and guard your test cases with ```#ifdef DOCTEST_LIBRARY_INCLUDED``` and ```#endif``` - that way your tests will be compiled and registered if the user includes the doctest header before your headers (and he will also have to implement the test runner somewhere).

Also note that it would be a good idea to add a tag in your test case names (like this: ```TEST_CASE("[the_lib] testing foo")```) so the user can easily filter them out with ```--test-case-exclude=*the_lib*``` if he wishes to.

### Does the framework use exceptions?

Yes - but they can be disabled - see the [**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](configuration.md#doctest_config_no_exceptions) config identifier.

### Why do I get compiler errors in STL headers when including the doctest header?

Try using the [**```DOCTEST_CONFIG_USE_STD_HEADERS```**](configuration.md#doctest_config_use_std_headers) configuration identifier.

### Can different versions of the framework be used within the same binary (executable/dll)?

Currently no. Single header libraries like [**stb**](https://github.com/nothings/stb) have this as an option (everything gets declared static - making it with internal linkage) but it isn't very logical for **doctest** - the main point is to write tests in any source file of the project and have the test runner implemented in only one source file.

### Why is doctest using macros?

Aren't they evil and not *modern*? - Check out the answer Phil Nash gives to this question [**here**](http://accu.org/index.php/journals/2064) (the creator of [**Catch**](https://github.com/catchorg/Catch2)).

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
