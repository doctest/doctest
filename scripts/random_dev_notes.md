- benchmark new assertion macros






- better docs
    - char ptr comparison - not what u'd expect
    - not just for c++98
    - updated compile time benchmarks (and include linux!)
    - list the subcases as a major selling point on the main page - after the expression decomposition macro stuff
    - faq - why macros - http://accu.org/index.php/journals/2064
    - FAQ - how is it different from Catch (or what is lacking)
    - faq - why c++
    - faq - require macros not working properly when using -fno-exceptions
    - FAQ - static libraries - also mention https://github.com/pthom/doctest_registerlibrary
    - default options should be mentioned
    - mention the new asserts
    - document the new config identifiers
    - for power users - separate headers
    - how to write tests in header-only libraries (DOCTEST_LIBRARY_INCLUDED, and a tag)
    - post the traffic screenshot in the FAQ - "how epic was the initial release"

- the project is in need of sponsors!

cpp
hackernews
programming
cplusplus
isocpp.org





how to deal with pull requests for the main branch instead of the dev branch
- http://stackoverflow.com/questions/9135913/merge-pull-request-to-a-different-branch-than-default-in-github
- git fetch origin pull/ID/head:BRANCHNAME
