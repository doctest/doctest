the lightest feature rich C++ single header testing framework
-------

The **doctest** library is inspired by the ```unittest {}``` functionality of the **D** programming language and **Python**'s **docstrings** - tests can be considered a form of documentation and _should_ be able to reside near the production code which they test.

It is heavily influenced by [**Catch**](https://github.com/philsquared/Catch) which is currently the most popular, simple and user friendly alternative out there.


- Check out [**features and design goals**](doc/markdown/features.md#why-does-this-library-exist-and-how-is-it-different-from-all-the-rest) to see why this library exists and how it is different from all the rest.
- Check out the tutorial [**here**](doc/markdown/tutorial.md#tutorial).
- [![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](http://melpon.org/wandbox/permlink/xvF0y5DTzIDLN98f)
- [![documentation](https://img.shields.io/badge/documentation-online%20%20%20%20%20-blue.svg)](doc/markdown/readme.md#reference)
- [![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-98-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Version](https://badge.fury.io/gh/onqtam%2Fdoctest.svg)](https://github.com/onqtam/doctest/releases)
[![Join the chat at https://gitter.im/onqtam/doctest](https://badges.gitter.im/onqtam/doctest.svg)](https://gitter.im/onqtam/doctest?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Build status
------------

| Service               | branch: master | branch: dev |
|-----------------------|----------------|-------------|
| Travis-CI (Linux/OSX) | [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=master)](https://travis-ci.org/onqtam/doctest)| [![Linux/OSX Status](https://travis-ci.org/onqtam/doctest.svg?branch=dev)](https://travis-ci.org/onqtam/doctest)|
| Appveyor (Windows)    | [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/master?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/master)| [![Windows status](https://ci.appveyor.com/api/projects/status/j89qxtahyw1dp4gd/branch/dev?svg=true)](https://ci.appveyor.com/project/onqtam/doctest/branch/dev)|
| Unit Test Coverage    | [![Coverage Status](https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=master)](https://coveralls.io/github/onqtam/doctest?branch=master)|[![Coverage Status](https://coveralls.io/repos/github/onqtam/doctest/badge.svg?branch=dev)](https://coveralls.io/github/onqtam/doctest?branch=dev)|
| Static Analysis       | [![Static Analysis](https://scan.coverity.com/projects/7865/badge.svg)](https://scan.coverity.com/projects/onqtam-doctest)|   |

Contributing
------------

Support the development of the project with donations! There is a list of planned features which are all important and big - see the [**roadmap**](features.md#roadmap). I work on this project in my spare time and every cent is a big deal. I took a break from working in the industry to make open source software.

[![Donate to support](https://pledgie.com/campaigns/31280.png)](https://pledgie.com/campaigns/31280)

Contributions in the form of issues and pull requests are welcome as well.

Open an issue for a discussion before making a pull request to make sure the contribution goes smoothly. 

This framework has some design goals which must be kept. Make sure you have read the [**features and design goals**](doc/markdown/features.md#features-and-design-goals) page.

The ```master``` branch is the stable one with the latest release and the ```dev``` branch is on the bleeding edge.

All the framework tests have their output collected when the CMake ```TEST_MODE``` variable is set to ```COLLECT``` (making the new reference output) and later the tests are ran on the CI services (```travis``` and ```appveyor```) - their output is compared with the current reference output in the repository with the ```COMPARE``` mode (default mode is ```NORMAL```).

Code should be formatted with a recent-enough ```clang-format``` using the config file in the root of the repo (or I will do it...)

Testing with compilers different from GCC/Clang/MSVC (and more platforms) is something the project would benefit from.
