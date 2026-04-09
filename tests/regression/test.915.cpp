// mingw has issues with thread_local in our setup
#if defined(__MINGW32__) || defined(__MINGW64__)
#define DOCTEST_THREAD_LOCAL /*nothing*/
#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <chrono>
#include <thread>

namespace {

void delayed_error() {
    const auto delay = std::chrono::milliseconds(50);
    std::this_thread::sleep_for(delay);
    CHECK(false);
}

} // namespace

TEST_CASE("Throwing an exception from a thread inside a subcase" * doctest::should_fail()) {
    std::thread t;
    SUBCASE("Subcase level 1") {
        t = std::thread(delayed_error);
        // Intentionally not joining `t` here
    }

    t.join();
}
