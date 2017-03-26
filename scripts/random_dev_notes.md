

todo:
- MESSAGE (like WARN in catch and MESSAGE in boost test)
- FAIL (like in catch)
- FAIL_CHECK (like in catch)

- checkpoint
- passpoint
http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/boost_test/test_output/test_tools_support_for_logging/checkpoints.html

- #define IREQUIRE(cond, msg) do { INFO(msg); REQUIRE(cond); } while(0)
BOOST_WARN_MESSAGE(predicate, message);
BOOST_CHECK_MESSAGE(predicate, message);
BOOST_REQUIRE_MESSAGE(predicate, message);
http://stackoverflow.com/questions/24009797/how-can-i-retrieve-the-last-argument-of-a-c99-variadic-macro


look at boost test again:
http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/index.html

document that the exception in the _THROWS_AS macros is used as-is and users should use const ref


note in all custom mains with a comment that the defaults/overrides are just an example and users shouldn't just copy/paste them!


Rust #[test]

dont rule out boost.test as not mainstream - its right there with catch and google test

Nothing is better than documentation with examples. Nothing is worse than examples that don't actually work, because the code has changed since the documentation has been (was) written.



try if there are warnings for unused variables when tests are disabled



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
