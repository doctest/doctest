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
