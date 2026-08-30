#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {

const ContextOptions *getContextOptions() {
    return DOCTEST_BRANCH_ON_DISABLED(nullptr, detail::g_cs);
}

const char *current_test_name() {
    DOCTEST_BRANCH_ON_DISABLED(return "", {
        if (!detail::g_cs || !detail::g_cs->currentTest)
            return "";
        if (!detail::g_cs->currentTest->m_full_name.empty())
            return detail::g_cs->currentTest->m_full_name.c_str();
        return detail::g_cs->currentTest->m_name;
    });
}

const char *current_subcase_name() {
    DOCTEST_BRANCH_ON_DISABLED(return "", {
        if (!detail::g_cs || detail::g_cs->currentSubcaseNames.empty())
            return "";
        return detail::g_cs->currentSubcaseNames.back();
    });
}

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
