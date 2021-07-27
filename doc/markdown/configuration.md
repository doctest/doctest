## Configuration

**doctest** is designed to "just work" as much as possible. It also allows configuring how it is built with a set of identifiers. 

The identifiers should be defined before the inclusion of the framework header.

Defining something ```globally``` means for every source file of the binary (executable / shared object).

- [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](#doctest_config_implement_with_main)
- [**```DOCTEST_CONFIG_IMPLEMENT```**](#doctest_config_implement)
- [**```DOCTEST_CONFIG_DISABLE```**](#doctest_config_disable)
- [**```DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL```**](#doctest_config_implementation_in_dll)
- [**```DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES```**](#doctest_config_no_short_macro_names)
- [**```DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING```**](#doctest_config_treat_char_star_as_string)
- [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](#doctest_config_super_fast_asserts)
- [**```DOCTEST_CONFIG_USE_STD_HEADERS```**](#doctest_config_use_std_headers)
- [**```DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS```**](#doctest_config_void_cast_expressions)
- [**```DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION```**](#doctest_config_no_comparison_warning_suppression)
- [**```DOCTEST_CONFIG_OPTIONS_PREFIX```**](#doctest_config_options_prefix)
- [**```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**](#doctest_config_no_unprefixed_options)
- [**```DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS```**](#doctest_config_no_try_catch_in_asserts)
- [**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](#doctest_config_no_exceptions)
- [**```DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS```**](#doctest_config_no_exceptions_but_with_all_asserts)
- [**```DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE```**](#doctest_config_assertion_parameters_by_value)
- [**```DOCTEST_CONFIG_COLORS_NONE```**](#doctest_config_colors_none)
- [**```DOCTEST_CONFIG_COLORS_WINDOWS```**](#doctest_config_colors_windows)
- [**```DOCTEST_CONFIG_COLORS_ANSI```**](#doctest_config_colors_ansi)
- [**```DOCTEST_CONFIG_WINDOWS_SEH```**](#doctest_config_windows_seh)
- [**```DOCTEST_CONFIG_NO_WINDOWS_SEH```**](#doctest_config_no_windows_seh)
- [**```DOCTEST_CONFIG_POSIX_SIGNALS```**](#doctest_config_posix_signals)
- [**```DOCTEST_CONFIG_NO_POSIX_SIGNALS```**](#doctest_config_no_posix_signals)
- [**```DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS```**](#doctest_config_include_type_traits)
- [**```DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS```**](#doctest_config_no_multi_lane_atomics)

For most people the only configuration needed is telling **doctest** which source file should host all the implementation code:

### **```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

This should be defined only in the source file where the library is implemented. It also creates a ```main()``` entry point.

### **```DOCTEST_CONFIG_IMPLEMENT```**

If the client wants to [**supply the ```main()``` function**](main.md) (either to set an option with some value from the code or to integrate the framework into their existing project codebase) this identifier should be used.

This should be defined only in the source file where the library is implemented.

### **```DOCTEST_CONFIG_DISABLE```**

One of the most most important configuration option - everything testing-related is removed from the binary - including most of the framework implementation and every test case written anywhere! This is one of the most unique features of **doctest**.

This should be defined globally.

### **```DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL```**

This will affect the public interface of doctest - all necessary forward declarations for writing tests will be turned into imported symbols. That way the test runner doesn't have to be implemented in the binary (executable / shared object) and can be reused from another binary where it is built and exported.

To export the test runner from a binary simply use [**```DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL```**](#doctest_config_implementation_in_dll) together with [**```DOCTEST_CONFIG_IMPLEMENT```**](#doctest_config_implement) (or [**```DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN```**](#doctest_config_implement_with_main) but then the other binaries will have to link to the executable) in whatever source file the test runner gets implemented into. Note that this identifier should not be defined in the other source files of the binary which exports the doctest test runner - or there will be linker conflicts - having the same symbols as both imported and exported within the same binary.

Checkout the [**example**](../../examples/executable_dll_and_plugin/) - it shows how to have the test runner implemented in a dll (and there are even tests in a plugin which is dynamically loaded).

This should be defined globally in binaries that import the symbols.

This should be defined only in the source file where the library is implemented for binaries that export the test runner.

### **```DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES```**

This will remove all macros from **doctest** that don't have the **```DOCTEST_```** prefix - like **```CHECK```**, **```TEST_CASE```** and **```SUBCASE```**. Then only the full macro names will be available - **```DOCTEST_CHECK```**, **```DOCTEST_TEST_CASE```** and **```DOCTEST_SUBCASE```**. The user is free to make their own short versions of these macros - [**example**](../../examples/all_features/alternative_macros.cpp).

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING```**

By default ```char*``` is being treated as a pointer. With this option comparing ```char*``` pointers will switch to using ```strcmp()``` for comparisons and when stringified the string will be printed instead of the pointer value. 

This should be defined globally.

### **```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**

This config option makes the assert macros (except for those dealing with exceptions) compile [**much faster**](benchmarks.md#cost-of-an-assertion-macro)! (31-91% - depending on the type - [**normal**](assertions.md#expression-decomposing-asserts) or [**binary**](assertions.md#binary-and-unary-asserts))

Each assert is turned into a single function call - the only downside of this is: if an assert fails and a debugger is attached - when it breaks it will be in an internal function - the user will have to go 1 level up in the callstack to see the actual assert.

It also implies [**```DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS```**](#doctest_config_no_try_catch_in_asserts) (so exceptions thrown during the evaluation of an assert are not caught by the assert itself but by the testing framework - meaning that the test case is immediately aborted).

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_USE_STD_HEADERS```**

The library by default provides a forward declaration of ```std::ostream``` in order to support the ```operator<<``` [**stringification**](stringification.md) mechanism (also ```std::tuple<>``` and ```std::nullptr_t```). This is forbidden by the standard (even though it works everywhere on all tested compilers). However if the user wishes to be 100% standards compliant - then this configuration option can be used to force the inclusion of the relevant standard headers.

Also it is possible that some STL implementation of a compiler with niche usage defines them differently - then there will be compilation errors in STL headers and using this option should fix the problem.  

This should be defined globally.

### **```DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS```**

This affects the [asserts dealing with exceptions](assertions.md#exceptions) - the expression is cast to void to avoid problems such as when functions with the ```[[nodiscard]]``` attribute are used but their result isn't checked.

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION```**

By default the library suppresses warnings about comparing signed and unsigned types, etc.

- g++/clang ```-Wsign-conversion```
- g++/clang ```-Wsign-compare```
- msvc ```C4389``` 'operator' : signed/unsigned mismatch
- msvc ```C4018``` 'expression' : signed/unsigned mismatch

You can checkout [**this**](https://github.com/onqtam/doctest/issues/16#issuecomment-246803303) issue to better understand why I suppress these warnings by default.

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_OPTIONS_PREFIX```**

Defining this as a string will change the prefix of the [**command line**](commandline.md) options to use the given prefix instead of the default ```dt-``` prefix. This can be useful for integrating the testing framework into a client codebase, where a command option prefix like ```selftest-``` might be more clear to users.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS```**

This will disable the short versions of the [**command line**](commandline.md) options and only the versions with ```--dt-``` prefix will be parsed by **doctest** - this is possible for easy interoperability with client command line option handling when the testing framework is integrated within a client codebase - so there are no clashes and so that the user can exclude everything starting with ```--dt-``` from their option parsing.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS```**

This will remove all ```try``` / ```catch``` sections from:

- the [normal asserts](assertions.md#expression-decomposing-asserts)
- the [binary and unary asserts](assertions.md#binary-and-unary-asserts)

so exceptions thrown while evaluating the expression in an assert will terminate the current test case.

This can be used for some mild compile time savings but for greater impact look into [**```DOCTEST_CONFIG_SUPER_FAST_ASSERTS```**](configuration.md#doctest_config_super_fast_asserts).

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_NO_EXCEPTIONS```**

This will remove everything that uses exceptions from the framework - it is also auto detectable if exceptions are disabled for compilers (like with ```-fno-exceptions``` for GCC/Clang).

What gets changed:

- asserts that evaluate the expression in a ```try``` / ```catch``` section no longer evaluate in such a context
- ```REQUIRE``` macros are gone (undefined)
- [exception macros](assertions.md#exceptions) are gone (undefined)
- the ```abort-after``` option won't be fully working because an exception is used to terminate test cases

The ```REQUIRE``` family of asserts uses exceptions to terminate the current test case when they fail. An exception is used instead of a simple ```return;``` because asserts can be used not only in a test case but also in functions called by a test case.

Also some of the [**logging macros**](logging.md#messages-which-can-optionally-fail-test-cases) which act like a ```REQUIRE``` assert (terminating the test case) - like ```FAIL()``` - start to work differently - like a ```FAIL_CHECK()```.

[**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](#doctest_config_no_exceptions) implies [**```DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS```**](#doctest_config_no_try_catch_in_asserts)

If you wish to use asserts that deal with exceptions and only sometimes build without exceptions - check the [**```DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS```**](#doctest_config_no_exceptions_but_with_all_asserts) config option.

This should be defined globally.

### **```DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS```**

When building with no exceptions (see [**```DOCTEST_CONFIG_NO_EXCEPTIONS```**](#doctest_config_no_exceptions)) ```REQUIRE``` asserts and the ones about dealing with exceptions are gone.

If however you want your code to use these assertions and only sometimes build without exceptions - then using this config will be of help. The effects of using it are the following:

- ```REQUIRE``` asserts are not gone - but they act like ```CHECK``` asserts - when one of them fails the whole test case will be marked as failed but will not be exited immediately
- the [asserts for dealing with exceptions](assertions.md#exceptions) are turned into a no-op (instead of being totally undefined)

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE```**

This option forces all doctest asserts to copy by value the expressions they are given instead of binding them to const references. This might be useful to avoid ODR-usage of static constants (which might lead to linker errors with g++/clang):

```c++
template<typename T> struct type_traits { static const bool value = false; };

// unless DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE is defined the following assertion
// will lead to a linker error if type_traits<int>::value isn't defined in a translation unit
CHECK(type_traits<int>::value == false);
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

### **```DOCTEST_CONFIG_WINDOWS_SEH```**

This will enable SEH handling on Windows. Currently enabled only when compiled with MSVC, because some versions of MinGW do not have the necessary Win32 API support. The user may choose to enable this explicitly - it is known to work with the MinGW-w64 project.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_NO_WINDOWS_SEH```**

This can be used to disable **```DOCTEST_CONFIG_WINDOWS_SEH```** when it is auto-selected by the library.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_POSIX_SIGNALS```**

This will enable the use of signals under UNIX for handling crashes. On by default.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_NO_POSIX_SIGNALS```**

This can be used to disable **```DOCTEST_CONFIG_POSIX_SIGNALS```** when it is auto-selected by the library.

This should be defined only in the source file where the library is implemented (it's relevant only there).

### **```DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS```**

This can be used to include the ```<type_traits>``` C++11 header. That in turn will enable the ability for the ```Approx``` helper to be used with strong typedefs of ```double``` - check [this](https://github.com/onqtam/doctest/issues/62) or [this](https://github.com/onqtam/doctest/issues/85) issue for more details on that.

This can be defined both globally and in specific source files only.

### **```DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS```**

This can be used to disable multi lane atomics. Multi lane atomics can speed up highly parallel use of assert statements, but have a small overhead for single threaded applications.

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
