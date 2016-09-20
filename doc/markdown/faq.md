## FAQ

#TODO...

- how is it different from Catch (or what is lacking)

- require macros not working properly when using -fno-exceptions

- static libraries - also mention https://github.com/pthom/doctest_registerlibrary

- char* comparisons

- how to get the most out of the library - fast asserts, custom macro names, etc.

- how to write tests in header-only libraries (DOCTEST_LIBRARY_INCLUDED, and a tag)

- linker issues? Make sure you have instantiated the test runner in only one source file.

- compiler errors in STL headers when including the doctest header

  try using [**```DOCTEST_CONFIG_USE_IOSFWD```**](configuration.md#DOCTEST_CONFIG_USE_IOSFWD)

- stringification issues - if operator<<(ostream for a type is visible only in one source file...

- why macros - aren't they evil? - http://accu.org/index.php/journals/2064

- why no regex and only wildcards

- tests are ran serially - no multi-threaded execution planned for now 

- mocking is not included because it is orthogonal to testing and a different third party library may be used for that (google mock)
https://github.com/tpounds/mockitopp

- linker errors for ```doctest::detail::...``` when using ```DOCTEST_CONFIG_DISABLE```
    solution: don't use anything from detail

- property based testing - what it is and how to use it with doctest

- tests in headers... might end up in different test suites - and only 1 of them will get registered? or might have ifdef-ed parts that get compiled differently based on how/where the header is included...... so not a good practice to write tests in header files

- how subcases work - http://pastebin.com/rwghFzK4 - or look in the repo

- why c++98? because.

- I'm a power user - is there anything else I should know?

   separate headers

- will the library support checking for memory leaks? no. use tools like valgrind or the sanitizers.

- mixing different versions of the framework within the same executable?
    - unfortunately what single header libraries like [stb](https://github.com/nothings/stb) are doing is not feasible with this library.
    - it could be done if tests are written only in source files where the library has been implemented with the ```DOCTEST_CONFIG_IMPLEMENT``` macro but that is very limiting.

- is the VC++6 support full?
    - no
        - the stringification with ```ostream& operator<<(ostream&, myType)``` is disabled
        - comparing C strings will compare the pointers and not the actual strings
        - VC6 subcases not working - set a bounty on this: http://stackoverflow.com/questions/36940730/preprocessor-problems-with-vc6

---------------

[Home](readme.md#reference)
