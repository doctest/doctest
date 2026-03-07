#include <doctest/doctest.h>
#include <set>
#include <string>

namespace platform {

#if defined(DOCTEST_PLATFORM_MAC)
constexpr auto mac = true;
#else
constexpr auto mac = false;
#endif // DOCTEST_PLATFORM_MAC

#if defined(DOCTEST_PLATFORM_IPHONE)
constexpr auto iphone = true;
#else
constexpr auto iphone = false;
#endif // DOCTEST_PLATFORM_IPHONE

#if defined(DOCTEST_PLATFORM_WINDOWS)
constexpr auto windows = true;
#else
constexpr auto windows = false;
#endif // DOCTEST_PLATFORM_WINDOWS

#if defined(DOCTEST_PLATFORM_WASI)
constexpr auto wasi = true;
#else
constexpr auto wasi = false;
#endif // DOCTEST_PLATFORM_WASI

// 'linux' is an established pre-precessor define
#if defined(DOCTEST_PLATFORM_LINUX)
constexpr auto linux_ = true;
#else
constexpr auto linux_ = false;
#endif // DOCTEST_PLATFORM_LINUX

} // namespace platform

TEST_CASE("Exactly one platform is detected") {
    CAPTURE(platform::mac);
    CAPTURE(platform::iphone);
    CAPTURE(platform::windows);
    CAPTURE(platform::wasi);
    CAPTURE(platform::linux_);

    const auto detected = std::multiset<bool>{
        platform::mac,
        platform::iphone,
        platform::windows,
        platform::wasi,
        platform::linux_,
    };

    CHECK(detected.count(true) == 1);
}

TEST_CASE("Detected platform matches what build-system informed us about") {
#if !defined(CMAKE_SYSTEM_NAME)
#error Build system must define CMAKE_SYSTEM_NAME to run this test!
    const auto cmake_system_name = std::string("<unknown>");
#else
    const auto cmake_system_name = std::string(CMAKE_SYSTEM_NAME);
    CAPTURE(cmake_system_name);
#endif

    CHECK(platform::mac == (cmake_system_name == "Darwin"));
    CHECK(platform::iphone == (cmake_system_name == "iOS"));
    CHECK(platform::windows == (cmake_system_name == "Windows"));
    CHECK(platform::wasi == (cmake_system_name == "Emscripten"));
    CHECK(platform::linux_ == (cmake_system_name == "Linux"));
}
