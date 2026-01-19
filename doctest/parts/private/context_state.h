#ifndef DOCTEST_PARTS_PRIVATE_CONTEXT_STATE
#define DOCTEST_PARTS_PRIVATE_CONTEXT_STATE

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/timer.h"
#include "doctest/parts/private/atomic.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    // this holds both parameters from the command line and runtime data for tests
    struct ContextState : ContextOptions, TestRunStats, CurrentTestCaseStats
    {
        MultiLaneAtomic<int> numAssertsCurrentTest_atomic;
        MultiLaneAtomic<int> numAssertsFailedCurrentTest_atomic;

        std::vector<std::vector<String>> filters = decltype(filters)(9); // 9 different filters

        std::vector<IReporter*> reporters_currently_used;

        assert_handler ah = nullptr;

        Timer timer;

        std::vector<String> stringifiedContexts; // logging from INFO() due to an exception

        // stuff for subcases
        bool reachedLeaf;
        std::vector<SubcaseSignature> subcaseStack;
        std::vector<SubcaseSignature> nextSubcaseStack;
        std::unordered_set<unsigned long long> fullyTraversedSubcases;
        size_t currentSubcaseDepth;
        Atomic<bool> shouldLogCurrentException;

        void resetRunData();

        void finalizeTestCaseData();
    };

    extern ContextState* g_cs;

    // used to avoid locks for the debug output
    // TODO: figure out if this is indeed necessary/correct - seems like either there still
    // could be a race or that there wouldn't be a race even if using the context directly
    extern DOCTEST_THREAD_LOCAL bool g_no_colors;

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_CONTEXT_STATE
