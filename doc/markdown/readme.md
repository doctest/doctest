Reference
=======


















doctest is a c++ header only library for writing tests that is inspired by the unittest functionality in the D programming language and the docstrings in python - tests are documentation and should reside near the code they test.

The library has automatic test discovery and is c++98 compatible.

Tests are registered with the following macros:

- doctest(test_name) {}
- doctest_noname {}
- doctest_fixture(fixture_class, test_name) {}
- doctest_fixture_noname(fixture_class) {}
- doctest_static_method(class_name, method_name)

Tests can be registered anywhere - from source to header files. Each test is guaranteed to be registered only once no matter how many occurances it has in the code.

To register a static method of a class as a test the user should use the doctest_static_method() macro outside of the class definition.
The method should be of type ```void(*)(void)```. Such tests cannot be named. See the **class_static_method** example.

To invoke all the registered tests call DOCTEST_INVOKE_ALL_TEST_FUNCTIONS() passing the argc and argv of the program like this:
```
DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
```

##Test naming

Tests can be invoked based on filter strings (comma separated on the command line). Names are case sensitive.
Tests registered without a name actually have an empty name ("") and are invoked only when no filters are supplied.

For more information about the command line go [here](command_line.md)

```
#include <iostream>
using namespace std;

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

Fixtures are supported - they are basically a normal class. To have access to the data members of a fixture class they should be public or protected. Each test using a fixture will have it's fixture initialized for it. Fixture classes may also inherit each other to form more complex hierarchies.

```
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

##Notes

- The registration of test functions can be disabled by defining ```DOCTEST_GLOBAL_DISABLE``` before the inclusion of the **doctest.h** header. For large projects with tens of thousands of tests this may reduce the link time of the production build especially if lots of tests are registered in header files and will also reduce the binary size. See **disabled** from the examples folder.

- Tests are registered from top to bottom of each processed cpp after the headers have been preprocessed and included but there is no ordering between cpp files.

- Tests are registered globally within each shared object/executable. If a test is defined in a header and that header is used in an executable and in a shared object, then the test is registered in both places. To see how to invoke tests from a shared object check out **multi_dll** from the examples folder.

- The library does not use operator new/delete (only malloc) so it's memory usage is completely transparent to the user and that makes it fit for testing even memory management.

- For tests written on the same line only the first one will be registered successfully.

- The only non-standard features used in this library are:

  - the **__COUNTER__** macro, but all major compilers support it (if it is not found, **__LINE__** is used and then there are some corner cases with the macro codegen and a test may be registered twice)

  - the **pragma once** directive - supporting the DRY principle