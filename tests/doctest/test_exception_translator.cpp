// Our CI/CD helpfully runs this test with -fno-exceptions,
// which obviously means we can't test exception translation!
// -fno-exceptions is annoying to detect, but thankfully
// our runner also sets DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#if !defined(DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS)

#include <doctest/doctest.h>
#include <type_traits>

namespace {

using doctest::detail::ExceptionTranslator;

/** Example exceptions... */
struct exception1 { };
struct exception2 { };
struct exception3 { };

/** ...and their translators */
const ExceptionTranslator<exception1> translator1([](exception1) { return doctest::String("exception1"); });
const ExceptionTranslator<exception2> translator2([](exception2) { return doctest::String("exception2"); });
const ExceptionTranslator<exception3> translator3([](exception3) { return doctest::String("exception3"); });

/**
 * Utility to raise an exception, call the functor, then resolve the exception
 * I would normally have parametrized the return-type to `result_of<Fn()>::type`,
 * but this is not available on a few compilers in our toolchain, hence ignoring this.
 * */
template <typename Ex, typename Fn>
inline doctest::String with_ambient_exception(Ex e, Fn f) {
    try { throw e; } // NOLINT(hicpp-exception-baseclass)
    catch (... ) { return f(); }
}

} // namespace

TEST_CASE("Translating custom exceptions") {
    SUBCASE("Throwing exception1") {
        auto result = with_ambient_exception(exception1 { }, [] {
            auto result_ = doctest::String();
            REQUIRE( translator1.translate(result_));
            REQUIRE(!translator2.translate(result_));
            REQUIRE(!translator3.translate(result_));
            return result_;
        });

        CHECK(result == "exception1");
    }

    SUBCASE("Throwing exception1") {
        auto result = with_ambient_exception(exception2 { }, [] {
            auto result_ = doctest::String();
            REQUIRE(!translator1.translate(result_));
            REQUIRE( translator2.translate(result_));
            REQUIRE(!translator3.translate(result_));
            return result_;
        });

        CHECK(result == "exception2");
    }

    SUBCASE("Throwing exception1") {
        auto result = with_ambient_exception(exception3 { }, [] {
            auto result_ = doctest::String();
            REQUIRE(!translator1.translate(result_));
            REQUIRE(!translator2.translate(result_));
            REQUIRE( translator3.translate(result_));
            return result_;
        });

        CHECK(result == "exception3");
    }
}

#endif // !defined(DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS)
