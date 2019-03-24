
figure out how to get the filters to the reporter interface so users can access them in any .cpp file (also the list of reporters)

move down from /Wall to /W4 for Visual Studio... /Wall is absolutely unnecessary and handling it clutters the repository

get rid of DOCTEST_DECLARE_DEFAULTS & friends...
    AND get rid of DOCTEST_INTERFACE for simple structs!

https://github.com/catchorg/Catch2/blob/master/docs/tostring.md#catchis_range-specialisation

rope-ish startup of doctest - and filter duplicates on run()

https://blog.travis-ci.com/2018-10-11-windows-early-release

add in the faq about the compile errors with stream<<"array" - nikola goranov, also moving to catch undef/define stuffs - use vararg macro, also make the "moving from catch to doctest" easier to find, also this: https://stackoverflow.com/questions/35530850/how-to-require-a-semicolon-after-a-macro/52122660#52122660



fix discoverability of subcases
    look at the subcase filtering and the filter_3 example - the test case is re-entered when in fact it shouldn't have
        convoluted to solve - not really important

https://github.com/catchorg/Catch2/commit/de36b2ada6e4593a9a32c4c86cd47d4bc002b148

try to forward declare std::string and specialize the string maker for it or something like that

UDL literal for approx: Approx operator "" _a(long double val)

https://github.com/catchorg/Catch2/issues/980
https://github.com/catchorg/Catch2/commit/00af677577973758b3b35e1c94ab4142c45c3f67
https://github.com/catchorg/Catch2/commit/11f716f28d6621d1e54cf13374f8b5dd1a7489f5
https://github.com/catchorg/Catch2/commit/22ac9d2184b3868cccfd635eb631d0eee1529121
https://github.com/catchorg/Catch2/commit/0b1f1b10030942c38ca8c1b95140b0c65efc903e

== when making a new release:

news.ycombinator.com
isocpp.org
reddit.com/r/cpp
reddit.com/r/programming
reddit.com/r/gamedev
reddit.com/r/cplusplus
https://www.linkedin.com/groups/86782/profile
twitter
https://www.linkedin.com/groups/86998/
https://www.linkedin.com/groups/100895/
https://www.linkedin.com/groups/1917190/
https://www.linkedin.com/groups/2771729/
https://www.linkedin.com/groups/133720/
https://www.linkedin.com/groups/2526482/
https://www.viva64.com/en/b/0604/

on big releases: update benchmarks, number of CI builds, differences with Catch, docs, etc.

also check if anything should be added here:
https://github.com/martinmoene/catch-lest-other-comparison

== how to deal with pull requests for the main branch instead of the dev branch
- http://stackoverflow.com/questions/9135913/merge-pull-request-to-a-different-branch-than-default-in-github
- git fetch origin pull/ID/head:BRANCHNAME         << BRANCHNAME should be a new local branch! and then rebase it

== other
- operator<< trouble
    - see how Catch is implementing their IsStreamInsertable trait for C++11
    - https://github.com/catchorg/Catch2/issues/757
    - https://github.com/catchorg/Catch2/issues/872
    - https://github.com/catchorg/Catch2/pull/877
    - https://github.com/catchorg/Catch2/issues/880
    - https://github.com/catchorg/Catch2/pull/1405
- toString trouble - https://github.com/catchorg/Catch2/issues/741
- https://github.com/catchorg/Catch2/commit/33ed1773f40b406dbf3b7201bf52694bd86b1503

- I suspect -Wsign-compare is not being silenced by the pragmas...
  see this build - https://travis-ci.org/onqtam/game/jobs/196987454
  check_eq(numDigits(0), 1);     numDigits returns uint32
