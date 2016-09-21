## Configuration

**doctest** is designed to "just work" as much as possible. It also allows configuring how it is built with a set of identifiers. 

The identifiers should be defined before the inclusion of the framework header.

- [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](#doctest_config_implement_with_main)
- [**```DOCTEST_CONFIG_IMPLEMENT```**](#doctest_config_implement)
- [**```DOCTEST_CONFIG_DISABLE```**](#doctest_config_disable)
- [**```DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES```**](#doctest_config_no_short_macro_names)
- [**```DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING```**](#doctest_config_treat_char_star_as_string)
- [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](#doctest_config_super_fast_asserts)
- [**```DOCTEST_CONFIG_USE_IOSFWD```**](#doctest_config_use_iosfwd)
- [**```DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION```**](#doctest_config_no_comparison_warning_suppression)
- [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](#doctest_config_no_unprefixed_options)
- [**```DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE```**](#doctest_config_assertion_parameters_by_value)
- [**```DOCTEST_CONFIG_COLORS_NONE```**](#doctest_config_colors_none)
- [**```DOCTEST_CONFIG_COLORS_WINDOWS```**](#doctest_config_colors_windows)
- [**```DOCTEST_CONFIG_COLORS_ANSI```**](#doctest_config_colors_ansi)
- [**```DOCTEST_CONFIG_WITH_NULLPTR```**](#doctest_config_with_nullptr)
- [**```DOCTEST_CONFIG_WITH_LONG_LONG```**](#doctest_config_with_long_long)
- [**```DOCTEST_CONFIG_WITH_STATIC_ASSERT```**](#doctest_config_with_static_assert)
- [**```DOCTEST_CONFIG_NO_NULLPTR```**](#doctest_config_no_nullptr)
- [**```DOCTEST_CONFIG_NO_LONG_LONG```**](#doctest_config_no_long_long)
- [**```DOCTEST_CONFIG_NO_STATIC_ASSERT```**](#doctest_config_no_static_assert)

For most people the only configuration needed is telling **doctest** which source file should host all the implementation code:

### **```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

This should be defined only in the source file where the library is implemented.

### **```DOCTEST_CONFIG_IMPLEMENT```**

If the client wants to [**supply the ```main()``` function**](main.md) (either to set an option with some value from the code or to integrate the framework into his existing project codebase) this identifier should be used.

This should be defined only in the source file where the library is implemented. It also creates a ```main()``` entry point.

### **```DOCTEST_CONFIG_DISABLE```**

One of the most most important configuration option - everything testing-related is removed from the binary - including most of the framework implementation and every test case written anywhere! This is one of the most unique features of **doctest**.

This should be defined globally.

### **```DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES```**

This will remove all macros from **doctest** that don't have the **```DOCTEST_```** prefix - like **```CHECK```**, **```TEST_CASE```** and **```SUBCASE```**. Then only the full macro names will be available - **```DOCTEST_CHECK```**, **```DOCTEST_TEST_CASE```** and **```DOCTEST_SUBCASE```**. The user is free to make his own short versions of these macros - [**example**](../../examples/alternative_macros/).

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING```**

By default ```char*``` is being treated as a pointer. With this option comparing ```char*``` pointers will switch to using ```strcmp()``` for comparisons and when stringified the string will be printed instead of the pointer value. 

This should be defined globally.

### **```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**

This makes the fast assert macros (```FAST_CHECK_EQ(a,b)``` - with a ```FAST_``` prefix) compile [**even faster**](benchmarks.md#cost-of-an-assertion-macro)! (35-80%)

Each fast assert is turned into a single function call - the only downside of this is: if an assert fails and a debugger is attached - when it breaks it will be in an internal function - the user will have to go 1 level up in the callstack to see the actual assert.

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_USE_IOSFWD```**

The library by default provides a forward declaration of ```std::ostream``` in order to support the ```operator<<``` [**stringification**](stringification.md) mechanism. This is forbidden by the standard (even though it works everywhere on all tested compilers). However if the user wishes to be 100% standards compliant - then this configuration option can be used to force the inclusion of ```<iosfwd>```.

Also it is possible that some STL implementation of a compiler with niche usage defines them differently - then there will be compilation errors in STL headers and using this option should fix the problem.  

This should be defined globally.

### **```DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION```**

By default the library suppresses warnings about comparing signed and unsigned types, etc.

- g++/clang ```-Wsign-conversion```
- g++/clang ```-Wsign-compare```
- g++/clang ```-Wdouble-promotion```
- msvc ```C4389``` 'operator' : signed/unsigned mismatch
- msvc ```C4018``` 'expression' : signed/unsigned mismatch

You can checkout [**this**](http://stackoverflow.com/questions/39106893) or [**this**](http://stackoverflow.com/questions/39075223) StackOverflow question to better understand why I suppress these warnings by default.

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**

This will disable the short versions of the [**command line**](commandline.md) options and only the versions with ```--dt-``` prefix will be parsed by **doctest** - this is possible for easy interoperability with client command line option handling when the testing framework is integrated within a client codebase - so there are no clashes and so that the user can exclude everything starting with ```--dt-``` from their option parsing.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE```**

This option forces all doctest asserts to copy by value the expressions they are given instead of binding them to const references. This might be useful to avoid ODR-usage of static constants (which might lead to linker errors with g++/clang):

```c++
template<typename T> struct type_traits { static const bool value = false; };

// unless DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE is defined the following assertion
// will lead to a linker error if type_traits<T>::value isn't defined in a translation unit
CHECK(type_traits::value == false);
```

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_COLORS_NONE```**

This will remove support for colors in the console output of the framework.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_COLORS_WINDOWS```**

This will force the support for colors in the console output to use the Windows APIs and headers.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_COLORS_ANSI```**

This will force the support for colors in the console output to use ANSI escape codes.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_WITH_NULLPTR```**

doctest tries to detect if c++11 ```nullptr``` is available but if it doesn't detect it - the user might define this.

This should be defined globally.

### **```DOCTEST_CONFIG_WITH_LONG_LONG```**

doctest tries to detect if c++11 ```long long``` is available but if it doesn't detect it - the user might define this.

This should be defined globally.

### **```DOCTEST_CONFIG_WITH_STATIC_ASSERT```**

doctest tries to detect if c++11 ```static_assert()``` is available but if it doesn't detect it - the user might define this.

This should be defined globally.

### **```DOCTEST_CONFIG_NO_NULLPTR```**

If doctest detects c++11 ```nullptr``` support as available but the user knows better - this can be defined to disable it.

This should be defined globally.

### **```DOCTEST_CONFIG_NO_LONG_LONG```**

If doctest detects c++11 ```long long``` support as available but the user knows better - this can be defined to disable it.

This should be defined globally.

### **```DOCTEST_CONFIG_NO_STATIC_ASSERT```**

If doctest detects c++11 ```static_assert()``` support as available but the user knows better - this can be defined to disable it.

This should be defined globally.

---------------

[Home](readme.md#reference)
