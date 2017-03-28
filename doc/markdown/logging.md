## Logging macros

Additional messages can be logged during a test case.

## INFO()

The ```INFO()``` macro allows heterogenous sequences of values to be streamed using the insertion operator (```<<```) in the same way that ```std::ostream```, ```std::cout```, etc support it.

```c++
INFO("The number is " << i);
```

This message will be relevant to all asserts after it in the current scope or in scopes nested in the current one and will be printed later only if an assert fails.

Note that there is no initial ```<<``` - instead the insertion sequence is placed in parentheses.

The message is **NOT** constructed right away - instead it gets lazily stringified only when needed. This means that rvalues (temporaries) cannot be passed to the ```INFO()``` macro.

Some notes:

- the lazy stringification means the values will be stringified when an assert fails and not at the point of capture - so the value might have changed
- if the library is built with C++11 rvalue reference support (see [**```DOCTEST_CONFIG_WITH_RVALUE_REFERENCES```**](configuration.md#doctest_config_with_rvalue_references)) then deleted overloads are provided to prohibit rvalues from being captured in an **```INFO()```** call - since the lazy stringification actually caches pointers to the objects. For C++98 temporaries will again not work but there will be horrible compilation errors
- the [**```DOCTEST_CONFIG_NUM_CAPTURES_ON_STACK```**](configuration.md#doctest_config_num_captures_on_stack) config identifier can be used to control how much stack space is used to avoid heap allocations for the streaming macros
- stream manipulators (from ```<iomanip>```) can be used but need to be created as local variables and used as lvalues

The lazy stringification and the stack usage means that in the common case when no asserts fail the code runs super fast. This makes it suitable even in loops - perhaps to log the iteration. 

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

---

[Home](readme.md#reference)
