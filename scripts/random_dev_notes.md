
add VS 2017 build on appveyor when the image is officially added:
https://www.appveyor.com/docs/installed-software/



add link to overload in main readme and in docs folder



I suspect -Wsign-compare is not being silenced by the pragmas...
see this build - https://travis-ci.org/onqtam/game/jobs/196987454
check_eq(numDigits(0), 1);     numDigits returns uint32



make the bench.py script more friendly for users




https://github.com/philsquared/Catch/pull/753
https://www.reddit.com/r/cpp/comments/5h6sq2/trying_googletest/db0q5h1/
Fix for sigsegv stack overflow behavior



https://github.com/linuxfoundation/cii-best-practices-badge




look into UnitTest++ and how it does test suites
https://github.com/chadaustin/sajson/blob/master/tests/test.cpp




linux debugger support
https://github.com/philsquared/Catch/pull/585



list users of doctest - like this:
https://github.com/philsquared/Catch/blob/master/docs/opensource-users.md



add way to explicitly register a test - for the static libs issue...

my current way of gettin the test registry vs nifty counter - https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter


think about this:
https://github.com/philsquared/Catch/issues/734


about executing tests in a separate process:
https://github.com/philsquared/Catch/issues/853




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
