## FAQ

- linker issues (_IMPLEMENT, etc)...

- tests are ran serially - no multi-threaded execution planned for now 

- mocking is not included because it is orthogonal to testing and a different third party library may be used for that (google mock)
https://github.com/tpounds/mockitopp

- property based testing - what it is and how to use it with doctest

- tests in headers... might end up in different test suites - and only 1 of them will get registered? or might have ifdef-ed parts that get compiled differently based on how/where the header is included...... so not a good practice to write tests in header files

- how subcases work - http://pastebin.com/rwghFzK4 - or look in the repo
