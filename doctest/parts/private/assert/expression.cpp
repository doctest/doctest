#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

ExpressionDecomposer::ExpressionDecomposer(assertType::Enum at)
        : m_at(at) {}

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
