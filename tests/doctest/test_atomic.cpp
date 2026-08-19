#include <doctest/doctest.h>
#include <doctest/parts/private/atomic.h>

#include <thread>

namespace {

// The same type the assertion counters use, so this test follows them if the type changes.
typedef decltype(doctest::TestRunStats::numAsserts) counter_t;

using Counter = doctest::detail::MultiLaneAtomic<counter_t>;

// 2^30. Four of these add up to 2^32, two of them to 2^31. Kept as long long so that a cast
// to the counter type is a real conversion whether that type is 32 or 64 bit wide, which
// keeps gcc -Wuseless-cast quiet in both cases.
const long long quarter_of_2_32 = 1073741824LL;

// Widening goes through a template so gcc does not see a cast to the type the operand
// already has, which -Wuseless-cast rejects.
template <typename T>
unsigned long long widen(T value) {
    return static_cast<unsigned long long>(value);
}

// Adds to the counter from its own thread. Each thread is assigned its own lane, so two
// calls leave two lanes holding a partial count and load() has to sum them. This only holds
// in the default configuration: DOCTEST_CONFIG_NO_MULTITHREADING and
// DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS collapse MultiLaneAtomic to a single plain atomic.
void addFromOwnThread(Counter &counter, long long amount) {
    const counter_t value = static_cast<counter_t>(amount);
    std::thread worker([&counter, value]() noexcept { counter.fetch_add(value); });
    worker.join();
}

TEST_CASE("MultiLaneAtomic keeps one lane exact up to 2^32") {
    Counter counter;
    const counter_t amount = static_cast<counter_t>(quarter_of_2_32);

    // All four go to the same lane, so a 32 bit counter wraps back to zero here.
    counter.fetch_add(amount);
    counter.fetch_add(amount);
    counter.fetch_add(amount);
    counter.fetch_add(amount);

    CHECK(widen(counter.load()) == 4ull * widen(quarter_of_2_32));
}

TEST_CASE("MultiLaneAtomic can hold a value above 2^32") {
    Counter counter;
    counter = static_cast<counter_t>(5000000000LL);

    CHECK(widen(counter.load()) == 5000000000ull);
}

TEST_CASE("MultiLaneAtomic sums two lanes past 2^31") {
    Counter counter;

    addFromOwnThread(counter, quarter_of_2_32);
    addFromOwnThread(counter, quarter_of_2_32);

    // With a 32 bit counter the addition inside load() overflows, so under
    // -fsanitize=undefined this case aborts instead of reporting a failed check.
    CHECK(widen(counter.load()) == 2ull * widen(quarter_of_2_32));
}

TEST_CASE("the assertion counter type is unsigned and at least 64 bit") {
    CHECK(sizeof(counter_t) >= 8);

    // All bits set is the maximum for an unsigned type and -1 for a signed one.
    CHECK(static_cast<counter_t>(-1LL) > static_cast<counter_t>(0LL));
}

} // namespace
