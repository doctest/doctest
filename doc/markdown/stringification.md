## String conversions

**doctest** needs to be able to convert types you use in assertions and logging expressions into strings (for logging and reporting purposes).
Most built-in types are supported out of the box but there are three ways that you can tell **doctest** how to convert your own types (or other, third-party types) into strings.

For stringifying enums checkout [this issue](https://github.com/doctest/doctest/issues/121).

## ```operator<<``` overload for ```std::ostream```

This is the standard way of providing string conversions in C++ - and the chances are you may already provide this for your own purposes. If you're not familiar with this idiom it involves writing a free function of the form:

```c++
std::ostream& operator<< (std::ostream& os, const T& value) {
    os << convertMyTypeToString(value);
    return os;
}
```

(where ```T``` is your type and ```convertMyTypeToString``` is where you'll write whatever code is necessary to make your type printable - it doesn't have to be in another function).

You should put this function in the same namespace as your type.

Alternatively you may prefer to write it as a member function:

```c++
std::ostream& T::operator<<(std::ostream& os) const {
    os << convertMyTypeToString(*this);
    return os;
}
```

## ```doctest::toString``` overload

If you don't want to provide an ```operator<<``` overload, or you want to convert your type differently for testing purposes, you can provide an overload for ```toString()``` for your type which returns ```doctest::String```.

```c++
namespace user {
    struct udt {};
    
    doctest::String toString(const udt& value) {
        return convertMyTypeToString(value);
    }
}
```

Note that the function must be in the same namespace as your type. If the type is not in any namespace - then the overload should be in the global namespace as well. ```convertMyTypeToString``` is where you'll write whatever code is necessary to make your type printable.

## ```doctest::StringMaker<T>``` specialisation

There are some cases where overloading ```toString``` does not work as expected. Specialising ```StringMaker<T>``` gives you more precise and reliable control - but at the cost of slightly more code and complexity:

```c++
namespace doctest {
    template<> struct StringMaker<T> {
        static String convert(const T& value) {
            return convertMyTypeToString(value);
        }
    };
}
```

## Translating exceptions

By default all exceptions deriving from ```std::exception``` will be translated to strings by calling the ```what()``` method (also C strings). For exception types that do not derive from ```std::exception``` - or if ```what()``` does not return a suitable string - use ```REGISTER_EXCEPTION_TRANSLATOR```. This defines a function that takes your exception type and returns a ```doctest::String```. It can appear anywhere in the code - it doesn't have to be in the same translation unit. For example:

```c++
REGISTER_EXCEPTION_TRANSLATOR(MyType& ex) {
    return doctest::String(ex.message());
}
```

Note that the exception may be accepted without a reference but it is considered bad practice in C++.

An alternative way to register an exception translator is to do the following in some function - before executing any tests:

```c++
    // adding a lambda - the signature required is `doctest::String(exception_type)`
    doctest::registerExceptionTranslator<int>([](int in){ return doctest::toString(in); });
```

The order of registering exception translators can be controlled - simply call the explicit function in the required order or list the exception translators with the macro in a top-to-bottom fashion in a single translation unit - everything that auto-registers in doctest works in a top-to-bottom way for a single translation unit (source file).

You could also [override the translation mechanism](https://github.com/catchorg/Catch2/issues/539#issuecomment-454549904) for exceptions deriving from ```std::exception```.

------

- Check out the [**example**](../../examples/all_features/stringification.cpp) which shows how to stringify ```std::vector<T>``` and other types/exceptions.
- Note that the type ```String``` is used when specializing ```StringMaker<T>``` or overloading ```toString()``` - it is the string type **doctest** works with. ```std::string``` is not an option because doctest would have to include the ```<string>``` header.
- To support the ```operator<<(std::ostream&...``` stringification the library has to offer a forward declaration of ```std::ostream``` and that is what the library does - but it is forbidden by the standard. It currently works everywhere - on all tested compilers - but if the user wishes to be 100% standards compliant - then the [**```DOCTEST_CONFIG_USE_STD_HEADERS```**](configuration.md#doctest_config_use_std_headers) identifier can be used to force the inclusion of ```<iosfwd>```. The reason the header is not included by default is that on MSVC (for example) it drags a whole bunch of stuff with it - and after the preprocessor is finished the translation unit has grown to 42k lines of C++ code - while Clang and the libc++ are so well implemented that including ```<iosfwd>``` there results in 400 lines of code. 

---

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
