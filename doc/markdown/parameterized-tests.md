## Parameterized test cases

Test cases can be parameterized easily by type and indirectly by value.

## Value-parameterized test cases

There will be proper support for this in the future. For now there are 2 ways of doing data-driven testing in doctest:

- extracting the asserts in a helper function and calling it with a user-constructed array of data:

    ```c++
    void doChecks(int data) {
        // do asserts with data
    }

    TEST_CASE("test name") {
        std::vector<int> data {1, 2, 3, 4, 5, 6};

        for(auto& i : data) {
            CAPTURE(i); // log the current input data
            doChecks(i);
        }
    }
    ```

    This has several drawbacks:
    - in case of an exception (or a ```REQUIRE``` assert failing) the entire test case ends and the checks are not done for the rest of the input data
    - the user has to manually log the data with calls to ```CAPTURE()``` ( or ```INFO()```)
    - more boilerplate - doctest should supply primitives for generating data but currently doesnt - so the user has to write their own data generation

- using subcases to initialize data differently:

    ```c++
    TEST_CASE("test name") {
        int data;
        SUBCASE("") { data = 1; }
        SUBCASE("") { data = 2; }

        CAPTURE(data);

        // do asserts with data
    }
    ```

    This has the following drawbacks:
    - doesn't scale well - it is very impractical to write such code for more than a few different inputs
    - the user has to manually log the data with calls to ```CAPTURE()``` (or ```INFO()```)

    --------------------------------

    There is however an easy way to encapsulate this into a macro (written with C++14 for simplicity):

    ```c++
    #include <algorithm>
    #include <string>

    #define DOCTEST_VALUE_PARAMETERIZED_DATA(data, data_container)                                  \
        static size_t _doctest_subcase_idx = 0;                                                     \
        std::for_each(data_container.begin(), data_container.end(), [&](const auto& in) {           \
            DOCTEST_SUBCASE((std::string(#data_container "[") +                                     \
                            std::to_string(_doctest_subcase_idx++) + "]").c_str()) { data = in; }  \
        });                                                                                         \
        _doctest_subcase_idx = 0
    ```

    and now this can be used as follows:

    ```c++
    TEST_CASE("test name") {
        int data;
        std::list<int> data_container = {1, 2, 3, 4}; // must be iterable - std::vector<> would work as well

        DOCTEST_VALUE_PARAMETERIZED_DATA(data, data_container);

        printf("%d\n", data);
    }
    ```

    and will print the 4 numbers by re-entering the test case 3 times (after the first entry) - just like subcases work:

    ```
    1
    2
    3
    4
    ```

    The big limitation of this approach is that the macro cannot be used with other subcases at the same code block {} indentation level (will act weird) - it can only be used within a subcase.

Stay tuned for proper value-parameterization in doctest!

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

Other testing frameworks use the header ```<typeinfo>``` in addition to demangling to get the string for types automatically but doctest cannot afford to include any header in it's forward declaration part (the public one) of the header - so the user has to teach the framework for each type. This is done to achieve [maximal compile time performance](benchmarks.md).

Some notes:

- types are NOT filtered for uniqueness - the same templated test case can be instantiated multiple times for the same type - preventing that is left up to the user
- you don't need to provide stringification for every type as that plays a role only in the test case name - the default is ```<>``` - the tests will still work and be distinct
- if you need parameterization on more than 1 type you can package multiple types in a single one like this:

    ```c++
    template <typename first, typename second>
    struct TypePair
    {
        typedef first  A;
        typedef second B;
    };

    #define pairs \
        TypePair<int, char>, \
        TypePair<char, int>

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
