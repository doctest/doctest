## Assertion macros

Most test frameworks have a large collection of assertion macros to capture all possible conditional forms (```_EQUALS```, ```_NOTEQUALS```, ```_GREATER_THAN``` etc).

**doctest** is different (but it's like [**Catch**](https://github.com/philsquared/Catch) in this regard). Because it decomposes natural C-style conditional expressions most of these forms are reduced to one or two that you will use all the time. That said there are a rich set of auxiliary macros as well. We'll describe all of these here.

Most of these macros come in two forms:

## Natural Expressions

The ```REQUIRE``` family of macros tests an expression and aborts the test case if it fails.

The ```CHECK``` family are equivalent but execution continues in the same test case even if the assertion fails. This is useful if you have a series of essentially orthogonal assertions and it is useful to see all the results rather than stopping at the first failure.

The ```WARN``` family will just print the error when the condition is not met - but will not fail the test case. 

* **REQUIRE(** _expression_ **)**
* **CHECK(** _expression_ **)**  
* **WARN(** _expression_ **)**

Evaluates the expression and records the result. If an exception is thrown it is caught, reported, and counted as a failure (unless it is a **WARN**). These are the macros you will use most of  the time

Examples:

```c++
CHECK(str == "string value");
CHECK(thisReturnsTrue());
REQUIRE(i == 42);
```

* **REQUIRE_FALSE(** _expression_ **)**
* **CHECK_FALSE(** _expression_ **)**
* **WARN_FALSE(** _expression_ **)**

Evaluates the expression and records the _logical NOT_ of the result. If an exception is thrown it is caught, reported, and counted as a failure.
(these forms exist as a workaround for the fact that ! prefixed expressions cannot be decomposed).

Example:

```c++
REQUIRE_FALSE(thisReturnsFalse());
```

### Floating point comparisons

When comparing floating point numbers - especially if at least one of them has been computed - great care must be taken to allow for rounding errors and inexact representations.

**doctest** provides a way to perform tolerant comparisons of floating point values through use of a wrapper class called ```doctest::Approx```. ```doctest::Approx``` can be used on either side of a comparison expression. It overloads the comparisons operators to take a tolerance into account. Here's a simple example:

```c++
REQUIRE(performComputation() == doctest::Approx(2.1));
```

By default a small epsilon value is used that covers many simple cases of rounding errors. When this is insufficient the epsilon value (the amount within which a difference either way is ignored) can be specified by calling the ```epsilon()``` method on the ```doctest::Approx``` instance. e.g.:

```c++
REQUIRE(22/7 == doctest::Approx(3.141).epsilon(0.01));
```

When dealing with very large or very small numbers it can be useful to specify a scale, which can be achieved by calling the ```scale()``` method on the ```doctest::Approx``` instance.

## Exceptions

* **REQUIRE_THROWS(** _expression_ **)**
* **CHECK_THROWS(** _expression_ **)**
* **WARN_THROWS(** _expression_ **)**

Expects that an exception (of any type) is be thrown during evaluation of the expression.

* **REQUIRE_THROWS_AS(** _expression_, _exception type_ **)**
* **CHECK_THROWS_AS(** _expression_, _exception type_ **)**
* **WARN_THROWS_AS(** _expression_, _exception type_ **)**

Expects that an exception of the _specified type_ is thrown during evaluation of the expression.

* **REQUIRE_NOTHROW(** _expression_ **)**
* **CHECK_NOTHROW(** _expression_ **)**
* **WARN_NOTHROW(** _expression_ **)**

Expects that no exception is thrown during evaluation of the expression.

--------

- 
- Do not wrap assertion macros in ```try```/```catch``` - the REQUIRE macros throw exceptions to end the test case execution!

---------------

[Home](readme.md#reference)
