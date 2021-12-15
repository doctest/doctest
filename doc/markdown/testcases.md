## Test cases

While **doctest** fully supports the traditional, xUnit, style of class-based fixtures containing test case methods this is not the preferred style. Instead **doctest** provides a powerful mechanism for nesting subcases within a test case. For a more detailed discussion and examples see the [**tutorial**](tutorial.md#test-cases-and-subcases).

Test cases and subcases are very easy to use in practice:

* **TEST_CASE(** _test name_ **)**
* **SUBCASE(** _subcase name_ **)**

_test name_ and _subcase name_ are free form, quoted, strings. Test names don't have to be unique within the **doctest** executable. They should also be string literals.

It is possible to write test cases inside of class bodies in C++17 with the help of ```TEST_CASE_CLASS()``` - used just like ```TEST_CASE()``` - making testing private parts of classes easier.

Keep in mind that even though **doctest** is [**thread-safe**](faq.md#is-doctest-thread-aware) - using subcases has to be done only in the main test runner thread.

Test cases can also be parameterized - see the [**documentation**](parameterized-tests.md)

Test cases and subcases can be filtered through the use of the [**command line**](commandline.md)

## BDD-style test cases

In addition to **doctest**'s take on the classic style of test cases, **doctest** supports an alternative syntax that allow tests to be written as "executable specifications" (one of the early goals of [Behaviour Driven Development](http://dannorth.net/introducing-bdd/)). This set of macros map on to ```TEST_CASE```s and ```SUBCASE```s, with a little internal support to make them smoother to work with.

* **SCENARIO(** _scenario name_ **)**

This macro maps onto ```TEST_CASE``` and works in the same way, except that the test case name will be prefixed by "Scenario: "

* **SCENARIO_TEMPLATE(** _scenario name_, _type_, _list of types_ **)**

This macro maps onto ```TEST_CASE_TEMPLATE``` and works in the same way, except that the test case name will be prefixed by "Scenario: "

* **SCENARIO_TEMPLATE_DEFINE(** _scenario name_, _type_, _id_ **)**

This macro maps onto ```TEST_CASE_TEMPLATE_DEFINE``` and works in the same way, except that the test case name will be prefixed by "Scenario: "

* **GIVEN(** _something_ **)**
* **WHEN(** _something_ **)**
* **THEN(** _something_ **)**

These macros map onto ```SUBCASE```s except that the subcase names are the _something_s prefixed by "given: ", "when: " or "then: " respectively.

* **AND_WHEN(** _something_ **)**
* **AND_THEN(** _something_ **)**

Similar to ```WHEN``` and ```THEN``` except that the prefixes start with "and ". These are used to chain ```WHEN```s and ```THEN```s together.

When any of these macros are used the console reporter recognises them and formats the test case header such that the Givens, Whens and Thens are aligned to aid readability.

Other than the additional prefixes and the formatting in the console reporter these macros behave exactly as ```TEST_CASE```s and ```SUBCASE```s. As such there is nothing enforcing the correct sequencing of these macros - that's up to the programmer!

Note that when using the [`--test-case=<filters>`](https://github.com/doctest/doctest/blob/master/doc/markdown/commandline.md) command line option (or `--subcase=<filters>`) you will have to pass the prefix `Scenario: ` as well.

## Test fixtures

Although **doctest** allows you to group tests together as subcases within a test case, it can still be convenient, sometimes, to group them using a more traditional test fixture. **doctest** fully supports this too. You define the test fixture as a simple structure:

```c++
class UniqueTestsFixture {
private:
    static int uniqueID;
protected:
    DBConnection conn;
public:
    UniqueTestsFixture() : conn(DBConnection::createConnection("myDB")) {}
protected:
    int getID() {
        return ++uniqueID;
    }
};

int UniqueTestsFixture::uniqueID = 0;

TEST_CASE_FIXTURE(UniqueTestsFixture, "Create Employee/No Name") {
    REQUIRE_THROWS(conn.executeSQL("INSERT INTO employee (id, name) VALUES (?, ?)", getID(), ""));
}
TEST_CASE_FIXTURE(UniqueTestsFixture, "Create Employee/Normal") {
    REQUIRE(conn.executeSQL("INSERT INTO employee (id, name) VALUES (?, ?)", getID(), "Joe Bloggs"));
}
```

The two test cases here will create uniquely-named derived classes of UniqueTestsFixture and thus can access the `getID()` protected method and `conn` member variables. This ensures that both the test cases are able to create a DBConnection using the same method (DRY principle) and that any ID's created are unique such that the order that tests are executed does not matter.

## Test suites

Test cases can be grouped into test suites. This is done with ```TEST_SUITE()``` or ```TEST_SUITE_BEGIN()``` / ```TEST_SUITE_END()```.

For example:

```c++
TEST_CASE("") {} // not part of any test suite

TEST_SUITE("math") {
    TEST_CASE("") {} // part of the math test suite
    TEST_CASE("") {} // part of the math test suite
}

TEST_SUITE_BEGIN("utils");

TEST_CASE("") {} // part of the utils test suite

TEST_SUITE_END();

TEST_CASE("") {} // not part of any test suite
```

Then test cases from specific test suites can be executed with the help of filters - check out the [**command line**](commandline.md)

## Decorators

Test cases can be *decorated* with additional attributes like this:

```c++
TEST_CASE("name"
          * doctest::description("shouldn't take more than 500ms")
          * doctest::timeout(0.5)) {
    // asserts
}
```

Multiple decorators can be used at the same time. These are the currently supported decorators:

- **```skip(bool = true)```** - marks the test case to be skipped from execution - unless the ```--no-skip``` option is used
- **```no_breaks(bool = true)```** - no breaking into the debugger for asserts in the test case - useful in combination with `may_fail`/`should_fail`/`expected_failures`
- **```no_output(bool = true)```** - no output from asserts in the test case - useful in combination with `may_fail`/`should_fail`/`expected_failures`
- **```may_fail(bool = true)```** - doesn't fail the test if any given assertion fails (but still reports it) - this can be useful to flag a work-in-progress, or a known issue that you don't want to immediately fix but still want to track in the your tests
- **```should_fail(bool = true)```** - like **```may_fail()```** but fails the test if it passes - this can be useful if you want to be notified of accidental, or third-party, fixes
- **```expected_failures(int)```** - defines the number of assertions that are expected to fail within the test case - reported as failure when the number of failed assertions is different than the declared expected number of failures
- **```timeout(double)```** - fails the test case if its execution exceeds this limit (in seconds) - but doesn't terminate it - that would require subprocess support
- **```test_suite("name")```** - can be used on test cases to override (or just set) the test suite they are in
- **```description("text")```** - a description of the test case

The values that the decorators take are computed while registering the test cases (during global initialization) - before entering ```main()``` and not just before running them.

Decorators can also be applied to test suite blocks and all test cases in that block inherit them:

```c++
TEST_SUITE("some TS" * doctest::description("all tests will have this")) {
    TEST_CASE("has a description from the surrounding test suite") {
        // asserts
    }
}
TEST_SUITE("some TS") {
    TEST_CASE("no description even though in the same test suite as the one above") {
        // asserts
    }
}
```

Test cases can override the decorators that they inherit from their surrounding test suite:

```c++
TEST_SUITE("not longer than 500ms" * doctest::timeout(0.5)) {
    TEST_CASE("500ms limit") {
        // asserts
    }
    TEST_CASE("200ms limit" * doctest::timeout(0.2)) {
        // asserts
    }
}
```

------

- Check out the [**subcases and BDD example**](../../examples/all_features/subcases.cpp)
- Check out the [**assertion macros example**](../../examples/all_features/assertion_macros.cpp) to see how test suites are used
- Tests are registered from top to bottom of each processed cpp after the headers have been preprocessed and included but there is no ordering between cpp files.

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
