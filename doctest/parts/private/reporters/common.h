#ifndef DOCTEST_PARTS_PRIVATE_REPORTERS_COMMON
#define DOCTEST_PARTS_PRIVATE_REPORTERS_COMMON

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_OPTIONS_PREFIX
#define DOCTEST_CONFIG_OPTIONS_PREFIX "dt-"
#endif

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {

    void fulltext_log_assert_to_stream(std::ostream& s, const AssertData& rb);

} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_REPORTERS_COMMON
