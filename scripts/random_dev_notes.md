mingw on appveyor - https://github.com/boost-experimental/di/blob/cpp14/.appveyor.yml
https://github.com/boostorg/hana/blob/master/.appveyor.yml

- llvm - https://github.com/boostorg/hana/commit/f08e824a34cc4e6e850fee285f3d182f95440f60
https://github.com/boost-experimental/di/blob/cpp14/.travis.yml

- patreon

- mocking - google mock assertion macros

- should check the expression decomposer - if it copies by value anything

- drop support for VC++6

- add underscores to all preprocessor identifiers not intended for use by the user

- users.md - projects that use doctest

- tests in a static library without the implementation there - problematic

- look at https://github.com/philsquared/Catch/issues/675

- fix unary/binary assertion macros...

- think about the expression decomposition static asserts
    - the static assert should use the c++11 feature if possible

- signed/unsigned comparison trouble in assertions

- examples
    - removing everything with -dt- from the command line - https://github.com/onqtam/doctest/issues/20
    - using the fast assertions
    - a test in a header in one binary with multiple cpp files

- coverage

- write tests for the library itself

- better docs
    - not just for c++98
    - updated compile time benchmarks (and include linux!)
    - list the subcases as a major selling point on the main page - after the expression decomposition macro stuff
    - faq - why macros - http://accu.org/index.php/journals/2064
    - FAQ - how is it different from Catch (or what is lacking)
    - faq - why c++
    - default options should be mentioned
    - mention the new asserts
    - for power users - separate headers
    - post the traffic screenshot in the FAQ - "how epic was the initial release"

post in reddit for 1.1!











how to deal with pull requests for the main branch instead of the dev branch
- http://stackoverflow.com/questions/9135913/merge-pull-request-to-a-different-branch-than-default-in-github
- git fetch origin pull/ID/head:BRANCHNAME
