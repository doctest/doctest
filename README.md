doctest
=======

doctest is a testing framework for c++ that is inspired by the unittest functionality in the D programming language and the docstrings in python - tests are documentation and should reside near the code they test











c++ only library for embedded tests in your source

can register standalone functions and classes with a `static void check(void)` method

test functions are registered in an unspecified order

test functions can be defined from multiple places and will actually be registered only once

test functions can be registered from multiple dlls or static libraries

can use blah blah macros for TEST_IF_EQUAL and other stuff like that

can have different categories of tests and run them conditionally




