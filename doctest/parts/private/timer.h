#ifndef DOCTEST_PARTS_PRIVATE_TIMER
#define DOCTEST_PARTS_PRIVATE_TIMER

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

uint64_t getCurrentTicks();

struct Timer {
    void start();
    unsigned int getElapsedMicroseconds() const;
    double getElapsedSeconds() const;

private:
    uint64_t m_ticks = 0;
};

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_TIMER
