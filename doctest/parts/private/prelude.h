#ifndef DOCTEST_PARTS_PRIVATE_PRELUDE
#define DOCTEST_PARTS_PRIVATE_PRELUDE

#include "doctest/parts/doctest_fwd.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

// required includes - will go only in one translation unit!
#include <ctime>
#include <cmath>
#include <climits>
// borland (Embarcadero) compiler requires math.h and not cmath -
// https://github.com/doctest/doctest/pull/37
#ifdef __BORLANDC__
#include <math.h>
#endif // __BORLANDC__
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <utility>
#include <fstream>
#include <sstream>
#ifndef DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
#include <iostream>
#endif // DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
#include <algorithm>
#include <iomanip>
#include <vector>
#ifndef DOCTEST_CONFIG_NO_MULTITHREADING
#include <atomic>
#include <mutex>
#define DOCTEST_DECLARE_MUTEX(name) std::mutex name;
#define DOCTEST_DECLARE_STATIC_MUTEX(name) static DOCTEST_DECLARE_MUTEX(name)
#define DOCTEST_LOCK_MUTEX(name) const std::lock_guard<std::mutex> DOCTEST_ANONYMOUS(DOCTEST_ANON_LOCK_)(name);
#else // DOCTEST_CONFIG_NO_MULTITHREADING
#define DOCTEST_DECLARE_MUTEX(name)
#define DOCTEST_DECLARE_STATIC_MUTEX(name)
#define DOCTEST_LOCK_MUTEX(name)
#endif // DOCTEST_CONFIG_NO_MULTITHREADING
#include <set>
#include <map>
#include <unordered_set>
#include <exception>
#include <stdexcept>
#include <cfloat>
#include <cctype>
#include <cstdint>
#include <string>

#ifdef DOCTEST_PLATFORM_MAC
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#endif // DOCTEST_PLATFORM_MAC

#ifndef DOCTEST_PLATFORM_WINDOWS
#include <sys/time.h>
#include <unistd.h>
#endif // DOCTEST_PLATFORM_WINDOWS

// counts the number of elements in a C array
#define DOCTEST_COUNTOF(x) (sizeof(x) / sizeof(x[0]))

#ifdef DOCTEST_CONFIG_DISABLE
#define DOCTEST_BRANCH_ON_DISABLED(if_disabled, if_not_disabled) if_disabled
#else // DOCTEST_CONFIG_DISABLE
#define DOCTEST_BRANCH_ON_DISABLED(if_disabled, if_not_disabled) if_not_disabled
#endif // DOCTEST_CONFIG_DISABLE

#ifndef DOCTEST_THREAD_LOCAL
#if defined(DOCTEST_CONFIG_NO_MULTITHREADING) || DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_THREAD_LOCAL
#else // DOCTEST_MSVC
#define DOCTEST_THREAD_LOCAL thread_local
#endif // DOCTEST_MSVC
#endif // DOCTEST_THREAD_LOCAL

#ifndef DOCTEST_MULTI_LANE_ATOMICS_THREAD_LANES
#define DOCTEST_MULTI_LANE_ATOMICS_THREAD_LANES 32
#endif

#ifndef DOCTEST_MULTI_LANE_ATOMICS_CACHE_LINE_SIZE
#define DOCTEST_MULTI_LANE_ATOMICS_CACHE_LINE_SIZE 64
#endif

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_PRELUDE
