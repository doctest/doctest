



exception translation mechanism - #12
- test with DOCTEST_CONFIG_NO_EXCEPTIONS
- exported symbols?
- edit examples - disabled and stringification
- write in docs
    - REGISTER_EXCEPTION_TRANSLATOR - can catch by value and by reference
    - doctest::registerExceptionTranslator<int>([](int in){ return doctest::String("int: ") + doctest::toString(in); });




== when making a new release:

news.ycombinator.com
isocpp.org
reddit.com/r/cpp
reddit.com/r/programming
reddit.com/r/gamedev
reddit.com/r/cplusplus

https://www.facebook.com/groups/cppEnthusiasts/

https://www.facebook.com/groups/226823534127108/
https://plus.google.com/communities/116007775542700637383
https://www.linkedin.com/groups/86782/profile
http://webchat.freenode.net/?channels=c++

and some pages...
https://www.facebook.com/cplusplus/
https://www.facebook.com/cpptovlee/
https://www.facebook.com/CPPprogram/
https://www.facebook.com/itsaji.vb.page
https://www.facebook.com/CppInstitute


twitter

slashdot?







== how to deal with pull requests for the main branch instead of the dev branch
- http://stackoverflow.com/questions/9135913/merge-pull-request-to-a-different-branch-than-default-in-github
- git fetch origin pull/ID/head:BRANCHNAME

== other
- compilation errors with operator<< - happens with doctest - in case it comes up... https://github.com/philsquared/Catch/issues/757

- I suspect -Wsign-compare is not being silenced by the pragmas...
  see this build - https://travis-ci.org/onqtam/game/jobs/196987454
  check_eq(numDigits(0), 1);     numDigits returns uint32
