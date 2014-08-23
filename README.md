doctest
=======

doctest is a testing framework for c++ that is inspired by the unittest functionality in the D programming language and the docstrings in python - tests are documentation and should reside near the code they test.

The library has automatic test discovery and is c++98 compatible.

The only non-standard features used in this library are:

- the **__COUNTER__** macro, but all major compilers support it (if it is not found, **__LINE__** is used and then there are some corner cases with the codegen)

- the **pragma once** directive - because of the DRY principle

##TODO

- asserts

- reporting

- documentation

##Current workflow under Windows

The requirements for the examples are to have CMake 2.8 or higher and atleast **MinGW w64** installed for Windows (or MSVC 2013 - hardcoded in the .bat files)

You can get **MinGW w64** from http://sourceforge.net/projects/mingwbuilds/ (arch: "x64" threads: "posix" exceptions: "seh")

- **g.bat** - generates build files through cmake

- **c.bat** - invokes mingw32-make on the makefile target

- **r.bat** - runs the exe
