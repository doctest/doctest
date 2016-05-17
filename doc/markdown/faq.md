## FAQ

- linker issues (_IMPLEMENT, etc)...

- why no regex and only wildcards

- tests are ran serially - no multi-threaded execution planned for now 

- mocking is not included because it is orthogonal to testing and a different third party library may be used for that (google mock)
https://github.com/tpounds/mockitopp

- property based testing - what it is and how to use it with doctest

- tests in headers... might end up in different test suites - and only 1 of them will get registered? or might have ifdef-ed parts that get compiled differently based on how/where the header is included...... so not a good practice to write tests in header files

- how subcases work - http://pastebin.com/rwghFzK4 - or look in the repo

- why c++98?

- why no ```long long``` in gcc/clang when in the c++98 standard?
    - http://stackoverflow.com/questions/35826731/detect-if-long-long-is-present-at-compile-time

- will the library support checking for memory leaks? no. use tools like valgrind or the sanitizers.

- is the VC++6 support full?
    - no
        - the stringification with ```ostream& operator<<(ostream&, myType)``` is disabled
        - comparing C strings will compare the pointers and not the actual strings