
https://github.com/philsquared/Catch/issues/952

look into https://github.com/onqtam/doctest/issues/85

look into the static analysis reports of martin - https://github.com/onqtam/doctest/issues/83

update stuff (with a PR) in https://github.com/martinmoene/catch-lest-other-comparison

== when making a new release:

news.ycombinator.com
isocpp.org
reddit.com/r/cpp
reddit.com/r/programming
reddit.com/r/gamedev
reddit.com/r/cplusplus
https://www.linkedin.com/groups/86782/profile
twitter



== how to deal with pull requests for the main branch instead of the dev branch
- http://stackoverflow.com/questions/9135913/merge-pull-request-to-a-different-branch-than-default-in-github
- git fetch origin pull/ID/head:BRANCHNAME         << BRANCHNAME should be a new local branch! and then rebase it

== other
- operator<< trouble
    - see how Catch is implementing their IsStreamInsertable trait for C++11
    - https://github.com/philsquared/Catch/issues/757
    - https://github.com/philsquared/Catch/issues/872
    - https://github.com/philsquared/Catch/pull/877
    - https://github.com/philsquared/Catch/issues/880
- toString trouble - https://github.com/philsquared/Catch/issues/741
- https://github.com/philsquared/Catch/commit/33ed1773f40b406dbf3b7201bf52694bd86b1503

- I suspect -Wsign-compare is not being silenced by the pragmas...
  see this build - https://travis-ci.org/onqtam/game/jobs/196987454
  check_eq(numDigits(0), 1);     numDigits returns uint32
