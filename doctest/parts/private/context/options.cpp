#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

namespace doctest {

    const ContextOptions* getContextOptions() { return DOCTEST_BRANCH_ON_DISABLED(nullptr, detail::g_cs); }

} // namespace doctest
