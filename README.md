The lightest feature rich C++ single header testing framework
-------

The **doctest** library is inspired by the [**```unittest {}```**](https://wiki.dlang.org/Unittest) functionality of the **D** programming language and **Python**'s [**docstrings**](https://en.wikipedia.org/wiki/Docstring) - tests can be considered a form of documentation and should be able to reside near the production code which they test.

A complete example with a self-registering test that compiles to an executable looks like this:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(10) == 3628800);
}
```

Note how a standard C++ operator for the comparison is used - **doctest** has only one core [**assertion macro**](assertions.md) (instead of many for *less than*, *equals*, *greater than*...) - yet the full expression is decomposed and the left and right values are logged.

It is modeled after [**Catch**](https://github.com/philsquared/Catch) which is currently the most popular and easy to use alternative for testing in C++

---------

There are many C++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [lest](https://github.com/martinmoene/lest), [bandit](http://banditcpp.org/), [igloo](http://igloo-testing.org/), [xUnit++](https://bitbucket.org/moswald/xunit/wiki/Home), [CppTest](http://cpptest.sourceforge.net/), [CppUnit](https://sourceforge.net/projects/cppunit/), [CxxTest](https://github.com/CxxTest/cxxtest), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest), [cute](https://github.com/Kosta-Github/cute) and many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

What makes **doctest** different is that it is ultra light on compile times (by [**orders of magnitude**](doc/markdown/benchmarks.md)) and is unintrusive.

The **key** differences between it and other testing libraries are:
- Ultra light - [**below 10ms**](doc/markdown/benchmarks.md) of compile time overhead for including the header in a source file
- Offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](doc/markdown/configuration.md) identifier
- Doesn't pollute the global namespace (everything is in the ```doctest``` namespace) and doesn't drag **any** headers with it
- Doesn't produce any warnings even on the [**most aggressive**](scripts/common.cmake#L59) warning levels for **MSVC**/**GCC**/**Clang**
- Very [**portable and well tested**](doc/markdown/features.md#extremely-portable) C++98 - per commit tested on CI with over 200 different builds (valgrind, sanitizers...)
- Just one header and no external dependencies apart from the C/C++ standard library! (well... same as [**Catch**](https://github.com/philsquared/Catch)...)

This allows the library to be used in more ways than any other - tests can be written directly in the production code!

- This makes the barrier for writing tests **much lower** - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of it's source file - or even header file!
- Tests in the production code can be thought of as documentation or up to date comments - showing how an API is used
- Tests can be shipped to the customer with the software to diagnose a bug faster
- [**TDD**](https://en.wikipedia.org/wiki/Test-driven_development) in C++ has never been easier!

The library can be used like any other if you don't like the idea of mixing production code and tests - check out the [**features**](doc/markdown/features.md)

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-98-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://badge.fury.io/gh/onqtam%2Fdoctest.svg)](https://github.com/onqtam/doctest/releases)
[![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)
[![Join the chat at https://gitter.im/onqtam/doctest](https://badges.gitter.im/onqtam/doctest.svg)](https://gitter.im/onqtam/doctest?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](http://melpon.org/wandbox/permlink/xvF0y5DTzIDLN98f)

| master branch | [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=master)](https://travis-ci.org/onqtam/doctest) | [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/master?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/master) | [![Static Analysis](https://scan.coverity.com/projects/7865/badge.svg)](https://scan.coverity.com/projects/onqtam-doctest) |
|----------------|---------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| **dev branch** | [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=dev)](https://travis-ci.org/onqtam/doctest) | [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/dev?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/dev) |  |

Documentation
-------------

- [Features and design goals](doc/markdown/features.md) - the complete list of features
- [Roadmap](doc/markdown/roadmap.md#roadmap) - upcoming features
- [Tutorial](doc/markdown/tutorial.md#tutorial) - make sure you have read it before the other parts of the documentation
- [Benchmarks](doc/markdown/benchmarks.md) - compile-time supremacy
- [Examples](examples)

-------------

- [Assertion macros](doc/markdown/assertions.md)
- [Test cases, subcases and test fixtures](doc/markdown/testcases.md)
- [Command line](doc/markdown/commandline.md)
- [Configuration](doc/markdown/configuration.md)
- [String conversions](doc/markdown/stringification.md)
- [```main()``` entry point and dealing with shared objects](doc/markdown/main.md)
- [FAQ](doc/markdown/faq.md)

Contributing
------------

Support the development of the project with donations! There is a list of planned features which are all important and big - see the [**roadmap**](features.md). I work on this project in my spare time and every cent is a big deal. I took a break from working in the industry to make open source software.

[![Donate to support](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)

Contributions in the form of issues and pull requests are welcome as well.

Open an issue for a discussion before making a pull request to make sure the contribution goes smoothly. 

This framework has some design goals which must be kept. Make sure you have read the [**features and design goals**](doc/markdown/features.md) page.

The ```master``` branch is the stable one with the latest release and the ```dev``` branch is on the bleeding edge.

All the framework tests (using ctest) have their output collected when the CMake ```TEST_MODE``` variable is set to ```COLLECT``` (making the new reference output) and later the tests are ran on the CI services (```travis``` and ```appveyor```) - their output is compared with the current reference output in the repository with the ```COMPARE``` mode (default mode is ```COMPARE```).

Code should be formatted with a recent-enough ```clang-format``` using the config file in the root of the repo (or I will do it...)

Testing with compilers different from GCC/Clang/MSVC (and more platforms) is something the project would benefit from.
