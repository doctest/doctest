#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {
namespace {

    using namespace detail;

#if !defined(DOCTEST_CONFIG_POSIX_SIGNALS) && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
    struct FatalConditionHandler
    {
        static void reset() {}
        static void allocateAltStackMem() {}
        static void freeAltStackMem() {}
    };
#else // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

#ifdef DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        DWORD id;
        const char* name;
    };
    // There is no 1-1 mapping between signals and windows exceptions.
    // Windows can easily distinguish between SO and SigSegV,
    // but SigInt, SigTerm, etc are handled differently.
    SignalDefs signalDefs[] = {
            {static_cast<DWORD>(EXCEPTION_ILLEGAL_INSTRUCTION),
             "SIGILL - Illegal instruction signal"},
            {static_cast<DWORD>(EXCEPTION_STACK_OVERFLOW), "SIGSEGV - Stack overflow"},
            {static_cast<DWORD>(EXCEPTION_ACCESS_VIOLATION),
             "SIGSEGV - Segmentation violation signal"},
            {static_cast<DWORD>(EXCEPTION_INT_DIVIDE_BY_ZERO), "Divide by zero error"},
    };

    struct FatalConditionHandler
    {
        static LONG CALLBACK handleException(PEXCEPTION_POINTERS ExceptionInfo) {
            // Multiple threads may enter this filter/handler at once. We want the error message to be printed on the
            // console just once no matter how many threads have crashed.
            DOCTEST_DECLARE_STATIC_MUTEX(mutex)
            static bool execute = true;
            {
                DOCTEST_LOCK_MUTEX(mutex)
                if(execute) {
                    bool reported = false;
                    for(size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                        if(ExceptionInfo->ExceptionRecord->ExceptionCode == signalDefs[i].id) {
                            reportFatal(signalDefs[i].name);
                            reported = true;
                            break;
                        }
                    }
                    if(reported == false)
                        reportFatal("Unhandled SEH exception caught");
                    if(isDebuggerActive() && !g_cs->no_breaks)
                        DOCTEST_BREAK_INTO_DEBUGGER();
                }
                execute = false;
            }
            std::exit(EXIT_FAILURE);
        }

        static void allocateAltStackMem() {}
        static void freeAltStackMem() {}

        FatalConditionHandler() {
            isSet = true;
            // 32k seems enough for doctest to handle stack overflow,
            // but the value was found experimentally, so there is no strong guarantee
            guaranteeSize = 32 * 1024;
            // Register an unhandled exception filter
            previousTop = SetUnhandledExceptionFilter(handleException);
            // Pass in guarantee size to be filled
            SetThreadStackGuarantee(&guaranteeSize);

            // On Windows uncaught exceptions from another thread, exceptions from
            // destructors, or calls to std::terminate are not a SEH exception

            // The terminal handler gets called when:
            // - std::terminate is called FROM THE TEST RUNNER THREAD
            // - an exception is thrown from a destructor FROM THE TEST RUNNER THREAD
            original_terminate_handler = std::get_terminate();
            std::set_terminate([]() DOCTEST_NOEXCEPT {
                reportFatal("Terminate handler called");
                if(isDebuggerActive() && !g_cs->no_breaks)
                    DOCTEST_BREAK_INTO_DEBUGGER();
                std::exit(EXIT_FAILURE); // explicitly exit - otherwise the SIGABRT handler may be called as well
            });

            // SIGABRT is raised when:
            // - std::terminate is called FROM A DIFFERENT THREAD
            // - an exception is thrown from a destructor FROM A DIFFERENT THREAD
            // - an uncaught exception is thrown FROM A DIFFERENT THREAD
            prev_sigabrt_handler = std::signal(SIGABRT, [](int signal) DOCTEST_NOEXCEPT {
                if(signal == SIGABRT) {
                    reportFatal("SIGABRT - Abort (abnormal termination) signal");
                    if(isDebuggerActive() && !g_cs->no_breaks)
                        DOCTEST_BREAK_INTO_DEBUGGER();
                    std::exit(EXIT_FAILURE);
                }
            });

            // The following settings are taken from google test, and more
            // specifically from UnitTest::Run() inside of gtest.cc

            // the user does not want to see pop-up dialogs about crashes
            prev_error_mode_1 = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
                                             SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
            // This forces the abort message to go to stderr in all circumstances.
            prev_error_mode_2 = _set_error_mode(_OUT_TO_STDERR);
            // In the debug version, Visual Studio pops up a separate dialog
            // offering a choice to debug the aborted program - we want to disable that.
            prev_abort_behavior = _set_abort_behavior(0x0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
            // In debug mode, the Windows CRT can crash with an assertion over invalid
            // input (e.g. passing an invalid file descriptor). The default handling
            // for these assertions is to pop up a dialog and wait for user input.
            // Instead ask the CRT to dump such assertions to stderr non-interactively.
            prev_report_mode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
            prev_report_file = _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        }

        static void reset() {
            if(isSet) {
                // Unregister handler and restore the old guarantee
                SetUnhandledExceptionFilter(previousTop);
                SetThreadStackGuarantee(&guaranteeSize);
                std::set_terminate(original_terminate_handler);
                std::signal(SIGABRT, prev_sigabrt_handler);
                SetErrorMode(prev_error_mode_1);
                _set_error_mode(prev_error_mode_2);
                _set_abort_behavior(prev_abort_behavior, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
                static_cast<void>(_CrtSetReportMode(_CRT_ASSERT, prev_report_mode));
                static_cast<void>(_CrtSetReportFile(_CRT_ASSERT, prev_report_file));
                isSet = false;
            }
        }

        ~FatalConditionHandler() { reset(); }

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

    UINT         FatalConditionHandler::prev_error_mode_1;
    int          FatalConditionHandler::prev_error_mode_2;
    unsigned int FatalConditionHandler::prev_abort_behavior;
    int          FatalConditionHandler::prev_report_mode;
    _HFILE       FatalConditionHandler::prev_report_file;
    void (DOCTEST_CDECL *FatalConditionHandler::prev_sigabrt_handler)(int);
    std::terminate_handler FatalConditionHandler::original_terminate_handler;
    bool FatalConditionHandler::isSet = false;
    ULONG FatalConditionHandler::guaranteeSize = 0;
    LPTOP_LEVEL_EXCEPTION_FILTER FatalConditionHandler::previousTop = nullptr;

#else // DOCTEST_PLATFORM_WINDOWS

    struct SignalDefs
    {
        int         id;
        const char* name;
    };
    SignalDefs signalDefs[] = {{SIGINT, "SIGINT - Terminal interrupt signal"},
                               {SIGILL, "SIGILL - Illegal instruction signal"},
                               {SIGFPE, "SIGFPE - Floating point error signal"},
                               {SIGSEGV, "SIGSEGV - Segmentation violation signal"},
                               {SIGTERM, "SIGTERM - Termination request signal"},
                               {SIGABRT, "SIGABRT - Abort (abnormal termination) signal"}};

    struct FatalConditionHandler
    {
        static bool             isSet;
        static struct sigaction oldSigActions[DOCTEST_COUNTOF(signalDefs)];
        static stack_t          oldSigStack;
        static size_t           altStackSize;
        static char*            altStackMem;

        static void handleSignal(int sig) {
            const char* name = "<unknown signal>";
            for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                SignalDefs& def = signalDefs[i];
                if(sig == def.id) {
                    name = def.name;
                    break;
                }
            }
            reset();
            reportFatal(name);
            raise(sig);
        }

        static void allocateAltStackMem() {
            altStackMem = new char[altStackSize];
        }

        static void freeAltStackMem() {
            delete[] altStackMem;
        }

        FatalConditionHandler() {
            isSet = true;
            stack_t sigStack;
            sigStack.ss_sp    = altStackMem;
            sigStack.ss_size  = altStackSize;
            sigStack.ss_flags = 0;
            sigaltstack(&sigStack, &oldSigStack);
            struct sigaction sa = {};
            sa.sa_handler       = handleSignal;
            sa.sa_flags         = SA_ONSTACK;
            for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                sigaction(signalDefs[i].id, &sa, &oldSigActions[i]);
            }
        }

        ~FatalConditionHandler() { reset(); }
        static void reset() {
            if(isSet) {
                // Set signals back to previous values -- hopefully nobody overwrote them in the meantime
                for(std::size_t i = 0; i < DOCTEST_COUNTOF(signalDefs); ++i) {
                    sigaction(signalDefs[i].id, &oldSigActions[i], nullptr);
                }
                // Return the old stack
                sigaltstack(&oldSigStack, nullptr);
                isSet = false;
            }
        }
    };

    bool             FatalConditionHandler::isSet = false;
    struct sigaction FatalConditionHandler::oldSigActions[DOCTEST_COUNTOF(signalDefs)] = {};
    stack_t          FatalConditionHandler::oldSigStack = {};
    size_t           FatalConditionHandler::altStackSize = 4 * SIGSTKSZ;
    char*            FatalConditionHandler::altStackMem = nullptr;

#endif // DOCTEST_PLATFORM_WINDOWS
#endif // DOCTEST_CONFIG_POSIX_SIGNALS || DOCTEST_CONFIG_WINDOWS_SEH

} // namespace
} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE