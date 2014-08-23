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

The requirements for the examples are to have CMake 2.8 or higher.

##Workflow under Linux

To build an example navigate to its folder and enter

```
cmake ./
make
./exe
```

##Workflow under Windows

You will need **MinGW w64** installed for Windows (or MSVC 2013 - hardcoded in the .bat files)

You can get **MinGW w64** from http://sourceforge.net/projects/mingwbuilds/ (arch: "x64" threads: "posix" exceptions: "seh")

Use the bat files in the examples folder to build (or atleast configure) each example - place them in the sub folder of an example and run them

- **g.bat** - generates build files through cmake

- **c.bat** - invokes mingw32-make on the makefile target

- **r.bat** - runs the exe
