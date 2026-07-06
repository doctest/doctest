#include <doctest/doctest.h>
#include <doctest/parts/private/exception_translator.h>
#include <string>
#include <type_traits>

namespace {

template <typename T>
doctest::String translate(T failure) {
    static_cast<void>(failure);
    try {
        // NOLINTNEXTLINE
        throw failure;
    } catch (...) { return doctest::detail::translateActiveException(); }
}

} // namespace

TEST_CASE("Throwing a null const char *") {
    const char *failure = nullptr;
    const auto reason = translate(failure);
    CHECK(reason == "(nullptr)");
}

TEST_CASE("Throwing an untyped nullptr") {
    const auto reason = translate(nullptr);
#if defined(_MSC_VER) && !defined(__clang__)
    /* cl / msvs are a bit weird when handling `std::nullptr_t` */
    CHECK(reason == "unknown exception");
#else
    CHECK(reason == "(nullptr)");
#endif
}
