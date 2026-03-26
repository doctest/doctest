## Parameterized test cases

Test cases can be parameterized easily by type and by value.

## Value-parameterized test cases

Value-parameterized tests are supported via the `GENERATE` macro in `doctest/doctest.h`.

```c++
TEST_CASE("example") {
    auto x = GENERATE(1, 2, 3);  // runs 3 times: x=1, x=2, x=3
    CHECK(x > 0);
}
```

Each additional `GENERATE` call adds a dimension. The last `GENERATE` in source order cycles fastest (innermost loop):

```c++
TEST_CASE("cartesian product") {
    auto x = GENERATE(1, 2);        // outer - 2 values
    auto y = GENERATE(10, 20, 30);  // inner - 3 values
    // 6 runs: (1,10),(1,20),(1,30),(2,10),(2,20),(2,30)
    CHECK(x * y > 0);
}
```

`GENERATE` is generator-outer: each generator combination runs through all subcase paths before the generator advances.

```c++
TEST_CASE("generator + subcases") {
    auto x = GENERATE(1, 2);
    SUBCASE("positive") { CHECK(x > 0); }
    SUBCASE("nonzero")  { CHECK(x != 0); }
    // 4 runs: (x=1,positive),(x=1,nonzero),(x=2,positive),(x=2,nonzero)
}
```

The value type is deduced from the first argument. All subsequent arguments are
implicitly converted to that type.
Use an explicit cast on the first argument to control the type:

```c++
auto x = GENERATE(1.0, 2, 3);    // double - 2 and 3 are implicit cast to double
auto s = GENERATE("foo", "bar"); // const char*
```

User-defined types work as long as they are copyable:

```c++
struct Point { int x, y; };
Point p1{1, 2}, p2{3, 4};
auto p = GENERATE(p1, p2);
```

`GENERATE` may appear inside control flow whose shape is determined by earlier traversal choices on the current path (for example, an outer `GENERATE` or `SUBCASE`).
```c++
int i = GENERATE(1, 2);
for(int j = 0; j < i; ++j) {
    SUBCASE(std::to_string(j).c_str()) {
        int k = GENERATE(10 - i - j, 20 - i - j);
        CHECK(((i + j + k) % 10) == 0);
    }
}
```

When using `SUBCASE` inside a loop, each iteration must have a distinct name. A loop such as:

```c++
for(int j = 0; j < 3; ++j) {
    SUBCASE("fixed name") { /* ... */ }
}
```

is unsupported because each iteration reuses the same subcase identity at the same source location and this leads to unpredictable (but deterministic) behavior.

If the name varies per iteration, the discovered loop subcases behave as siblings at that nesting depth:

```c++
for(int j = 0; j < 3; ++j) {
    SUBCASE(doctest::toString(j)) { /* ... */ }
}
```

This behaves the same as replacing the loop with a `GENERATE` over those sibling choices. As with `GENERATE` in control flow generally, this relies on the loop shape being stable across reruns and determined only by earlier traversal choices on the current path.

Constraints:

- All arguments must be implicitly convertible to the type of the first argument.
- `GENERATE` must not appear inside control flow that varies between reruns independently of `SUBCASE` or earlier `GENERATE` choices (for example, mutable external state, randomness, or time-dependent conditions).

See [`examples/all_features/generators.cpp`](../../examples/all_features/generators.cpp).

## Templated test cases - parameterized by type

Suppose you have multiple implementations of the same interface and want to make sure that all of them satisfy some common requirements. Or, you may have defined several types that are supposed to conform to the same "concept" and you want to verify it. In both cases, you want the same test logic repeated for different types.

While you can write one ```TEST_CASE``` for each type you want to test (and you may even factor the test logic into a function template that you invoke from the test case), it's tedious and doesn't scale: if you want ```M``` tests over ```N``` types, you'll end up writing ```M * N``` tests.

Templated tests allow you to repeat the same test logic over a list of types. You only need to write the test logic once.

There are 2 ways to do it:

- directly pass the list of types to the templated test case

    ```c++
    TEST_CASE_TEMPLATE("signed integers stuff", T, char, short, int, long long int) {
        T var = T();
        --var;
        CHECK(var == -1);
    }
    ```

- define the templated test case with a specific unique name (identifier) for later instantiation

    ```c++
    TEST_CASE_TEMPLATE_DEFINE("signed integer stuff", T, test_id) {
        T var = T();
        --var;
        CHECK(var == -1);
    }

    TEST_CASE_TEMPLATE_INVOKE(test_id, char, short, int, long long int);

    TEST_CASE_TEMPLATE_APPLY(test_id, std::tuple<float, double>);
    ```
    If you are designing an interface or concept, you can define a suite of type-parameterized tests to verify properties that any valid implementation of the interface/concept should have. Then, the author of each implementation can just instantiate the test suite with their type to verify that it conforms to the requirements, without having to write similar tests repeatedly.


A test case named ```signed integers stuff``` instantiated for type ```int``` will yield the following test case name:

```
signed integers stuff<int>
```

By default all primitive types (fundamental - ```int```, ```bool```, ```float```...) have stringification provided by the library. For all other types the user will have to use the ```TYPE_TO_STRING(type)``` macro - like this:

```c++
TYPE_TO_STRING(std::vector<int>);
```

The ```TYPE_TO_STRING``` macro has an effect only in the current source file and thus needs to be used in some header if the same type will be used in separate source files for templated test cases.

Other testing frameworks use the header ```<typeinfo>``` in addition to demangling to get the string for types automatically but doctest cannot afford to include any header in its forward declaration part (the public one) of the header - so the user has to teach the framework for each type. This is done to achieve [maximal compile time performance](benchmarks.md).

Some notes:

- types are NOT filtered for uniqueness - the same templated test case can be instantiated multiple times for the same type - preventing that is left up to the user
- you don't need to provide stringification for every type as that plays a role only in the test case name - the default is ```<>``` - the tests will still work and be distinct
- if you need parameterization on more than 1 type you can package multiple types in a single one like this:

    ```c++
    template <typename first, typename second>
    struct TypePair {
        typedef first A;
        typedef second B;
    };

    #define pairs TypePair<int, char>, TypePair<char, int>

    TEST_CASE_TEMPLATE("multiple types", T, pairs) {
        typedef typename T::A T1;
        typedef typename T::B T2;
        // use T1 and T2 types
    }
    ```

------

- Check out the [**example**](../../examples/all_features/templated_test_cases.cpp) which shows how all of these are used.

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
