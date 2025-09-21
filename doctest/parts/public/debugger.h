#ifndef DOCTEST_BREAK_INTO_DEBUGGER
// should probably take a look at https://github.com/scottt/debugbreak
#ifdef DOCTEST_PLATFORM_LINUX
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
// Break at the location of the failing check if possible
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("int $3\n" : :) // NOLINT(hicpp-no-assembler)
#else
#include <signal.h>
#define DOCTEST_BREAK_INTO_DEBUGGER() raise(SIGTRAP)
#endif
#elif defined(DOCTEST_PLATFORM_MAC)
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__i386)
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("int $3\n" : :) // NOLINT(hicpp-no-assembler)
#elif defined(__ppc__) || defined(__ppc64__)
// https://www.cocoawithlove.com/2008/03/break-into-debugger.html
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("li r0, 20\nsc\nnop\nli r0, 37\nli r4, 2\nsc\nnop\n": : : "memory","r0","r3","r4") // NOLINT(hicpp-no-assembler)
#else
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("brk #0"); // NOLINT(hicpp-no-assembler)
#endif
#elif DOCTEST_MSVC
#define DOCTEST_BREAK_INTO_DEBUGGER() __debugbreak()
#elif defined(__MINGW32__)
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wredundant-decls")
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
DOCTEST_GCC_SUPPRESS_WARNING_POP
#define DOCTEST_BREAK_INTO_DEBUGGER() ::DebugBreak()
#else // linux
#define DOCTEST_BREAK_INTO_DEBUGGER() (static_cast<void>(0))
#endif // linux
#endif // DOCTEST_BREAK_INTO_DEBUGGER

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {
    DOCTEST_INTERFACE bool isDebuggerActive();
} // detail
} // doctest

#endif
