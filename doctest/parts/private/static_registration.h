#ifndef DOCTEST_PARTS_PRIVATE_STATIC_REGISTRATION
#define DOCTEST_PARTS_PRIVATE_STATIC_REGISTRATION

#include "doctest/parts/public/config.h"

// Registrations are intentionally performed during static initialization. In a
// debug-CRT build these objects can still be alive when a host DLL asks the CRT
// for a leak dump during its detach notification. Mark only allocations made
// while registering doctest metadata as ignored, so that such a snapshot does
// not mistake framework lifetime state for a user allocation leak.
#if DOCTEST_MSVC && defined(_DEBUG)
#include <crtdbg.h>
#endif

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {
namespace detail {

class StaticRegistrationMemoryGuard {
public:
    StaticRegistrationMemoryGuard() noexcept {
#if DOCTEST_MSVC && defined(_DEBUG)
        m_flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        _CrtSetDbgFlag(m_flags & ~_CRTDBG_ALLOC_MEM_DF);
#endif
    }

    ~StaticRegistrationMemoryGuard() {
#if DOCTEST_MSVC && defined(_DEBUG)
        _CrtSetDbgFlag(m_flags);
#endif
    }

private:
#if DOCTEST_MSVC && defined(_DEBUG)
    int m_flags;
#endif
};

} // namespace detail
} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_STATIC_REGISTRATION
