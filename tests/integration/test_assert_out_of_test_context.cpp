#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <utils/executable.h>
#include <utils/text.h>
#include <map>
using namespace utils;


namespace {

class OneOf {
private:
    std::vector<std::string> _choices;

public:
    inline OneOf(std::initializer_list<std::string> choices)
        : _choices(choices)
    { }

    inline friend bool operator==(const std::string &lhs, const OneOf &rhs) noexcept {
        for (const auto &choice: rhs._choices) {
            if (lhs == choice ) { return true; }
        }
        return false;
    }
};

} // namespace

namespace handlers {

inline void no_op(const doctest::AssertData &data) {
    static_cast<void>(data);
}

inline void log_and_continue(const doctest::AssertData &data) {
    std::cout << doctest::failureString(data.m_at) << ": " << data.m_decomp.c_str() << std::endl;
}

[[noreturn]] inline void log_and_exit(const doctest::AssertData &data) {
    log_and_continue(data);
    std::exit(1);
}

static const auto table = std::map<std::string, doctest::detail::assert_handler> {
    { "nullptr",          nullptr          },
    { "no_op",            no_op            },
    { "log_and_continue", log_and_continue },
    { "log_and_exit",     log_and_exit     },
};

} // namespace handlers


namespace actions {

inline void passing_warn() { WARN(true); }
inline void failing_warn() { WARN(false); }
inline void passing_check() { CHECK(true); }
inline void failing_check() { CHECK(false); }
inline void passing_require() { REQUIRE(true); }
inline void failing_require() { REQUIRE(false); }

static const auto table = std::map<std::string, void(*)()> {
    { "passing_warn",    passing_warn    },
    { "failing_warn",    failing_warn    },
    { "passing_check",   passing_check   },
    { "failing_check",   failing_check   },
    { "passing_require", passing_require },
    { "failing_require", failing_require },
};

} // namespace actions


int main(int argc, char **argv) {
    doctest::Context ctx { argc, argv };

    const auto handler_iter = (argc >= 3)? handlers::table.find(argv[1]) : handlers::table.end();
    const auto action_iter  = (argc >= 3)? actions::table.find(argv[2]) : actions::table.end();

    if ((handler_iter != handlers::table.end()) && (action_iter != actions::table.end())) {
        const auto &handler = handler_iter->second;
        const auto &action  = action_iter->second;

        ctx.setAsDefaultForAssertsOutOfTestCases();
        if (handler) { ctx.setAssertHandler(handler); }
        action();
        return 0;
    } else {
        return doctest::Context(argc, argv).run();
    }
}


TEST_CASE("User-code containing a single passing WARN-like assertion") {
    const auto exe = executable::current();

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "passing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "passing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "passing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "passing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }
}


TEST_CASE("User-code containing a single failing WARN-like assertion") {
    const auto exe = executable::current();

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "failing_warn" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "failing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "failing_warn" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "WARNING: false\n");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "failing_warn" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "WARNING: false\n");
    }
}


TEST_CASE("User-code containing a single passing CHECK-like assertion") {
    const auto exe = executable::current();

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "passing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "passing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "passing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "passing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }
}


TEST_CASE("User-code containing a single failing CHECK-like assertion") {
    const auto exe = executable::current();

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "failing_check" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "failing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "failing_check" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "ERROR: false\n");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "failing_check" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "ERROR: false\n");
    }
}


TEST_CASE("User-code containing a single passing REQUIRE-like assertion") {
    const auto exe = executable::current();

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "passing_require" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "passing_require" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "passing_require" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "passing_require" });

        CHECK(result.exit_code == 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }
}


TEST_CASE("User-code containing a single failing REQUIRE-like assertion") {
    const auto exe = executable::current();

    const auto unhandled_exception = OneOf {
        "libc++abi: terminating due to uncaught exception of type doctest::detail::TestFailureException\n", // clang
        "terminate called after throwing an instance of 'doctest::detail::TestFailureException'\n", // GCC
        "", // MSVC
    };

    SUBCASE("With no user-supplied handler") {
        const auto result = exe.run({ "nullptr", "failing_require" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::no_op") {
        const auto result = exe.run({ "no_op", "failing_require" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == unhandled_exception);
        CHECK(result.out == "");
    }

    SUBCASE("With handlers::log_and_continue") {
        const auto result = exe.run({ "log_and_continue", "failing_require" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == unhandled_exception);
        CHECK(result.out == "FATAL ERROR: false\n");
    }

    SUBCASE("With handlers::log_and_exit") {
        const auto result = exe.run({ "log_and_exit", "failing_require" });

        CHECK(result.exit_code != 0);
        CHECK(result.err == "");
        CHECK(result.out == "FATAL ERROR: false\n");
    }
}
