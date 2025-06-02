#define DOCTEST_CONFIG_EVALUATE_ASSERTS_EVEN_WHEN_DISABLED
#define DOCTEST_CONFIG_IMPLEMENT
#define TEST_FLIP 1
#include "assert_returns_disabled.cpp"

int main(int argc, char** argv) {
    // Introduce a volatile variable here to avoid the main being optimized away in certain
    // configurations resulting in a broken binary (observed for XCode >= 16.2 on ARM64).
    volatile int dummy = 0;

    return doctest::Context(argc, argv).run() + dummy;
}
