#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <thread>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#include <doctest/parts/private/reporters/debug_output_window.h>
#endif

TEST_CASE("doctest test run owns its temporary memory") {
    INFO("Create active INFO context storage");
    static_cast<void>(doctest::toString(42));

    std::thread worker{[] {
        INFO("Create active INFO context storage in a worker thread");
        static_cast<void>(doctest::toString(42));
        CHECK(true);
    }};
    worker.join();

    CHECK(true);
}

int main(int argc, char **argv) {
#if defined(_MSC_VER) && defined(_DEBUG)
    {
        // This used to own a thread-local stream for the process lifetime.
        doctest::ContextOptions options{};
        doctest::detail::DebugOutputWindowReporter reporter{options};
    }
#endif

    int result = EXIT_SUCCESS;
    {
        doctest::Context context{argc, argv};
        context.setOption("quiet", true);
        context.setOption("no-debug-output", true);
        result = context.run();
    }

#if defined(_MSC_VER) && defined(_DEBUG)
    if (_CrtDumpMemoryLeaks())
        return EXIT_FAILURE;
#endif

    return result;
}
