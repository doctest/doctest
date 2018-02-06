<h3>The fastest feature-rich C++98/C++11 single-header testing framework for unit tests and TDD</h3>

<b>
<table>
    <tr>
        <td>
            master branch
        </td>
        <td>
            Linux/OSX <a href="https://travis-ci.org/onqtam/doctest"><img src="https://travis-ci.org/onqtam/doctest.svg?branch=master"></a>
        </td>
        <td>
            Windows <a href="https://ci.appveyor.com/project/onqtam/doctest/branch/master"><img src="https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/master?svg=true"></a>
        </td>
        <td>
            <a href="https://coveralls.io/github/onqtam/doctest?branch=master"><img src="https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=master"></a>
        </td>
        <td>
            <a href="https://scan.coverity.com/projects/onqtam-doctest"><img src="https://scan.coverity.com/projects/7865/badge.svg"></a>
        </td>
    </tr>
    <tr>
        <td>
            dev branch
        </td>
        <td>
            Linux/OSX <a href="https://travis-ci.org/onqtam/doctest"><img src="https://travis-ci.org/onqtam/doctest.svg?branch=dev"></a>
        </td>
        <td>
            Windows <a href="https://ci.appveyor.com/project/onqtam/doctest/branch/dev"><img src="https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/dev?svg=true"></a>
        </td>
        <td>
            <a href="https://coveralls.io/github/onqtam/doctest?branch=dev"><img src="https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=dev"></a>
        </td>
        <td>
        </td>
    </tr>
</table>
</b>

**doctest** is a new C++ testing framework but is by far the fastest both in compile times (by [**orders of magnitude**](doc/markdown/benchmarks.md)) and runtime compared to other feature-rich alternatives. It brings the ability of compiled languages such as [**D**](https://dlang.org/spec/unittest.html) / [**Rust**](https://doc.rust-lang.org/book/testing.html) / [**Nim**](https://nim-lang.org/docs/unittest.html) to have tests written directly in the production code by providing a fast, transparent and flexible test runner with a clean interface.

[<img src="https://cloud.githubusercontent.com/assets/8225057/5990484/70413560-a9ab-11e4-8942-1a63607c0b00.png" align="right">](http://www.patreon.com/onqtam)

The framework is and will stay free but needs your support to sustain its development. There are lots of <a href="doc/markdown/roadmap.md"><b>new features</b></a> and maintenance to do. If you work for a company using **doctest** or have the means to do so, please consider financial support. Monthly donations via Patreon and one-offs via PayPal.

[<img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" align="right">](https://www.paypal.me/onqtam/10)

A complete example with a self-registering test that compiles to an executable looks like this:

![cover-example](scripts/data/using_doctest_888px_wide.gif)

There are many C++ testing frameworks - [Catch](https://github.com/philsquared/Catch), [Boost.Test](http://www.boost.org/doc/libs/1_64_0/libs/test/doc/html/index.html), [UnitTest++](https://github.com/unittest-cpp/unittest-cpp), [cpputest](https://github.com/cpputest/cpputest), [googletest](https://github.com/google/googletest) and many [other](https://en.wikipedia.org/wiki/List_of_unit_testing_frameworks#C.2B.2B).

The **key** differences between it and other testing frameworks are that it is light and unintrusive:
- Ultra light on compile times both in terms of [**including the header**](doc/markdown/benchmarks.md#cost-of-including-the-header) and writing [**thousands of asserts**](doc/markdown/benchmarks.md#cost-of-an-assertion-macro)
- Doesn't produce any warnings even on the [**most aggressive**](scripts/cmake/common.cmake#L84) warning levels for **MSVC**/**GCC**/**Clang**
- Offers a way to remove **everything** testing-related from the binary with the [**```DOCTEST_CONFIG_DISABLE```**](doc/markdown/configuration.md#doctest_config_disable) identifier
- Doesn't pollute the global namespace (everything is in namespace ```doctest```) and doesn't drag **any** headers with it
- Very [**portable**](doc/markdown/features.md#extremely-portable) C++98 - per commit tested on CI with over 330 different builds (static analysis, sanitizers...)

![cost-of-including-the-framework-header](scripts/data/benchmarks/header.png)

This allows the framework to be used in more ways than any other - tests can be written directly in the production code!

Mantra: *Tests can be considered a form of documentation and should be able to reside near the production code which they test.*

- This makes the barrier for writing tests **much lower** - you don't have to: **1)** make a separate source file **2)** include a bunch of stuff in it **3)** add it to the build system and **4)** add it to source control - You can just write the tests for a class or a piece of functionality at the bottom of its source file - or even header file!
- Tests in the production code can be thought of as documentation or up-to-date comments - showing how an API is used
- Testing internals that are not exposed through the public API and headers is no longer a mind-bending exercise
- [**Test-driven development**](https://en.wikipedia.org/wiki/Test-driven_development) in C++ has never been easier!

The framework can be used like any other if you don't want/need to mix production code and tests - check out the [**features**](doc/markdown/features.md).

**doctest** is modeled after [**Catch**](https://github.com/philsquared/Catch) and some parts of the code have been taken directly - check out [**the differences**](doc/markdown/faq.md#how-is-doctest-different-from-catch).

[This table](https://github.com/martinmoene/catch-lest-other-comparison) compares **doctest** / [**Catch**](https://github.com/philsquared/Catch) / [**lest**](https://github.com/martinmoene/lest) which are all very similar.

[![Standard](https://img.shields.io/badge/c%2B%2B-98/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://badge.fury.io/gh/onqtam%2Fdoctest.svg)](https://github.com/onqtam/doctest/releases)
[![download](https://img.shields.io/badge/download%20%20-latest-blue.svg)](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/503/badge)](https://bestpractices.coreinfrastructure.org/projects/503)
[![Join the chat at https://gitter.im/onqtam/doctest](https://badges.gitter.im/onqtam/doctest.svg)](https://gitter.im/onqtam/doctest?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](https://wandbox.org/permlink/YSid52GI4qMDFvs1)
<!--
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![documentation](https://img.shields.io/badge/documentation%20%20-online-blue.svg)](https://github.com/onqtam/doctest/blob/master/doc/markdown/readme.md#reference)
-->

Checkout the [**CppCon 2017 talk**](https://cppcon2017.sched.com/event/BgsI/mix-tests-and-production-code-with-doctest-implementing-and-using-the-fastest-modern-c-testing-framework) on [**YouTube**](https://www.youtube.com/watch?v=eH1CxEC29l8) to get a better understanding of how the framework works and read about how to use it in [**the article**](https://accu.org/var/uploads/journals/Overload137.pdf) of the february edition of ACCU Overload 2017!

[![CppCon 2017 talk about doctest on youtube](scripts/data/youtube-cppcon-talk-thumbnail.png)](https://www.youtube.com/watch?v=eH1CxEC29l8)

Documentation
-------------

Project:

- [Features and design goals](doc/markdown/features.md) - the complete list of features
- [Roadmap](doc/markdown/roadmap.md) - upcoming features
- [Benchmarks](doc/markdown/benchmarks.md) - compile-time and runtime supremacy
- [Contributing](CONTRIBUTING.md) - how to make a proper pull request
- [Changelog](CHANGELOG.md) - generated changelog based on closed issues/PRs

Usage:

- [Tutorial](doc/markdown/tutorial.md) - make sure you have read it before the other parts of the documentation
- [Assertion macros](doc/markdown/assertions.md)
- [Test cases, subcases and test fixtures](doc/markdown/testcases.md)
- [Parameterized test cases](doc/markdown/parameterized-tests.md)
- [Command line](doc/markdown/commandline.md)
- [Logging macros](doc/markdown/logging.md)
- [```main()``` entry point](doc/markdown/main.md)
- [Configuration](doc/markdown/configuration.md)
- [String conversions](doc/markdown/stringification.md)
- [FAQ](doc/markdown/faq.md)
- [Build systems](doc/markdown/build-systems.md)
- [Examples](examples)

Contributing
------------

[<img src="https://cloud.githubusercontent.com/assets/8225057/5990484/70413560-a9ab-11e4-8942-1a63607c0b00.png" align="right">](http://www.patreon.com/onqtam)

Support the development of the project with donations! There is a list of planned features which are all important and big - see the [**roadmap**](doc/markdown/roadmap.md). I took a break from working in the industry to make open source software so every cent is a big deal.

[<img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" align="right">](https://www.paypal.me/onqtam/10)

If you work for a company using **doctest** or have the means to do so, please consider financial support.

Contributions in the form of issues and pull requests are welcome as well - check out the [**Contributing**](CONTRIBUTING.md) page.


Sponsors
--------

Want to see your name or the name of your company here? Consider donating!

### :zap: Rockstar sponsors :zap:

### :gem: Gold sponsors :gem:

- Pascal Thomet
- Mario Kostadinov

### :cake: Silver sponsors :cake:

- Dan Nissenbaum

### :hamburger: Bronze sponsors :hamburger:

- Sebastien Feldis
- Zahari Karadzhov
