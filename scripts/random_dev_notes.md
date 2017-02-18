


add a wait key option
https://github.com/philsquared/Catch/issues/477#issuecomment-256417686




check if this is possible:
REQUIRE_THROWS({
    item.next();
    item.skip();
});



add a build with "-fno-rtti"


for the exceptions related macros:
static_cast<void>(expr);
or maybe not:
https://github.com/philsquared/Catch/issues/798



I suspect -Wsign-compare is not being silenced by the pragmas...
see this build - https://travis-ci.org/onqtam/game/jobs/196987454
check_eq(numDigits(0), 1);     numDigits returns uint32



make the bench.py script more friendly for users



https://github.com/philsquared/Catch/issues/579


runtime performance: lazily stringify expressions - only when needed


document that the approx helper uses relative percentage and not absolute value as the epsilon
https://github.com/philsquared/Catch/issues/499#issuecomment-262398916




https://github.com/philsquared/Catch/pull/753
https://www.reddit.com/r/cpp/comments/5h6sq2/trying_googletest/db0q5h1/
Fix for sigsegv stack overflow behavior



https://github.com/linuxfoundation/cii-best-practices-badge




look into UnitTest++ and how it does test suites
https://github.com/chadaustin/sajson/blob/master/tests/test.cpp





better color stuffs in console (also styles - underline, bold, etc)
https://github.com/agauniyal/rang



linux debugger support
https://github.com/philsquared/Catch/pull/585



list users of doctest - like this:
https://github.com/philsquared/Catch/blob/master/docs/opensource-users.md




compilation errors with operator<< - should test this!
https://github.com/philsquared/Catch/issues/757


- ability to transfer registered tests between binaries - so the runner of the executable can execute the ones from the shared objects as well!
- ability to not have a test runner in the shared object? so registration symbols should be imported... idk



add way to explicitly register a test - for the static libs issue...

my current way of gettin the test registry vs nifty counter - https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter



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
