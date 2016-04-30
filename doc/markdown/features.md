Features and design goals
=======




###extremely portable and bug free

- c++98
- Debug/Release
- 32/64 bit
- valgrind, address sanitizer, UB sanitizer, coverity (more static analysis coming soon)
- tested with gcc under OSX/Linux
    - 4.4
    - 4.5
    - 4.6
    - 4.7
    - 4.8
    - 4.9
    - 5
- tested with clang under OSX/Linux
    - 3.4
    - 3.5
    - 3.6
    - 3.7
    - 3.8
- tested with Visual Studio
    - VC++6 (yes - that 18 year old compiler from 1998)
    - 2008
    - 2010
    - 2012
    - 2013
    - 2015

###CI

- 180 builds linux/osx
- 18 builds windows

###unintrusive:

- everything testing-related can be removed from the binary executable with a global define
- all macros have prefixes
    - some by default have unprefixed versions as well but that can be turned off
- 0 warnings even with the most aggresive flags
- doesn't leave warnings disabled after itself
- doesn't drag any headers with itself
- everything is in doctest namespace
    - implementation details are in a nested namespace
- command line
    - doesn't error on unrecognized options
    - supports prefixed options that won't clash with user defines ones
- can set options procedurally and not worry about the command line