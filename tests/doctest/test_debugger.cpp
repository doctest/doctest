#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#if defined(DOCTEST_PLATFORM_WINDOWS)
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wnonportable-system-include-path")
#include <windows.h>
DOCTEST_CLANG_SUPPRESS_WARNING_POP

#elif defined(DOCTEST_PLATFORM_LINUX)
#include <csignal>

#else
#error No test implemented for this platform!
#endif
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace debugger {
bool signal_caught = false;

#if defined(DOCTEST_PLATFORM_WINDOWS)
extern "C" inline LONG WINAPI veh(PEXCEPTION_POINTERS info) {
    if (info->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
        signal_caught = true;
#if defined(_M_AMD64)
        info->ContextRecord->Rip++;
#elif defined(_M_IX86)
        info->ContextRecord->Eip++;
#elif defined(_M_ARM64)
        info->ContextRecord->Pc += 4;
#endif

        return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}
void *handle = nullptr;
#endif

inline void setup() {
    signal_caught = false;
#if defined(DOCTEST_PLATFORM_WINDOWS)
    REQUIRE(handle == nullptr);
    handle = AddVectoredExceptionHandler(1, veh);

#elif defined(DOCTEST_PLATFORM_LINUX)
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wdisabled-macro-expansion") /* sa_handler is a self-referential macro */
    struct sigaction sa = {};
    sa.sa_handler = [](int) { signal_caught = true; };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTRAP, &sa, nullptr);
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif
}

inline void cleanup() {
#if defined(DOCTEST_PLATFORM_WINDOWS)
    if (handle != nullptr) {
        RemoveVectoredExceptionHandler(handle);
        handle = nullptr;
    }
#elif defined(DOCTEST_PLATFORM_LINUX)
#if DOCTEST_CLANG >= DOCTEST_COMPILER(5, 0, 0)
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(
        "-Wzero-as-null-pointer-constant"
    ) /* SIG_DFL=0, but is the correct way to de-register a handler */
#endif

    static_cast<void>(signal(SIGTRAP, SIG_DFL));

#if DOCTEST_CLANG >= DOCTEST_COMPILER(5, 0, 0)
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif
#endif
}

} // namespace debugger

TEST_CASE("Manually triggering the debugger") {
    // Unfortunately, we cannot test doctest::detail::isDebuggerActive with this strategy

    debugger::setup();
    CHECK(debugger::signal_caught == false);
    DOCTEST_BREAK_INTO_DEBUGGER();
    CHECK(debugger::signal_caught == true);
    debugger::cleanup();
}
