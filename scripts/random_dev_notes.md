
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <vector>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END
DOCTEST_MSVC_SUPPRESS_WARNING(4820)
DOCTEST_MSVC_SUPPRESS_WARNING(4626)
DOCTEST_MSVC_SUPPRESS_WARNING(5027)

using namespace doctest;

struct XmlReporter : public IReporter
{
    std::ostream&                 s;
    std::vector<SubcaseSignature> subcasesStack;

    // caching pointers to objects of these types - safe to do
    const ContextOptions* opt;
    const TestCaseData*   tc;

    XmlReporter(std::ostream& in)
            : s(in) {}

    void test_run_start(const ContextOptions& o) override { opt = &o; }

    void test_run_end(const TestRunStats& /*p*/) override {}

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    void test_case_end(const CurrentTestCaseStats& /*st*/) override {}

    void subcase_start(const SubcaseSignature& /*subc*/) override {}

    void subcase_end(const SubcaseSignature& /*subc*/) override {}

    void log_assert(const AssertData& /*rb*/) override {}

    void log_message(const MessageData& /*mb*/) override {}

    void test_case_skipped(const TestCaseData& /*tc*/) override {}
};

XmlReporter r(std::cout);
DOCTEST_REGISTER_REPORTER("xml", 1, r);





TEST_CASE("better message needed") {
    CHECK(false);
    SUBCASE("subcase 1") {
        CHECK(false);
    }
    CHECK(false);
}







// doctest_0_pre_cpp11
 8.88 |   12.31 | ===> 0.0172 sec
15.02 |   19.78 | ===> 0.0238 sec

// doctest_1_pre_fwd_to_impl
 9.70 |   13.03 | ===> 0.0166 sec
15.40 |   20.15 | ===> 0.0238 sec

// doctest_2_after_fwd_to_impl
 9.91 |   12.60 | ===> 0.0135 sec
15.48 |   19.08 | ===> 0.0180 sec

// doctest_3_after_struct_madness
 9.35 |   11.70 | ===> 0.0118
14.81 |   18.55 | ===> 0.0187

// doctest_4_after_anon_ns
 9.85 |   11.91 | ===> 0.0103
14.96 |   18.75 | ===> 0.0189


https://i.stack.imgur.com/b2VBV.png



reporters issue: 138
c++11 issue: 137
threading: issue: 4 pr: 117

move to C++11 - done:
    - remove the config identifiers for C++11 features
    - use variadic templates where appropriate
    - update type lists to C++11
    - move initialization of fields from initializer lists to class bodies
    - for loops
    - lambdas
    - auto
    - easy thread safety

reporters - done:
    - can be used for listening for events
    - multiple reporters can be used
    - custom reporters can be written
    - register and choose reporters
    - list all reporters
reporters - todo:
    - output to file (or just not stdout)
    - xml output
    - xUnit reporter
    - compact reporter
    - progress reporter
    - [junit related stuff](https://github.com/ujiro99/doctest-junit-report))
    - options
        - absolutely no output on success
        - summary only
    - make sure all relevant parts of the reporters can be accessed from the doctest namespace without the need for detail
    - examples
    - documentation

thread safety - todo:
    logging with INFO - perhaps with thread_local or __declspec(thread)
    look into unique_lock
    look into these:
        http://preshing.com/20111124/always-use-a-lightweight-mutex/
        http://preshing.com/20120226/roll-your-own-lightweight-mutex/
    example
    DebugOutputWindowReporter - think about it - touching the no_colors option and its stream in the context of asserts...
    thread sanitizer tests
    documentation
        mention that subcases should still be used in a single threaded way

before release:
    coverage
    documentation
        update faq - differences with catch, thread safety, reporters/listeners

on release:
    ask in the reddit thread what from the roadmap they would like to see next
    contact https://github.com/wichtounet and tell him that xml output is present

release texts:
- further improvement of compile and link times - moved the body of absolutely every non-templated function out of the interface part of the header and into the implementation part (including implicitly generated special member functions!) (also using anonymous namespaces for most of the test runner)





fix discoverability of subcases

https://github.com/catchorg/Catch2/commit/de36b2ada6e4593a9a32c4c86cd47d4bc002b148

try to forward declare std::string and specialize the string maker for it or something like that

https://github.com/philsquared/Catch/issues/980
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

on big releases: update benchmarks, number of CI builds, differences with Catch, docs, etc.

also check if anything should be added here:
https://github.com/martinmoene/catch-lest-other-comparison

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
