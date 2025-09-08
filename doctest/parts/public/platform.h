// not using __APPLE__ because... this is how Catch does it
#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#define DOCTEST_PLATFORM_MAC
#elif defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#define DOCTEST_PLATFORM_IPHONE
#elif defined(_WIN32)
#define DOCTEST_PLATFORM_WINDOWS
#elif defined(__wasi__)
#define DOCTEST_PLATFORM_WASI
#else // DOCTEST_PLATFORM
#define DOCTEST_PLATFORM_LINUX
#endif // DOCTEST_PLATFORM
