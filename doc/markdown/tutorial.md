## Tutorial

To get started with doctest all you need is to download the [**latest version**](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h) which is just a single header and include it in your source files (or add this repository as a git submodule).

This tutorial assumes you can use the header directly: ```#include "doctest.h"``` - so it is either in the same folder with your test source files or you have set up the include paths to it in your build system properly. 

[TDD](https://en.wikipedia.org/wiki/Test-driven_development) is not discussed in this tutorial.

## A simple example

Suppose we have a ```factorial()``` function that we want to test:

```
int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }
```

A complete compiling example with a self-registering test looks like this:

```
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TESTCASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}
```

This will compile to a complete executable which responds to command line arguments. If you just run it with no arguments it will execute all test cases (in this case - just one), report any failures, report a summary of how many tests passed and failed and returns 0 on success and 1 if anything failed (useful if you just want a yes/no answer to: "did it work").

If you run this as written it will pass. Everything is good. Right? Well, there is still a bug here. We missed to check if ```factorial(0) == 1``` so lets add that check as well:

```
TESTCASE("testing the factorial function") {
    CHECK(factorial(0) == 1);
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}
```

Now we get a failure - something like:








The [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](configuration.md) macro tells the library to **1.** implement itself and **2.** to provide a ```main()``` function.

This has to be done in exactly one translation unit (source file) of the project (executable/shared object) 

If you want to provide your own ```main()```  - to integrate the library in your production code or to do something different - then the  [**```DOCTEST_CONFIG_IMPLEMENT```**](configuration.md) macro should be used instead - check out the [**documentation**](main.md) on that topic.




























---------------

[Home](readme.md#reference)






























