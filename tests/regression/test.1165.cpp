#include <doctest/doctest.h>
#include <ostream>
#include <iomanip>

namespace {

std::ostream *g_stream = nullptr;

struct object {};

inline std::ostream &operator<<(std::ostream &os, object) {
    // Hold a pointer to the stream so we can recover within the test
    g_stream = &os;

    // Emulate some failure in writing to the stream
    os << "{}";
    os.setstate(std::ios::failbit);

    return os;
}

inline void recover_from_failure() {
    g_stream->setstate(std::ios::goodbit);
}

} // namespace

TEST_CASE("Printing a value which causes a failure in the underlying stream") {
    auto o = object{};

    try {
        static_cast<void>(doctest::toString(o));
        FAIL("Expression should have failed");
    } catch (const std::logic_error &e) {
        CHECK(e.what() == doctest::Contains("Internal doctest error: Output stream is bad"));

        recover_from_failure();
    }
}
