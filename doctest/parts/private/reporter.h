#ifndef DOCTEST_PARTS_PRIVATE_REPORTER
#define DOCTEST_PARTS_PRIVATE_REPORTER

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace {
    // the int (priority) is part of the key for automatic sorting - sadly one can register a
    // reporter with a duplicate name and a different priority but hopefully that won't happen often :|
    using reporterMap = std::map<std::pair<int, String>, detail::reporterCreatorFunc>;

    reporterMap& getReporters() {
        static reporterMap data;
        return data;
    }
    reporterMap& getListeners() {
        static reporterMap data;
        return data;
    }
} // namespace

#define DOCTEST_ITERATE_THROUGH_REPORTERS(function, ...)                                           \
    for(auto& curr_rep : g_cs->reporters_currently_used)                                           \
    curr_rep->function(__VA_ARGS__)

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTER
