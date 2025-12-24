#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {
#ifdef DOCTEST_IS_DEBUGGER_ACTIVE
    bool isDebuggerActive() { return DOCTEST_IS_DEBUGGER_ACTIVE(); }
#else // DOCTEST_IS_DEBUGGER_ACTIVE
#ifdef DOCTEST_PLATFORM_LINUX
    class ErrnoGuard {
    public:
        ErrnoGuard() : m_oldErrno(errno) {}
        ~ErrnoGuard() { errno = m_oldErrno; }
    private:
        int m_oldErrno;
    };
    // See the comments in Catch2 for the reasoning behind this implementation:
    // https://github.com/catchorg/Catch2/blob/v2.13.1/include/internal/catch_debugger.cpp#L79-L102
    bool isDebuggerActive() {
        ErrnoGuard guard;
        std::ifstream in("/proc/self/status");
        for(std::string line; std::getline(in, line);) {
            static const int PREFIX_LEN = 11;
            if(line.compare(0, PREFIX_LEN, "TracerPid:\t") == 0) {
                return line.length() > PREFIX_LEN && line[PREFIX_LEN] != '0';
            }
        }
        return false;
    }
#elif defined(DOCTEST_PLATFORM_MAC)
    // The following function is taken directly from the following technical note:
    // https://developer.apple.com/library/archive/qa/qa1361/_index.html
    // Returns true if the current process is being debugged (either
    // running under the debugger or has a debugger attached post facto).
    bool isDebuggerActive() {
        int        mib[4];
        kinfo_proc info;
        size_t     size;
        // Initialize the flags so that, if sysctl fails for some bizarre
        // reason, we get a predictable result.
        info.kp_proc.p_flag = 0;
        // Initialize mib, which tells sysctl the info we want, in this case
        // we're looking for information about a specific process ID.
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();
        // Call sysctl.
        size = sizeof(info);
        if(sysctl(mib, DOCTEST_COUNTOF(mib), &info, &size, 0, 0) != 0) {
            std::cerr << "\nCall to sysctl failed - unable to determine if debugger is active **\n";
            return false;
        }
        // We're being debugged if the P_TRACED flag is set.
        return ((info.kp_proc.p_flag & P_TRACED) != 0);
    }
#elif DOCTEST_MSVC || defined(__MINGW32__) || defined(__MINGW64__)
    bool isDebuggerActive() { return ::IsDebuggerPresent() != 0; }
#else
    bool isDebuggerActive() { return false; }
#endif // Platform
#endif // DOCTEST_IS_DEBUGGER_ACTIVE
} // detail
} // doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
