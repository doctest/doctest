### The lightest feature-rich C++ single-header testing framework for unit tests and TDD

---------

The **doctest** library is inspired by the [**```unittest {}```**](https://wiki.dlang.org/Unittest) functionality of the **D** programming language and **Python**'s [**docstrings**](https://en.wikipedia.org/wiki/Docstring) - tests can be considered a form of documentation and should be able to reside near the production code which they test.

A complete example with a self-registering test that compiles to an executable looks like this:

![cover-example](doc/data/cover_888px_wide.png)

Note how a standard C++ operator for equality comparison is used - **doctest** has one core [**assertion macro**](doc/markdown/assertions.md) (it also has for *less than*, *equals*, *greater than*...) - yet the full expression is decomposed and the left and right values are logged.

It is modeled after [**Catch**](https://github.com/philsquared/Catch) which is currently the most popular alternative for testing in C++ - check out [**the differences**](doc/markdown/faq.md).

---------

There are many C++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_60_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest) and many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

What makes **doctest** different is that it is ultra light on compile times (by [**orders of magnitude**](doc/markdown/benchmarks.md#cost-of-including-the-header)) and is unintrusive.

The **key** differences between it and other testing libraries are:
- Ultra light - [**below 10ms**](doc/markdown/benchmarks.md#cost-of-including-the-header) of compile time overhead for including the header in a source file
- The [**fastest possible**](doc/markdown/benchmarks.md#cost-of-an-assertion-macro) assertion macros - 50k asserts can compile for under 30 seconds (even under 10 sec)
- [**Subcases**](doc/markdown/tutorial.md#test-cases-and-subcases) - an intuitive way to share common setup and teardown code for test cases
- Offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](doc/markdown/configuration.md#doctest_config_disable) identifier
- Doesn't pollute the global namespace (everything is in the ```doctest``` namespace) and doesn't drag **any** headers with it
- Doesn't produce any warnings even on the [**most aggressive**](scripts/common.cmake#L71) warning levels for **MSVC**/**GCC**/**Clang**
- Very [**portable and well tested**](doc/markdown/features.md#extremely-portable) C++98 - per commit tested on CI with over 220 different builds (valgrind, sanitizers...)
- Just one header and no external dependencies apart from the C/C++ standard library

This allows the library to be used in more ways than any other - tests can be written directly in the production code!

- This makes the barrier for writing tests **much lower** - you don't have to: **1.** make a separate source file **2.** include a bunch of stuff in it **3.** add it to the build system and **4.** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of it's source file - or even header file!
- Tests in the production code can be thought of as documentation or up-to-date comments - showing how an API is used
- Tests can be shipped to the customer with the software to diagnose a bug faster
- [**Test-driven development**](https://en.wikipedia.org/wiki/Test-driven_development) in C++ has never been easier!

The library can be used like any other if you don't like the idea of mixing production code and tests - check out the [**features**](doc/markdown/features.md)

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-98/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://badge.fury.io/gh/onqtam%2Fdoctest.svg)](https://github.com/onqtam/doctest/releases)
[![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)
[![Join the chat at https://gitter.im/onqtam/doctest](https://badges.gitter.im/onqtam/doctest.svg)](https://gitter.im/onqtam/doctest?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](http://melpon.org/wandbox/permlink/ANJXAwv3UqNCe9zy)

| master branch  |   Linux/OSX   [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=master)](https://travis-ci.org/onqtam/doctest) |   Windows   [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/master?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/master) | [![Coverage Status](https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=master)](https://coveralls.io/github/onqtam/doctest?branch=master) | [![Static Analysis](https://scan.coverity.com/projects/7865/badge.svg)](https://scan.coverity.com/projects/onqtam-doctest) |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| **dev branch** | **Linux/OSX** [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=dev)](https://travis-ci.org/onqtam/doctest)    | **Windows** [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/dev?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/dev)       | [![Coverage Status](https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=dev)](https://coveralls.io/github/onqtam/doctest?branch=dev)       |  |

Contributing
------------

Support the development of the project with donations! There is a list of planned features which are all important and big - see the [**roadmap**](doc/markdown/roadmap.md). I took a break from working in the industry to make open source software so every cent is a big deal.

If you work for a company using doctest or have the means to do so, please consider financial support.

[![Pledgie](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)
[![Patreon](https://cloud.githubusercontent.com/assets/8225057/5990484/70413560-a9ab-11e4-8942-1a63607c0b00.png)](http://www.patreon.com/onqtam)
[![PayPal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3K423Q6TK48BN)

Contributions in the form of issues and pull requests are welcome as well - check out the [**Contributing**](doc/markdown/contributing.md) page.


Documentation
-------------

- [Features and design goals](doc/markdown/features.md) - the complete list of features
- [Roadmap](doc/markdown/roadmap.md) - upcoming features
- [Benchmarks](doc/markdown/benchmarks.md) - compile-time supremacy
- [Contributing](doc/markdown/contributing.md)
- [Examples](examples)

-------------

- [Tutorial](doc/markdown/tutorial.md) - make sure you have read it before the other parts of the documentation
- [Assertion macros](doc/markdown/assertions.md)
- [Test cases, subcases and test fixtures](doc/markdown/testcases.md)
- [Command line](doc/markdown/commandline.md)
- [```main()``` entry point](doc/markdown/main.md)
- [Configuration](doc/markdown/configuration.md)
- [String conversions](doc/markdown/stringification.md)
- [FAQ](doc/markdown/faq.md)

Sponsors
--------

### Rockstar sponsors ( >= 1000$ )

### Gold sponsors ( >= 100$ )

- Pascal Thomet

### Silver sponsors ( >= 20$ )

- Manuel Freiberger

### Bronze sponsors ( >= 5$ )

- Zahari Karadzhov
- Sébastien Feldis
- Mario Kostadinov
