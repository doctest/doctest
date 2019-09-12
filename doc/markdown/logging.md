## Logging macros

Additional messages can be logged during a test case (safely even in [**concurrent threads**](faq.md#is-doctest-thread-aware)).

## INFO()

The ```INFO()``` macro allows heterogeneous sequences of expressions to be streamed using the insertion operator (```<<```) in the same way that ```std::ostream```, ```std::cout```, etc support it.

```c++
INFO("The number is " << i);
```

This message will be relevant to all asserts after it in the current scope or in scopes nested in the current one and will be printed later only if an assert fails.

The expression is **NOT** evaluated right away - instead it gets lazily evaluated only when needed.

Some notes:

- the lazy stringification means the expressions will be evaluated when an assert fails and not at the point of capture - so the value might have changed by then
- refer to the [**stringification**](stringification.md) page for information on how to teach doctest to stringify your types

The lazy evaluation means that in the common case when no asserts fail the code runs super fast. This makes it suitable even in loops - perhaps to log the iteration. 

There is also the **```CAPTURE()```** macro which is a convenience wrapper of **```INFO()```**:

```c++
CAPTURE(some_variable)
```

This will handle the stringification of the variable name for you (actually it works with any expression, not just variables).

This would log something like:

```c++
  some_variable := 42
```

## Messages which can optionally fail test cases

There are a few other macros for logging information:

- ```MESSAGE(message)```
- ```FAIL_CHECK(message)```
- ```FAIL(message)```

```FAIL()``` is like a ```REQUIRE``` assert - fails the test case and exits it. ```FAIL_CHECK()``` acts like a ```CHECK``` assert - fails the test case but continues with the execution. ```MESSAGE()``` just prints a message.

In all these macros the messages are again composed using the ```<<``` streaming operator - like this:

```c++
FAIL("This is not supposed to happen! some var: " << var);
```

Also there is no lazy stringification here - strings are always constructed and printed and thus there are no limitations to the values being logged - temporaries and rvalues are accepted - unlike with the ```INFO()``` macro.

There are also a few more intended for use by third party libraries such as mocking frameworks:

- ```ADD_MESSAGE_AT(file, line, message)```
- ```ADD_FAIL_CHECK_AT(file, line, message)```
- ```ADD_FAIL_AT(file, line, message)```

They can be useful when integrating asserts from a different framework with doctest.

------

- Check out the [**example**](../../examples/all_features/logging.cpp) which shows how all of these are used.

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
