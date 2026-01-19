#ifndef DOCTEST_PARTS_PRIVATE_SIGNALS
#define DOCTEST_PARTS_PRIVATE_SIGNALS

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/assert/handler.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

#if !defined(DOCTEST_CONFIG_POSIX_SIGNALS) && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
    struct FatalConditionHandler
    {
        static void reset();
        static void allocateAltStackMem();
        static void freeAltStackMem();
    };
#else // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

#ifdef DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        DWORD id;
        const char* name;
    };

    struct FatalConditionHandler
    {
        static LONG CALLBACK handleException(PEXCEPTION_POINTERS ExceptionInfo);
        static void allocateAltStackMem();
        static void freeAltStackMem();

        FatalConditionHandler();

        static void reset();

        ~FatalConditionHandler();

    private:
        static UINT         prev_error_mode_1;
        static int          prev_error_mode_2;
        static unsigned int prev_abort_behavior;
        static int          prev_report_mode;
        static _HFILE       prev_report_file;
        static void (DOCTEST_CDECL *prev_sigabrt_handler)(int);
        static std::terminate_handler original_terminate_handler;
        static bool isSet;
        static ULONG guaranteeSize;
        static LPTOP_LEVEL_EXCEPTION_FILTER previousTop;
    };

#else // DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        int         id;
        const char* name;
    };

    struct FatalConditionHandler
    {
        static bool             isSet;
        static struct sigaction oldSigActions[6];
        static stack_t          oldSigStack;
        static size_t           altStackSize;
        static char*            altStackMem;

        static void handleSignal(int sig);

        static void allocateAltStackMem();

        static void freeAltStackMem();

        FatalConditionHandler();

        ~FatalConditionHandler();
        static void reset();
    };

#endif // DOCTEST_PLATFORM_WINDOWS
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_SIGNALS
