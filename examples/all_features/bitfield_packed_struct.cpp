#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

#ifndef WIN32
struct __attribute__((packed)) P {
    int i;
};
TEST_CASE("packed_struct") {
    P p;
    p.i = 0;
    CHECK(p.i == 0); // error: cannot bind packed field ‘s.S::i’ to ‘int&’
    //CHECK(int(s.i) == 0); // 'useless cast'
}
#else
#pragma pack(push, 1)
struct P {
    int i;
};
TEST_CASE("packed_struct") {
    P p;
    p.i = 0;
    CHECK(p.i == 0); // error: cannot bind packed field ‘s.S::i’ to ‘int&’
    //CHECK(int(s.i) == 0); // 'useless cast'
}
#pragma pack(pop)
#endif
