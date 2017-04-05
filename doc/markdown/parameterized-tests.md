## Parameterized test cases

Test cases can be parameterized by value and by type - currently only by type is supported.

## Templated test cases - parameterized by type

Suppose you have multiple implementations of the same interface and want to make sure that all of them satisfy some common requirements. Or, you may have defined several types that are supposed to conform to the same "concept" and you want to verify it. In both cases, you want the same test logic repeated for different types.

While you can write one ```TEST_CASE``` for each type you want to test (and you may even factor the test logic into a function template that you invoke from the test case), it's tedious and doesn't scale: if you want ```M``` tests over ```N``` types, you'll end up writing ```M * N``` tests.

Templated tests allow you to repeat the same test logic over a list of types. You only need to write the test logic once.

There are 2 ways to do it:

- directly pass the list of types to the templated test case

    ```c++
    typedef doctest::Types<char, short, int, long long int> the_types;

    TEST_CASE_TEMPLATE("signed integers stuff", T, the_types) {
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

    typedef doctest::Types<char, short, int, long long int> the_types;
    TEST_CASE_TEMPLATE_INSTANTIATE(test_id, the_types);

    typedef doctest::Types<float, double> the_types_2;
    TEST_CASE_TEMPLATE_INSTANTIATE(test_id, the_types_2);
    ```
    If you are designing an interface or concept, you can define a suite of type-parameterized tests to verify properties that any valid implementation of the interface/concept should have. Then, the author of each implementation can just instantiate the test suite with his type to verify that it conforms to the requirements, without having to write similar tests repeatedly.


A test case named ```signed integers stuff``` instantiated for type ```int``` will yield the following test case name:

```
signed integers stuff<int>
```

By default all primitive types (fundamental - ```int```, ```bool```, ```float```...) have stringification provided by the library. For all other types the user will have to use the ```TYPE_TO_STRING(type)``` macro - like this:

```c++
TYPE_TO_STRING(std::vector<int>);
```

The ```TYPE_TO_STRING``` macro has an effect only in the current source file and thus needs to be used in some header if the same type will be used in separate source files for templated test cases.

Other testing frameworks use the header ```<typeinfo>``` in addition to demangling to get the string for types automatically but doctest cannot afford to include any header in it's forward declaration part (the public one) of the header - so the user has to teach the framework for each type. This is done to achieve [maximal compile time performance](benchmarks.md).

Some notes:

- types are NOT filtered for uniqueness - the same templated test case can be instantiated multiple times for the same type - preventing that is left up to the user
- you don't need to provide stringification for every type as that plays a role only in the test case name - the default is ```<>``` - the tests will still work and be distinct
- the ```doctest::Types<>``` template accepts up to 60 type arguments
- if variadic macros are enabled (see [**```DOCTEST_CONFIG_WITH_VARIADIC_MACROS```**](configuration.md#doctest_config_with_variadic_macros)) the typedefs can be skipped and the type lists can be constructed directly in the macros - otherwise the compiler will think that each comma in the type list introduces a new macro argument. With variadic macro support the ```TYPE_TO_STRING``` macro will also be able to work with types such as ```std::pair<int, float>```.
- if you need parameterization on more than 1 type you can package multiple types in a single one like this:

    ```c++
    template <typename first, typename second>
    struct TypePair
    {
        typedef first  A;
        typedef second B;
    };

    typedef Types<
        TypePair<int, char>,
        TypePair<char, int>
    > pairs;

    TEST_CASE_TEMPLATE("multiple types", T, pairs) {
        typedef typename T::A T1;
        typedef typename T::B T2;
        // use T1 and T2 types
    }
    ```

---

[Home](readme.md#reference)
