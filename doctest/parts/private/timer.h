#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

namespace timer_large_integer
{

#if defined(DOCTEST_PLATFORM_WINDOWS)
    using type = ULONGLONG;
#else // DOCTEST_PLATFORM_WINDOWS
    using type = std::uint64_t;
#endif // DOCTEST_PLATFORM_WINDOWS
}

using ticks_t = timer_large_integer::type;

#ifdef DOCTEST_CONFIG_GETCURRENTTICKS
    ticks_t getCurrentTicks() { return DOCTEST_CONFIG_GETCURRENTTICKS(); }
#elif defined(DOCTEST_PLATFORM_WINDOWS)
    ticks_t getCurrentTicks() {
        static LARGE_INTEGER hz = { {0} }, hzo = { {0} };
        if(!hz.QuadPart) {
            QueryPerformanceFrequency(&hz);
            QueryPerformanceCounter(&hzo);
        }
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        return ((t.QuadPart - hzo.QuadPart) * LONGLONG(1000000)) / hz.QuadPart;
    }
#else  // DOCTEST_PLATFORM_WINDOWS
    ticks_t getCurrentTicks() {
        timeval t;
        gettimeofday(&t, nullptr);
        return static_cast<ticks_t>(t.tv_sec) * 1000000 + static_cast<ticks_t>(t.tv_usec);
    }
#endif // DOCTEST_PLATFORM_WINDOWS

    struct Timer
    {
        void         start() { m_ticks = getCurrentTicks(); }
        unsigned int getElapsedMicroseconds() const {
            return static_cast<unsigned int>(getCurrentTicks() - m_ticks);
        }
        //unsigned int getElapsedMilliseconds() const {
        //    return static_cast<unsigned int>(getElapsedMicroseconds() / 1000);
        //}
        double getElapsedSeconds() const { return static_cast<double>(getCurrentTicks() - m_ticks) / 1000000.0; }

    private:
        ticks_t m_ticks = 0;
    };

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
