## Test cases

While **doctest** fully supports the traditional, xUnit, style of class-based fixtures containing test case methods this is not the preferred style.

Instead **doctest** provides a powerful mechanism for nesting subcases within a test case. For a more detailed discussion see the [**tutorial**](tutorial.md#test-cases-and-subcases).

Test cases and subcases are very easy to use in practice:

* **TEST_CASE(** _test name_ **)**
* **SUBCASE(** _subcase name_ **)**

_test name_ and _subcase name_ are free form, quoted, strings. Test names don't have to be unique within the **doctest** executable. They should also be string literals.

For examples see the [Tutorial](tutorial.md)

## BDD-style test cases

In addition to **doctest**'s take on the classic style of test cases, **doctest** supports an alternative syntax that allow tests to be written as "executable specifications" (one of the early goals of [Behaviour Driven Development](http://dannorth.net/introducing-bdd/)). This set of macros map on to ```TEST_CASE```s and ```SUBCASE```s, with a little internal support to make them smoother to work with.

* **SCENARIO(** _scenario name_ **)**

This macro maps onto ```TEST_CASE``` and works in the same way, except that the test case name will be prefixed by "Scenario: "

* **GIVEN(** _something_ **)**
* **WHEN(** _something_ **)**
* **THEN(** _something_ **)**

These macros map onto ```SUBCASE```s except that the subcase names are the _something_s prefixed by "given: ", "when: " or "then: " respectively.

* **AND_WHEN(** _something_ **)**
* **AND_THEN(** _something_ **)**

Similar to ```WHEN``` and ```THEN``` except that the prefixes start with "and ". These are used to chain ```WHEN```s and ```THEN```s together.

When any of these macros are used the console reporter recognises them and formats the test case header such that the Givens, Whens and Thens are aligned to aid readability.

Other than the additional prefixes and the formatting in the console reporter these macros behave exactly as ```TEST_CASE```s and ```SUBCASE```s. As such there is nothing enforcing the correct sequencing of these macros - that's up to the programmer!

## Test fixtures

Although **doctest** allows you to group tests together as subcases within a test case, it can still be convenient, sometimes, to group them using a more traditional test fixture. **doctest** fully supports this too. You define the test fixture as a simple structure:

```c++
class UniqueTestsFixture {
  private:
   static int uniqueID;
  protected:
   DBConnection conn;
  public:
   UniqueTestsFixture() : conn(DBConnection::createConnection("myDB")) {
   }
  protected:
   int getID() {
     return ++uniqueID;
   }
 };

 int UniqueTestsFixture::uniqueID = 0;

 TEST_CASE_FIXTURE(UniqueTestsFixture, "Create Employee/No Name") {
   REQUIRE_THROWS(conn.executeSQL("INSERT INTO employee (id, name) VALUES (?, ?)", getID(), ""));
 }
 TEST_CASE_METHOD(UniqueTestsFixture, "Create Employee/Normal") {
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

------

- Check out the [**subcases and BDD example**](../../examples/subcases_and_bdd/main.cpp)
- Check out the [**assertion macros example**](../../examples/assertion_macros/main.cpp) to see how test suites are used
- Tests are registered from top to bottom of each processed cpp after the headers have been preprocessed and included but there is no ordering between cpp files.

---------------

[Home](readme.md#reference)
