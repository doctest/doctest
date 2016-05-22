## String conversions

**doctest** needs to be able to convert types you use in assertions and logging expressions into strings (for logging and reporting purposes).
Most built-in types are supported out of the box but there are three ways that you can tell **doctest** how to convert your own types (or other, third-party types) into strings.

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

If you don't want to provide an ```operator<<``` overload, or you want to convert your type differently for testing purposes, you can provide an overload for ```doctest::toString()``` for your type.

```c++
namespace doctest {
	String toString(const T& value) {
		return convertMyTypeToString(value);
	}
}
```

Again ```T``` is your type and ```convertMyTypeToString``` is where you'll write whatever code is necessary to make your type printable. Note that the function must be in the **doctest** namespace which itself must be in the global namespace.

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

------

- Check out the [**example**](../../examples/stringification/main.cpp) which shows how to stringify ```std::vector<T>``` and other types.
- Note that the type ```String``` is used when specializing ```StringMaker<T>``` or overloading ```toString()``` - it is the string type **doctest** works with. ```std::string``` is not an option for the library because then it would have to drag the ```<string>``` header with it.
- To support the ```operator<<(std::ostream&...``` stringification the library has to offer a forward declaration of ```std::ostream``` and that is what the library does - but it is forbidden by the standard. It currently works everywhere - on all tested compilers - but if the user wishes to be 100% standards compliant - then the [**```DOCTEST_CONFIG_USE_IOSFWD```**](configuration.md) identifier can be used to force the inclusion of ```<iosfwd>```. The reason the header is not included by default is that on MSVC (for example) it drags a whole bunch of stuff with it - and after the preprocessor is finished the translation unit has grown to 42k lines of C++ code - while Clang and the libc++ are so well implemented that including ```<iosfwd>``` there results in 400 lines of code. 

---

[Home](readme.md#reference)
