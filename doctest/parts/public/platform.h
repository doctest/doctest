// IWYU pragma: private, include "public/config.h"
#ifndef DOCTEST_PARTS_PUBLIC_PLATFORM
#define DOCTEST_PARTS_PUBLIC_PLATFORM

#if defined(__APPLE__)
// Apple detection taken from Catch2 codebase
// For <TargetConditionals.h> information:
//   https://github.com/swiftlang/swift-corelibs-foundation/blob/release/5.10/CoreFoundation/Base.subproj/SwiftRuntime/TargetConditionals.h
#include <TargetConditionals.h>
#if (defined(TARGET_OS_MAC) && TARGET_OS_MAC == 1) || (defined(TARGET_OS_OSX) && TARGET_OS_OSX == 1)
#define DOCTEST_PLATFORM_MAC

#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE == 1
#define DOCTEST_PLATFORM_IPHONE
#endif

#elif defined(WIN32) || defined(_WIN32)
#define DOCTEST_PLATFORM_WINDOWS

#elif defined(__wasi__)
#define DOCTEST_PLATFORM_WASI

#else // defined(linux) || defined(__linux) // defined(__linux__)
#define DOCTEST_PLATFORM_LINUX
#endif // DOCTEST_PLATFORM

#endif // DOCTEST_PARTS_PUBLIC_PLATFORM
