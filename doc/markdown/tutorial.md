## Tutorial

To get started with **doctest** all you need is to download the [**latest version**](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h) which is just a single header and include it in your source files (or add this repository as a git submodule).

This tutorial assumes you can use the header directly: ```#include "doctest.h"``` - so it is either in the same folder with your test source files or you have set up the include paths to it in your build system properly. 

[TDD](https://en.wikipedia.org/wiki/Test-driven_development) is not discussed in this tutorial.

## A simple example

Suppose we have a ```factorial()``` function that we want to test:

```c++
int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }
```

A complete compiling example with a self-registering test looks like this:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}
```

This will compile to a complete executable which responds to command line arguments. If you just run it with no arguments it will execute all test cases (in this case - just one), report any failures, report a summary of how many tests passed and failed and returns 0 on success and 1 if anything failed (useful if you just want a yes/no answer to: "did it work").

If you run this as written it will pass. Everything is good. Right? Well there is still a bug here. We missed to check if ```factorial(0) == 1``` so lets add that check as well:

```c++
TEST_CASE("testing the factorial function") {
    CHECK(factorial(0) == 1);
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}
```

Now we get a failure - something like:

```
test.cpp(7) FAILED!
  CHECK( factorial(0) == 1 )
with expansion:
  CHECK( 0 == 1 )
```

Note that we get the actual return value of ```factorial(0)``` printed for us (0) - even though we used a natural expression with the ```==``` operator. That lets us immediately see what the problem is.

Let's change the factorial function to:

```c++
int factorial(int number) { return number > 1 ? factorial(number - 1) * number : 1; }
```

Now all the tests pass.

Of course there are still more issues to do deal with. For example we'll hit problems when the return value starts to exceed the range of an int. With factorials that can happen quite quickly. You might want to add tests for such cases and decide how to handle them. We'll stop short of doing that here.

## What did we do here?

Although this was a simple test it's been enough to demonstrate a few things about how **doctest** is used.

1. All we did was ```#define``` one identifier and ```#include``` one header and we got everything - even an implementation of ```main()``` that will respond to command line arguments. You can only use that ```#define``` in one source file for (hopefully) obvious reasons. Once you have more than one file with unit tests in you'll just ```#include "doctest.h"``` and go. Usually it's a good idea to have a dedicated implementation file that just has ```#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN``` and ```#include "doctest.h"```. You can also provide your own implementation of main and drive **doctest** yourself - see [**supplying your own ```main()```**](main.md).
2. We introduce test cases with the ```TEST_CASE``` macro. It takes one argument - a free form test name (for more see [**Test cases and subcases**](testcases.md)). The test name doesn't have to be unique. You can run sets of tests by specifying a wildcarded test name or a tag expression. See the [**command line**](commandline.md) docs for more information on running tests.
3. The name is just a string. We haven't had to declare a function or method - or explicitly register the test case anywhere. Behind the scenes a function with a generated name is defined for you and automatically registered using static registry classes. By abstracting the function name away we can name our tests without the constraints of identifier names.
4. We write our individual test assertions using the ```CHECK()``` macro. Rather than a separate macro for each type of condition (equal, less than, greater than, etc.) we express the condition naturally using C++ syntax. Behind the scenes a simple expression template captures the left-hand-side and right-hand-side of the expression so we can display the values in our test report. There are other [**assertion macros**](assertions.md) not covered in this tutorial - but because of this technique the number of them is drastically reduced.

## Test cases and subcases

Most test frameworks have a class-based fixture mechanism - test cases map to methods on a class and common setup and teardown can be performed in ```setup()``` and ```teardown()``` methods (or constructor/ destructor in languages like C++ that support deterministic destruction).

While **doctest** fully supports this way of working there are a few problems with the approach. In particular the way your code must be split up and the blunt granularity of it may cause problems. You can only have one setup/ teardown pair across a set of methods but sometimes you want slightly different setup in each method or you may even want several levels of setup (a concept which we will clarify later on in this tutorial). It was [**problems like these**](http://jamesnewkirk.typepad.com/posts/2007/09/why-you-should-.html) that led James Newkirk who led the team that built NUnit to start again from scratch and build [**xUnit**](http://jamesnewkirk.typepad.com/posts/2007/09/announcing-xuni.html)).

**doctest** takes a different approach (to both NUnit and xUnit) that is a more natural fit for C++ and the C family of languages.

This is best explained through an example:

```c++
TEST_CASE("vectors can be sized and resized") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SUBCASE("adding to the vector increases it's size") {
        v.push_back(1);

        CHECK(v.size() == 6);
        CHECK(v.capacity() >= 6);
    }
    SUBCASE("reserving increases just the capacity") {
        v.reserve(6);

        CHECK(v.size() == 5);
        CHECK(v.capacity() >= 6);
    }
}
```

For each ```SUBCASE()``` the ```TEST_CASE()``` is executed from the start - so as we enter each subcase we know that the size is 5 and the capacity is at least 5. We enforce those requirements with the ```REQUIRE()``` macros at the top level so we can be confident in them. If a ```CHECK()``` fails - the test is marked as failed but the execution continues - but if a ```REQUIRE()``` fails - execution of the test stops.

This works because the ```SUBCASE()``` macro contains an if statement that calls back into **doctest** to see if the subcase should be executed. One leaf subcase is executed on each run through a ```TEST_CASE()```. The other subcases are skipped. Next time the next subcase is executed and so on until no new subcases are encountered.

So far so good - this is already an improvement on the setup/teardown approach because now we see our setup code inline and use the stack. The power of subcases really shows when we start nesting them like in the example below:

<table><tr><td>
Code
</td><td>
Output
</td></tr><tr><td>
<pre lang="c++">
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
<br>
#include &lt;iostream&gt;
using namespace std;
<br>
TEST_CASE("lots of nested subcases") {
&nbsp;&nbsp;&nbsp;&nbsp;cout << endl << "root" << endl;
&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") {
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cout << "1" << endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "1.1" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;}
&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") {   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cout << "2" << endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "2.1" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") {
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cout << "2.2" << endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") {
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cout << "2.2.1" << endl;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "2.2.1.1" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "2.2.1.2" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "2.3" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SUBCASE("") { cout << "2.4" << endl; }
&nbsp;&nbsp;&nbsp;&nbsp;}
}
</pre>
</td><td width="400">
<pre lang="">
root
1
1.1<br>
root
2
2.1<br>
root
2
2.2
2.2.1
2.2.1.1<br>
root
2
2.2
2.2.1
2.2.1.2<br>
root
2
2.3<br>
root
2
2.4
</pre>
</td></tr></table>

Subcases can be nested to an arbitrary depth (limited only by your stack size). Each leaf subcase (a subcase that contains no nested subcases) will be executed exactly once on a separate path of execution from any other leaf subcase (so no leaf subcase can interfere with another). A fatal failure in a parent subcase will prevent nested subcases from running - but then that's the idea.

Keep in mind that even though **doctest** is [**thread-safe**](faq.md#is-doctest-thread-aware) - using subcases has to be done only in the main test runner thread and all threads spawned in a subcase ought to be joined before the end of that subcase and no new subcases should be entered while other threads with doctest assertions in them are still running.

## Scaling up

To keep the tutorial simple we put all our code in a single file. This is fine to get started - and makes jumping into **doctest** even quicker and easier. This is not really the best approach when you start writing more real-world tests.

The requirement is that the following block of code ([**or equivalent**](main.md)):

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

appears in _exactly_ one translation unit (source file). Use as many additional source files as you need for your tests - partitioned however makes most sense for your way of working. Each additional file needs only to ```#include "doctest.h"``` - do not repeat the ```#define```!

In fact it is usually a good idea to put the block with the ```#define``` in it's own source file.

## Next steps

This has been a brief introduction to get you up and running with **doctest** and to point out some of the key differences between **doctest** and other frameworks you may already be familiar with. This will get you going quite far already and you are now in a position to dive in and write some tests.

Of course there is more to learn - see the ever-growing [**reference**](readme.md#reference) section for what's available.

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
