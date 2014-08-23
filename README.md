doctest
=======

doctest is a testing framework for c++ that is inspired by the unittest functionality in the D programming language and the docstrings in python - tests are documentation and should reside near the code they test.

The library has automatic test discovery and is c++98 compatible.

Tests are registered with the following macros:

- doctest(test_name)
- doctest_noname
- doctest_fixture(fixture_class, test_name)
- doctest_fixture_noname(fixture_class)
- doctest_static_method(class_name, method_name)

Tests can be registered anywhere - from source to header files. Each test is guaranteed to be registered only once no matter how many occurances it has in the code.

##Test naming

Tests can be invoked based on filter strings (comma separated on the command line).
Tests registered without a name actually have an empty name ("") and are invoked only when no filters are supplied.

```C++
doctest(Test1)      { cout << "Test1" << endl; }
doctest(Test2)      { cout << "Test2" << endl; }
doctest(Test3)      { cout << "Test3" << endl; }
doctest(Test_asd)   { cout << "Test_asd" << endl; }
doctest(Test_asd2)  { cout << "Test_asd2" << endl; }
doctest(Test_asd2)  { cout << "Test_asd2 (copy)" << endl; }
doctest(Test_asd2)  { cout << "Test_asd2 (copy 2)" << endl; }
```

For the above tests when we call the program with these args

```
./exe -doctest=asd2,Test2
```

the output will be this:

```
Test_asd2
Test_asd2 (copy)
Test_asd2 (copy 2)
Test2
```

##Fixtures

Fixtures are supported - register with macros doctest_fixture() and doctest_fixture_noname() like this:

```C++
#include <iostream>
using namespace std;

struct Shared {
    Shared() : a(5) { cout << "hello! I am a fixture ctor!" << endl; }
    ~Shared() { cout << "dtor-ing...!" << endl; }
    int a;
};

doctest_fixture(Shared, Fixture1_name) {
    cout << a << endl;
    a = 6;
    cout << a << endl;
}

doctest_fixture(Shared, Fixture2_name) {
    cout << a << endl;
    a = 8;
    cout << a << endl;
}

doctest_fixture_noname(Shared) {
    cout << a << endl;
    a = 2;
    cout << a << endl;
}
```

For the above tests the output will be this:

```
hello! I am a fixture ctor!
5
6
dtor-ing...!
hello! I am a fixture ctor!
5
8
dtor-ing...!
hello! I am a fixture ctor!
5
2
dtor-ing...!
```

##Non-standard dependencies

The only non-standard features used in this library are:

- the **__COUNTER__** macro, but all major compilers support it (if it is not found, **__LINE__** is used and then there are some corner cases with the macro codegen and a test may be registered twice)

- the **pragma once** directive - supporting the DRY principle

##TODO

- asserts

- reporting

- documentation

##Workflow with examples

The requirements for the examples are to have CMake 2.8 or higher.

####Under Linux

To build an example navigate to its folder and enter

```
cmake ./
make
./exe
```

####Under Windows

You will need **MinGW w64** installed for Windows (or MSVC 2013 - hardcoded in the .bat files)

You can get **MinGW w64** from http://sourceforge.net/projects/mingwbuilds/ (arch: "x64" threads: "posix" exceptions: "seh")

Use the bat files in the examples folder to build (or atleast configure) each example - place them in the sub folder of an example and run them

- **g.bat** - generates build files through cmake

- **c.bat** - invokes mingw32-make on the makefile target

- **r.bat** - runs the exe
