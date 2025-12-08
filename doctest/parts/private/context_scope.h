#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {
    extern DOCTEST_THREAD_LOCAL std::vector<IContextScope*> g_infoContexts; // for logging with INFO()
}
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
