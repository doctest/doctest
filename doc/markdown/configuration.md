## Configuration

**doctest** is designed to "just work" as much as possible. It exposes a small set of defines for configuring how it is built.

For most people the only configuration needed is telling **doctest** which source file should host all the implementation code:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

If the client wants to [**supply the ```main()``` function**](main.md) (either to set an option with some value from the code or to integrate the framework into his existing project codebase) then the **```DOCTEST_CONFIG_IMPLEMENT```** define should be used.

The next most important configuration option is **```DOCTEST_CONFIG_DISABLE```** - when defined globally in the whole project before the inclusion of the framework header - everything testing - related is removed from the binary - including most of the framework implementation and every test case written anywhere! This is one of the most unique features of **doctest**.



Other configuration options are:

- **```DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES```** - this will remove all macros from **doctest** that don't have the **```DOCTEST_```** prefix - like **```CHECK```**, **```TEST_CASE```** and **```SUBCASE```**. Then only the full macro names will be available - **```DOCTEST_CHECK```**, **```DOCTEST_TEST_CASE```** and **```DOCTEST_SUBCASE```**. The user is free to make his own short versions of these macros - [**example**](../../examples/alternative_macros/)

- **```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```** - this will disable the short versions of the [**command line**](commandline.md) options and only the versions with ```-dt-``` prefix will be parsed by **doctest** - this is possible for easy interoperability with client command line option handling when the testing framework is integrated within a client codebase - so there are no clashes and so that the user can exclude everything starting with ```-dt-``` from their option parsing. This configuration option is relevant only for the source file where the library is implemented

- **```DOCTEST_CONFIG_COLORS_NONE```** - this will remove support for colors in the console output of the framework. This configuration option is relevant only for the source file where the library is implemented

- **```DOCTEST_CONFIG_COLORS_WINDOWS```** - this will force the support for colors in the console output to use the Windows APIs and headers. This configuration option is relevant only for the source file where the library is implemented

- **```DOCTEST_CONFIG_COLORS_ANSI```** - this will force the support for colors in the console output to use ANSI escape codes. This configuration option is relevant only for the source file where the library is implemented

- **```DOCTEST_CONFIG_USE_IOSFWD```** - the library by default provides a forward declaration of ```std::ostream``` in order to support the ```operator<<``` [**stringification**](stringification.md) mechanism. This is forbidden by the standard (even though it works everywhere on all tested compilers). However if the user wishes to be 100% standards compliant - then this configuration option can be used to force the inclusion of ```<iosfwd>```. It should be defined everywhere before the framework header is included.

- **```DOCTEST_CONFIG_WITH_LONG_LONG```** - by default the library includes support for stringifying ```long long``` only if the value of ```__cplusplus``` is atleast ```201103L``` (C++11) or if the compiler is MSVC 2003 or newer. Many compilers that don't fully support C++11 have it as an extension but it errors for GCC/Clang when the ```-std=c++98``` option is used and this cannot be detected with the preprocessor in any way. Use this configuration option if your compiler supports ```long long``` but doesn't yet support the full C++11 standard. It should be defined everywhere before the framework header is included.

---------------

[Home](readme.md#reference)
