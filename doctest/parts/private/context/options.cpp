#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {

    const ContextOptions* getContextOptions() { return DOCTEST_BRANCH_ON_DISABLED(nullptr, detail::g_cs); }

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
