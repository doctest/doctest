#include "doctest/doctest.h"

#include <stdexcept>

struct PosixException : std::exception {
    int code;
    explicit PosixException(int c) : code(c) {}
};

static void throw_posix(int code) { throw PosixException(code); }

TEST_CASE("CHECK_THROWS_WITH_PREDICATE validates exception fields") {
    CHECK_THROWS_WITH_PREDICATE(throw_posix(1337), PosixException, [](const PosixException& e) {
        CHECK(e.code == 1337);
    });
}
