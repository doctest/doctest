#ifndef DOCTEST_PARTS_PRIVATE_CONTEXT_SCOPE
#define DOCTEST_PARTS_PRIVATE_CONTEXT_SCOPE

#include "doctest/parts/public/context_scope.h"

#include <vector>

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {
DOCTEST_INTERFACE std::vector<IContextScope *> &getInfoContexts(); // for logging with INFO()
} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_CONTEXT_SCOPE
